#pragma once

#include "crycare_protocol.h"

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

typedef enum {
    FSM_STATE_IDLE            = 0,
    FSM_STATE_PRE_ALERT       = 1,
    FSM_STATE_ALERT_SOFT      = 2,
    FSM_STATE_ALERT_ESCALATED = 3,
    FSM_STATE_COOLDOWN        = 4
} fsm_state_t;

typedef enum {
    FSM_STEP_OK        = 0,
    FSM_STEP_INVALID   = 1,
    FSM_STEP_DUPLICATE = 2
} fsm_step_status_t;

typedef struct {
    uint8_t debounce_k;
    uint32_t escalate_after_sec; // wait time until the next alert if no respones.
    uint32_t cooldown_sec; // cooldown time after press the ACK button for mom to care baby

    fsm_state_t state; // current c3 state
    uint8_t duty_cycle; // buzzer volume
    uint8_t consecutive_hits; // avaiable cry packet one by one to compare with debounce_k
    uint32_t last_seq_id; // the latest seq_id
    bool has_seen_packet;  // to detect first last_seq_id (0 <= 0) -> therefrore use has_seen_packet
    uint32_t alert_start_sec; // time when first alert start
    uint32_t cooldown_until_sec; // now_sec + cooldown_sec -> signal to call COOLDOWN state is done, change to IDLE
} alarm_fsm_t;

uint16_t estimate_f0_hz(
    const int16_t *samples,
    size_t num_samples,
    uint32_t sample_rate_hz
);

bool is_cry_qualified(int8_t dbfs, uint8_t conf, uint16_t f0_hz);

bool alarm_fsm_init(
    alarm_fsm_t *fsm,
    uint8_t debounce_k,
    uint32_t escalate_after_sec,
    uint32_t cooldown_sec
);

fsm_step_status_t alarm_fsm_process_packet(
    alarm_fsm_t *fsm,
    const crycare_packet_t *pkt,
    uint32_t now_sec
);
