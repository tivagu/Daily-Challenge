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
    for (size_t i = 1; i <= num_samples; i++){
        if (samples[i-1] < 0 && samples[i] > 0 || samples[i] <0 && samples[i-1] > 0){
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

fsm_step_status_t alarm_fsm_process_packet(
    alarm_fsm_t *fsm,
    const crycare_packet_t *pkt,
    uint32_t now_sec
) {
    if (fsm == NULL || pkt == NULL || !crycare_verify_packet(pkt) || pkt->event_type != 1 && pkt->event_type !=2 && pkt->event_type != 3) return FSM_STEP_INVALID;
    
    // duplicate check
    else if(fsm->has_seen_packet && pkt->seq_id <= fsm->last_seq_id) return FSM_STEP_DUPLICATE;
    fsm->last_seq_id = pkt->seq_id;
    fsm->has_seen_packet = true;
    if(fsm->state == FSM_STATE_COOLDOWN && now_sec >= fsm->cooldown_until_sec){
        fsm->duty_cycle = 0;
        fsm->consecutive_hits = 0;
        fsm->state = FSM_STATE_IDLE;
    }
    
    else if (pkt->event_type == CRYCARE_EVENT_ACK_MOM){
        fsm->duty_cycle = 0;
        fsm->state = FSM_STATE_COOLDOWN;
        fsm->cooldown_sec = now_sec;
    }
    else if (pkt->event_type == CRYCARE_EVENT_CRY_ALERT){
        if (fsm->state == FSM_STATE_COOLDOWN){
            // question: if COOLDOWN, then what is our output when cry_alert occurs?
        }
        else if (fsm->state == CRYCARE_EVENT_CRY_ALERT){
            if (fsm->duty_cycle == 0){
                fsm->state = FSM_STATE_PRE_ALERT;
                
            }
            else if (fsm->duty_cycle == 50){
                fsm->state = FSM_STATE_ALERT_SOFT;
                // question: what i know about duty_cycle is the cry frequence of baby. Then we can't exactly know if it 50 or louder -> why don't we set greater than ?
                // also: what do you mean by recording alert_start_sec once the streak reached debounce_k and escalate_after_sec ==0?
                // how can we handle and let theirs value run automatically?
            
            }
        }
        else{
            // i don't know what will we do with it though i have read the workflow
        }
    }
}
