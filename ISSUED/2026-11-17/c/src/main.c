/* Supplied single-threaded C11 host replay driver. */
#include "mqtt_outbox.h"

#include <inttypes.h>
#include <stdio.h>
#include <string.h>

int main(void) {
    size_t capacity = 0;
    size_t num_commands = 0;

    if (scanf("%zu %zu", &capacity, &num_commands) != 2) {
        return 0;
    }

    mqtt_outbox_t box;
    if (!outbox_init(&box, capacity)) {
        fprintf(stderr, "Invalid outbox capacity: %zu\n", capacity);
        return 2;
    }

    char cmd[32];
    for (size_t i = 0; i < num_commands; ++i) {
        if (scanf("%31s", cmd) != 1) {
            break;
        }

        if (strcmp(cmd, "BROKER_UP") == 0) {
            outbox_set_broker_status(&box, true);
        } else if (strcmp(cmd, "BROKER_DOWN") == 0) {
            outbox_set_broker_status(&box, false);
        } else if (strcmp(cmd, "ENQUEUE") == 0) {
            telemetry_event_data_t ev;
            memset(&ev, 0, sizeof(ev));
            unsigned int ev_type = 0;
            unsigned int conf = 0;
            int dbfs = 0;
            if (scanf("%31s %u %" SCNu32 " %" SCNu32 " %u %d %" SCNu16,
                      ev.device_id, &ev_type, &ev.seq_id, &ev.ts_sec,
                      &conf, &dbfs, &ev.f0_hz) != 7) {
                return 1;
            }
            ev.event_type = (uint8_t)ev_type;
            ev.conf = (uint8_t)conf;
            ev.dbfs = (int8_t)dbfs;
            printf("%s\n", outbox_enqueue(&box, &ev) ? "OK" : "FULL");
        } else if (strcmp(cmd, "PUBLISH_NEXT") == 0) {
            char json_buf[JSON_BUF_SIZE];
            if (outbox_next_publish(&box, json_buf, sizeof(json_buf))) {
                printf("PUB %s\n", json_buf);
            } else {
                printf("NONE\n");
            }
        } else if (strcmp(cmd, "PUBACK") == 0) {
            uint32_t seq_id = 0;
            if (scanf("%" SCNu32, &seq_id) != 1) {
                return 1;
            }
            printf("%s\n", outbox_ack(&box, seq_id) ? "ACK_OK" : "ACK_ERR");
        } else if (strcmp(cmd, "SIZE") == 0) {
            printf("%zu\n", outbox_size(&box));
        } else if (strcmp(cmd, "FORMAT_SMALL") == 0) {
            size_t small_sz = 0;
            telemetry_event_data_t ev;
            memset(&ev, 0, sizeof(ev));
            unsigned int ev_type = 0;
            unsigned int conf = 0;
            int dbfs = 0;
            if (scanf("%zu %31s %u %" SCNu32 " %" SCNu32 " %u %d %" SCNu16,
                      &small_sz, ev.device_id, &ev_type, &ev.seq_id, &ev.ts_sec,
                      &conf, &dbfs, &ev.f0_hz) != 8) {
                return 1;
            }
            ev.event_type = (uint8_t)ev_type;
            ev.conf = (uint8_t)conf;
            ev.dbfs = (int8_t)dbfs;
            char small_buf[JSON_BUF_SIZE];
            if (small_sz > sizeof(small_buf)) {
                small_sz = sizeof(small_buf);
            }
            int len = format_telemetry_json(&ev, small_buf, small_sz);
            if (len < 0) {
                printf("TRUNCATED\n");
            } else {
                printf("LEN %d %s\n", len, small_buf);
            }
        } else {
            return 1;
        }
    }

    return 0;
}
