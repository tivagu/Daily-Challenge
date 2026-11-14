/* Supplied single-threaded C11 host replay driver. */
#include "audio_pipeline.h"
#include "crycare_protocol.h"

#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(void) {
    size_t capacity = 0;
    int threshold_dbfs = -35;
    size_t num_commands = 0;

    if (scanf("%zu %d %zu", &capacity, &threshold_dbfs, &num_commands) != 3) {
        return 0;
    }

    chunk_queue_t queue;
    if (!chunk_queue_init(&queue, capacity)) {
        fprintf(stderr, "Invalid queue capacity: %zu\n", capacity);
        return 2;
    }

    char cmd[32];
    for (size_t i = 0; i < num_commands; ++i) {
        if (scanf("%31s", cmd) != 1) {
            break;
        }

        if (strcmp(cmd, "INGEST") == 0) {
            uint32_t seq_id = 0;
            size_t n = 0;
            if (scanf("%" SCNu32 " %zu", &seq_id, &n) != 2) {
                return 1;
            }
            int16_t *samples = NULL;
            if (n > 0) {
                samples = (int16_t *)malloc(n * sizeof(int16_t));
                if (samples == NULL) {
                    return 2;
                }
                for (size_t j = 0; j < n; ++j) {
                    int32_t raw = 0;
                    if (scanf("%" SCNd32, &raw) != 1) {
                        free(samples);
                        return 1;
                    }
                    samples[j] = normalize_i2s_sample(raw);
                }
            }
            int8_t dbfs = compute_dbfs(samples, n);
            free(samples);

            if ((int)dbfs >= threshold_dbfs) {
                audio_chunk_t chunk = { .seq_id = seq_id, .dbfs = dbfs };
                bool ok = chunk_queue_push(&queue, chunk);
                printf("ACTIVE %d %s\n", (int)dbfs, ok ? "OK" : "FULL");
            } else {
                printf("SILENT %d\n", (int)dbfs);
            }
        } else if (strcmp(cmd, "POP_PACK") == 0) {
            unsigned int event_type = 0;
            unsigned int conf = 0;
            uint16_t f0_hz = 0;
            unsigned int duty = 0;
            if (scanf("%u %u %" SCNu16 " %u", &event_type, &conf, &f0_hz, &duty) != 4) {
                return 1;
            }

            audio_chunk_t chunk;
            if (!chunk_queue_pop(&queue, &chunk)) {
                printf("EMPTY\n");
            } else {
                crycare_packet_t pkt = crycare_pack_packet(
                    (uint8_t)event_type,
                    (uint8_t)conf,
                    chunk.dbfs,
                    f0_hz,
                    (uint8_t)duty,
                    chunk.seq_id
                );
                printf("PKT %" PRIu32 " %d %02X\n",
                       pkt.seq_id,
                       (int)pkt.dbfs,
                       (unsigned int)pkt.checksum);
            }
        } else if (strcmp(cmd, "VERIFY") == 0) {
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
            if (crycare_verify_packet(&pkt)) {
                printf("VALID %u %" PRIu32 " %d\n",
                       (unsigned int)pkt.event_type,
                       pkt.seq_id,
                       (int)pkt.dbfs);
            } else {
                printf("INVALID\n");
            }
        } else if (strcmp(cmd, "SIZE") == 0) {
            printf("%zu\n", chunk_queue_size(&queue));
        } else {
            return 1;
        }
    }

    return 0;
}
