#include "alarm_fsm.h"
#include "crycare_protocol.h"

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

uint8_t crycare_compute_xor_checksum(const crycare_packet_t *pkt) {
    if (pkt == NULL) return 0;
    const uint8_t *bytes = (const uint8_t *) pkt;
    uint8_t sum = 0;
    for (size_t i = 0; i < sizeof(crycare_packet_t) -1; i++){
        sum ^= bytes[i];
    }
    return sum;
}

crycare_packet_t crycare_pack_packet(
    uint8_t event_type,
    uint8_t conf,
    int8_t dbfs,
    uint16_t f0_hz,
    uint8_t duty_cycle,
    uint32_t seq_id
) {
    crycare_packet_t pkt= {0};
    pkt.magic = CRYCARE_MAGIC_BYTE;
    pkt.event_type = event_type;
    pkt.conf = conf;
    pkt.dbfs = dbfs;
    pkt.f0_hz = f0_hz;
    pkt.duty_cycle = duty_cycle;
    pkt.seq_id = seq_id;
    pkt.checksum = crycare_compute_xor_checksum(&pkt);
    return pkt;
}

bool crycare_verify_packet(const crycare_packet_t *pkt) {
    if (pkt == NULL || pkt->magic != CRYCARE_MAGIC_BYTE || pkt->checksum != crycare_compute_xor_checksum(pkt)) return false;
    return true;
}

uint16_t estimate_f0_hz(
    const int16_t *samples,
    size_t num_samples,
    uint32_t sample_rate_hz
) {
    /* TODO:
     * 1. If samples == NULL || num_samples < 2 || sample_rate_hz == 0, return 0.
     * 2. Count zero-crossings zc for i = 1 .. num_samples - 1:
     *    (samples[i-1] < 0 && samples[i] >= 0) || (samples[i-1] >= 0 && samples[i] < 0)
     * 3. f0 = lround(((double)zc * (double)sample_rate_hz) / (2.0 * (double)(num_samples - 1)))
     * 4. Clamp to [0, 65535] and return as uint16_t.
     */
    if (samples == NULL || num_samples < 2 || sample_rate_hz == 0) return 0;
    long f0 = 0;
    size_t zc = 0;
    for (size_t i = 1; i <= num_samples - 1; i++){
        if (samples[i-1] < 0 && samples[i] >= 0 || samples[i] < 0 && samples[i-1] >= 0){
            zc++; 
        }
    }
    f0 = lround(((double) zc * (double) sample_rate_hz) / (2.0 * (double) (num_samples -1)));
    if ( f0 < 0) return 0;
    if (f0 > 65535) return 65535;
    return (uint16_t) f0;
}

bool is_cry_qualified(int8_t dbfs, uint8_t conf, uint16_t f0_hz) {
    /* TODO: Return true iff dbfs >= -35 && conf >= 70 && f0_hz >= 300 && f0_hz <= 600 */
    if (dbfs >= -35 && conf >= 70 && f0_hz >= 300 && f0_hz <= 600) return true;
    return false;
}

bool alarm_fsm_init( 
    alarm_fsm_t *fsm, 
    uint8_t debounce_k,
    uint32_t escalate_after_sec,
    uint32_t cooldown_sec
) {
   if ( fsm == NULL || debounce_k == 0) return false;
    fsm->state = FSM_STATE_IDLE;
    fsm->duty_cycle = 0;
    fsm->consecutive_hits = 0;
    fsm->last_seq_id = 0;
    fsm->has_seen_packet = false;
    fsm->alert_start_sec = 0;
    fsm->cooldown_until_sec = 0;
    fsm->cooldown_sec = cooldown_sec;
    fsm->escalate_after_sec = escalate_after_sec;
    fsm->debounce_k = debounce_k;
    return true;
    
}

static void process_cry_packet(alarm_fsm_t *fsm, uint32_t now_sec){
    if (fsm->state == FSM_STATE_COOLDOWN){
                fsm->duty_cycle = 0; // khi cooldown, ta phải tắt âm lượng, vì trước đó có điều kiện if else kiểm tra now_sec >= cooldown_until_sec
    }
    if((fsm->state == FSM_STATE_PRE_ALERT || fsm->state == FSM_STATE_IDLE) && fsm->consecutive_hits < fsm->debounce_k){
        fsm->consecutive_hits++;
        fsm->state = FSM_STATE_PRE_ALERT;
    }
    if(fsm->consecutive_hits >= fsm->debounce_k){
        if (fsm->escalate_after_sec ==0){
            fsm->state = FSM_STATE_ALERT_ESCALATED;
            fsm->duty_cycle = 100;
        }
        else{
        fsm->state = FSM_STATE_ALERT_SOFT;
        fsm->duty_cycle = 50;
        fsm->alert_start_sec = now_sec;
        }
    }
    if(fsm->state == FSM_STATE_ALERT_SOFT && now_sec - fsm->alert_start_sec >= fsm->escalate_after_sec){
        fsm->state = FSM_STATE_ALERT_ESCALATED;
        fsm->duty_cycle = 100;
    }


}
fsm_step_status_t alarm_fsm_process_packet(
    alarm_fsm_t *fsm,
    const crycare_packet_t *pkt,
    uint32_t now_sec // realtime
    /*Workflow:
    * 1. check if packet is invalid by NULL or verify or duplicate
    * 2. test with pkt->event_type: if ACK_MOM -> then change to cooldown and silient, cooldown_until_sec = sec_not + cooldown_time, and hit = 0 (reset everything)
    * 3. if COOLDOWN and now_sec > cooldown_until_sec => then reset everything and change state to IDLE
    * 4. if CRY_ALERT -> analyze with each alarm_fsm_t state.
    * 5. check if crycare_verify_packet(pkt) since the 1 workflow only or not and doesn't mean crycare_verify have run
    * 6. if state is IDLE | PRE_ALERT -> hits++ and change state into PRE_ALERT
    * 7. if not crycare_verify_packet -> hits = 0 and duty = 0 and state - IDLE. Otherwise, back to 6
    * 8. if hits >= K: if escalate_time == 0 -> then state = ESCALATED and duty = 100. Otherwise: state = soft and alert_start_time = now_Sec
    * 9. if now_sec - alert_start >= escalated_time -> state = ESCALATED and duty = 100.
    * 10. Only pkt->event_type = ACK_MOM do we change into COOLDOWN and duty = 0; even noise cannot change
    * 11. return status_OK (since we use pointer not local variables) */
) {
    if (fsm == NULL || pkt == NULL || !crycare_verify_packet(pkt) || pkt->event_type != 1 && pkt->event_type !=2 && pkt->event_type != 3) return FSM_STEP_INVALID;
    
    // duplicate check
    if(fsm->has_seen_packet && pkt->seq_id <= fsm->last_seq_id) return FSM_STEP_DUPLICATE;
    fsm->last_seq_id = pkt->seq_id;
    fsm->has_seen_packet = true;
    if(fsm->state == FSM_STATE_COOLDOWN && now_sec >= fsm->cooldown_until_sec){
        fsm->duty_cycle = 0; // buzzer volume
        fsm->consecutive_hits = 0; // số cry_alert liên tiếp
        fsm->state = FSM_STATE_IDLE; // sẵn sàng
    }
    
    if (pkt->event_type == CRYCARE_EVENT_ACK_MOM){ // ans: ta dùng if vì event cần xử lý độc lập và không phụ thuộc if trước đó (do đó dùng if/else-if sẽ bị delay 1 gói)
        fsm->duty_cycle = 0;
        fsm->state = FSM_STATE_COOLDOWN;
        fsm->cooldown_until_sec = now_sec + fsm->cooldown_sec;
        fsm->consecutive_hits = 0;
    }
    if (pkt->event_type == CRYCARE_EVENT_CRY_ALERT){
        if (is_cry_qualified(pkt->dbfs, pkt->conf, pkt->f0_hz))process_cry_packet(fsm, now_sec);
        else if(!is_cry_qualified(pkt->dbfs, pkt->conf, pkt->f0_hz) && (fsm->state == FSM_STATE_PRE_ALERT || fsm->state == FSM_STATE_IDLE)){
            fsm->duty_cycle = 0;
            fsm->consecutive_hits = 0;
            fsm->state = FSM_STATE_IDLE;
        }
    }
    if (pkt->event_type == CRYCARE_EVENT_PING){
        if(fsm->state == FSM_STATE_ALERT_SOFT && now_sec - fsm->alert_start_sec >= fsm->escalate_after_sec){
            fsm->state = FSM_STATE_ALERT_ESCALATED;
            fsm->duty_cycle = 100;
        }
    }
    return FSM_STEP_OK;
}