#pragma once

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#define MAX_TOPIC_LEN     64
#define MAX_PAYLOAD_LEN   64
#define MAX_DEVICE_ID_LEN 32

typedef enum {
    CMD_UNKNOWN       = 0,
    CMD_ACK_MOM       = 1,
    CMD_SET_THRESHOLD = 2,
    CMD_PING_REQ      = 3
} mqtt_cmd_type_t;

typedef struct {
    mqtt_cmd_type_t cmd_type;
    uint32_t seq_id;
    int32_t value;
    bool valid;
} mqtt_command_t;

typedef struct {
    char topic[MAX_TOPIC_LEN];
    char payload[MAX_PAYLOAD_LEN];
    uint8_t qos;
    bool retain;
} mqtt_lwt_config_t;

bool mqtt_build_topic(
    const char *device_id,
    const char *channel,
    char *out_buf,
    size_t buf_size
);

bool mqtt_build_lwt(
    const char *device_id,
    mqtt_lwt_config_t *lwt
);

bool mqtt_topic_matches_cmd(
    const char *topic,
    const char *expected_device_id
);

bool mqtt_parse_command_json(
    const char *json_str,
    mqtt_command_t *out_cmd
);
