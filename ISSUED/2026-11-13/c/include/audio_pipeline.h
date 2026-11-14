#pragma once

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#define CHUNK_QUEUE_MAX_CAPACITY 16

typedef struct {
    uint32_t seq_id;
    int8_t dbfs;
} audio_chunk_t;

typedef struct {
    audio_chunk_t items[CHUNK_QUEUE_MAX_CAPACITY];
    size_t capacity;
    size_t head;
    size_t tail;
    size_t count;
} chunk_queue_t;

int16_t normalize_i2s_sample(int32_t raw_sample);

int8_t compute_dbfs(const int16_t *samples, size_t num_samples);

bool chunk_queue_init(chunk_queue_t *q, size_t capacity);
bool chunk_queue_push(chunk_queue_t *q, audio_chunk_t chunk);
bool chunk_queue_pop(chunk_queue_t *q, audio_chunk_t *out);
size_t chunk_queue_size(const chunk_queue_t *q);
