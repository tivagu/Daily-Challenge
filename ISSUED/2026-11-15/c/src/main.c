/* Supplied single-threaded C11 host replay driver. */
#include "alarm_fsm.h"
#include "crycare_protocol.h"

#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static const char *state_to_str(fsm_state_t st) {
    switch (st) {
        case FSM_STATE_IDLE:            return "IDLE";
        case FSM_STATE_PRE_ALERT:       return "PRE_ALERT";
        case FSM_STATE_ALERT_SOFT:      return "ALERT_SOFT";
        case FSM_STATE_ALERT_ESCALATED: return "ALERT_ESCALATED";
        case FSM_STATE_COOLDOWN:        return "COOLDOWN";
        default:                        return "UNKNOWN";
    }
}

int main(void) {
    unsigned int k = 0;
    uint32_t escalate_sec = 0;
    uint32_t cooldown_sec = 0;
    size_t num_commands = 0;

    if (scanf("%u %" SCNu32 " %" SCNu32 " %zu", &k, &escalate_sec, &cooldown_sec, &num_commands) != 4) {
        return 0;
    }

    alarm_fsm_t fsm;
    if (!alarm_fsm_init(&fsm, (uint8_t)k, escalate_sec, cooldown_sec)) {
        fprintf(stderr, "Invalid FSM config\n");
        return 2;
    }

    char cmd[32];
    for (size_t i = 0; i < num_commands; ++i) {
        if (scanf("%31s", cmd) != 1) {
            break;
        }

        if (strcmp(cmd, "F0") == 0) {
            uint32_t sr = 0;
            size_t n = 0;
            if (scanf("%" SCNu32 " %zu", &sr, &n) != 2) {
                return 1;
            }
            int16_t *samples = NULL;
            if (n > 0) {
                samples = (int16_t *)malloc(n * sizeof(int16_t));
                if (samples == NULL) {
                    return 2;
                }
                for (size_t j = 0; j < n; ++j) {
                    int s = 0;
                    if (scanf("%d", &s) != 1) {
                        free(samples);
                        return 1;
                    }
                    samples[j] = (int16_t)s;
                }
            }
            uint16_t f0 = estimate_f0_hz(samples, n, sr);
            free(samples);
            printf("F0 %" PRIu16 "\n", f0);
        } else if (strcmp(cmd, "SEND_FRAME") == 0) {
            uint32_t now_sec = 0;
            uint32_t seq_id = 0;
            unsigned int ev = 0;
            unsigned int conf = 0;
            int dbfs = 0;
            uint16_t f0_hz = 0;
            if (scanf("%" SCNu32 " %" SCNu32 " %u %u %d %" SCNu16,
                      &now_sec, &seq_id, &ev, &conf, &dbfs, &f0_hz) != 6) {
                return 1;
            }
            crycare_packet_t pkt = crycare_pack_packet(
                (uint8_t)ev,
                (uint8_t)conf,
                (int8_t)dbfs,
                f0_hz,
                fsm.duty_cycle,
                seq_id
            );
            fsm_step_status_t st = alarm_fsm_process_packet(&fsm, &pkt, now_sec);
            if (st == FSM_STEP_INVALID) {
                printf("INVALID\n");
            } else if (st == FSM_STEP_DUPLICATE) {
                printf("DUPLICATE\n");
            } else {
                printf("%s duty=%u hits=%u ck=%02X\n",
                       state_to_str(fsm.state),
                       (unsigned int)fsm.duty_cycle,
                       (unsigned int)fsm.consecutive_hits,
                       (unsigned int)pkt.checksum);
            }
        } else if (strcmp(cmd, "RAW_PKT") == 0) {
            uint32_t now_sec = 0;
            if (scanf("%" SCNu32, &now_sec) != 1) {
                return 1;
            }
            uint8_t raw_bytes[16] = {0};
            for (int b = 0; b < 16; ++b) {
                unsigned int val = 0;
                if (scanf("%x", &val) != 1) {
                    return 1;
                }
                raw_bytes[b] = (uint8_t)(val & 0xFFu);
            }
            crycare_packet_t pkt;
            memcpy(&pkt, raw_bytes, sizeof(pkt));
            fsm_step_status_t st = alarm_fsm_process_packet(&fsm, &pkt, now_sec);
            if (st == FSM_STEP_INVALID) {
                printf("INVALID\n");
            } else if (st == FSM_STEP_DUPLICATE) {
                printf("DUPLICATE\n");
            } else {
                printf("%s duty=%u hits=%u ck=%02X\n",
                       state_to_str(fsm.state),
                       (unsigned int)fsm.duty_cycle,
                       (unsigned int)fsm.consecutive_hits,
                       (unsigned int)pkt.checksum);
            }
        } else if (strcmp(cmd, "STATUS") == 0) {
            printf("%s duty=%u hits=%u seq=%" PRIu32 "\n",
                   state_to_str(fsm.state),
                   (unsigned int)fsm.duty_cycle,
                   (unsigned int)fsm.consecutive_hits,
                   fsm.last_seq_id);
        } else {
            return 1;
        }
    }

    return 0;
}
