#pragma once

#include <stdbool.h>
#include <stdint.h>

#define CRYCARE_MAGIC_BYTE 0xCC

typedef enum {
    CRYCARE_EVENT_CRY_ALERT = 0x01,
    CRYCARE_EVENT_ACK_MOM   = 0x02,
    CRYCARE_EVENT_PING      = 0x03
} crycare_event_t;

typedef struct __attribute__((packed)) {
    uint8_t magic;
    uint8_t event_type;
    uint8_t conf;
    int8_t dbfs;
    uint16_t f0_hz;
    uint8_t duty_cycle;
    uint8_t reserved[4];
    uint32_t seq_id;
    uint8_t checksum;
} crycare_packet_t;

_Static_assert(sizeof(crycare_packet_t) == 16, "crycare_packet_t must be exactly 16 bytes");

uint8_t crycare_compute_xor_checksum(const crycare_packet_t *pkt);

crycare_packet_t crycare_pack_packet(
    uint8_t event_type,
    uint8_t conf,
    int8_t dbfs,
    uint16_t f0_hz,
    uint8_t duty_cycle,
    uint32_t seq_id
);

bool crycare_verify_packet(const crycare_packet_t *pkt);
