#pragma once

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#define OUTBOX_MAX_CAPACITY 8
#define JSON_BUF_SIZE       256
#define DEVICE_ID_MAX_LEN   32

typedef enum {
    TELEMETRY_CRY_ALERT = 1,
    TELEMETRY_ACK_MOM   = 2,
    TELEMETRY_PING      = 3
} telemetry_event_t;

typedef struct {
    char device_id[DEVICE_ID_MAX_LEN];
    uint8_t event_type;
    uint32_t seq_id;
    uint32_t ts_sec;
    uint8_t conf;
    int8_t dbfs;
    uint16_t f0_hz;
} telemetry_event_data_t;

typedef struct {
    telemetry_event_data_t items[OUTBOX_MAX_CAPACITY];
    size_t capacity;
    size_t head;
    size_t tail;
    size_t count;
    bool broker_connected;
    uint32_t in_flight_seq;
    bool has_in_flight;
} mqtt_outbox_t;

const char *telemetry_event_name(uint8_t event_type);

int format_telemetry_json(
    const telemetry_event_data_t *ev,
    char *out_buf,
    size_t buf_size
);

bool outbox_init(mqtt_outbox_t *box, size_t capacity);

void outbox_set_broker_status(mqtt_outbox_t *box, bool connected);

bool outbox_enqueue(mqtt_outbox_t *box, const telemetry_event_data_t *ev);

bool outbox_next_publish(mqtt_outbox_t *box, char *json_buf, size_t buf_size);

bool outbox_ack(mqtt_outbox_t *box, uint32_t seq_id);

size_t outbox_size(const mqtt_outbox_t *box);
