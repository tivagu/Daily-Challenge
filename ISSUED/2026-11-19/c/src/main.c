/* Supplied single-threaded C11 host replay driver. */
#include "mqtt_router.h"

#include <inttypes.h>
#include <stdio.h>
#include <string.h>

static const char *cmd_name(mqtt_cmd_type_t t) {
    switch (t) {
        case CMD_ACK_MOM:       return "ACK_MOM";
        case CMD_SET_THRESHOLD: return "SET_THRESHOLD";
        case CMD_PING_REQ:      return "PING_REQ";
        default:                return "UNKNOWN";
    }
}

int main(void) {
    char expected_dev[MAX_DEVICE_ID_LEN] = {0};
    size_t num_commands = 0;

    if (scanf("%31s %zu", expected_dev, &num_commands) != 2) {
        return 0;
    }

    char cmd[32];
    for (size_t i = 0; i < num_commands; ++i) {
        if (scanf("%31s", cmd) != 1) {
            break;
        }

        if (strcmp(cmd, "BUILD_TOPIC") == 0) {
            char dev[64] = {0};
            char chan[64] = {0};
            size_t bsz = 0;
            if (scanf("%63s %63s %zu", dev, chan, &bsz) != 3) {
                return 1;
            }
            char out[MAX_TOPIC_LEN];
            if (bsz > sizeof(out)) {
                bsz = sizeof(out);
            }
            if (mqtt_build_topic(dev, chan, out, bsz)) {
                printf("TOPIC %s\n", out);
            } else {
                printf("TOPIC_ERR\n");
            }
        } else if (strcmp(cmd, "BUILD_LWT") == 0) {
            char dev[64] = {0};
            if (scanf("%63s", dev) != 1) {
                return 1;
            }
            mqtt_lwt_config_t lwt;
            memset(&lwt, 0, sizeof(lwt));
            if (mqtt_build_lwt(dev, &lwt)) {
                printf("LWT %s qos=%u retain=%u %s\n",
                       lwt.topic,
                       (unsigned int)lwt.qos,
                       lwt.retain ? 1u : 0u,
                       lwt.payload);
            } else {
                printf("LWT_ERR\n");
            }
        } else if (strcmp(cmd, "ROUTE") == 0) {
            char topic[MAX_TOPIC_LEN] = {0};
            char json_line[256] = {0};
            if (scanf("%63s", topic) != 1) {
                return 1;
            }
            if (fgets(json_line, sizeof(json_line), stdin) == NULL) {
                return 1;
            }
            size_t len = strlen(json_line);
            while (len > 0 && (json_line[len - 1] == '\n' || json_line[len - 1] == '\r')) {
                json_line[--len] = '\0';
            }
            const char *payload = json_line;
            while (*payload == ' ' || *payload == '\t') {
                ++payload;
            }

            if (!mqtt_topic_matches_cmd(topic, expected_dev)) {
                printf("IGNORE_TOPIC\n");
            } else {
                mqtt_command_t parsed;
                memset(&parsed, 0, sizeof(parsed));
                if (!mqtt_parse_command_json(payload, &parsed) || !parsed.valid) {
                    printf("BAD_CMD\n");
                } else {
                    printf("CMD %s seq=%" PRIu32 " val=%" PRId32 "\n",
                           cmd_name(parsed.cmd_type),
                           parsed.seq_id,
                           parsed.value);
                }
            }
        } else {
            return 1;
        }
    }

    return 0;
}
