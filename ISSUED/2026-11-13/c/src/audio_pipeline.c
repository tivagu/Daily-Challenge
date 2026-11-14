#include "audio_pipeline.h"
#include "crycare_protocol.h"

#include <math.h>
#include <stdio.h>
#include <stdlib.h>



int16_t normalize_i2s_sample(int32_t raw_sample) {
    /*workflow: get a 32-bits from I2S (raw_sample)
    * 1. typecast from 32-bits to 16-bits using bitwise. (which function to implement?)
    * 2. finally, return 16-bits values*/
    int16_t result = (int16_t) (raw_sample >> 16); // since typecase has the highest priority -> we use (raw_sample) to proces raw_sample first
    return result;
}

int8_t compute_dbfs(const int16_t *samples, size_t num_samples) {
    /* workflow
     * 1. return NULL if samples == NULL, num_samples == 0 | and what exactly does sum_sq mean?  
     * 2. rms = sqrt((double) sum_sq / (double) num_samples )
     * 3. dbfs = lround(20.0 * log10(rms/32768.0)) clamped (-120, 0)
     * 4. return result*/
    if (samples == NULL || num_samples == 0) return -120; // why -120?
    int64_t sum_sq = 0;
    for (size_t i = 0; i < num_samples; i++){ // why size_t but not int? -> match with num_samples condition datatype
        int64_t s = samples[i];
        sum_sq += s * s;
    }
    if (sum_sq == 0) return -120; // if avoid log10(0) -> why -120?
    double rms = sqrt((double)sum_sq / (double)num_samples);
    long dbfs = lround(20.0 * log10(rms/32768.0));
    if (dbfs < -120) return -120;
    if (dbfs > 0) return 0;
    return (int8_t) dbfs;    
}

bool chunk_queue_init(chunk_queue_t *q, size_t capacity) {
    // chunk_queue_t from audio_pipeline.h
    if (q == NULL || capacity == 0 || capacity > CHUNK_QUEUE_MAX_CAPACITY) return false;
    q->capacity = capacity;
    q->head = 0;
    q->tail = 0;
    q->count = 0;
    return true;
}

bool chunk_queue_push(chunk_queue_t *q, audio_chunk_t chunk) {
    if ( q == NULL || q->count == q->capacity) return false;
    // since q->item and chunk have the same datatype -> we can assign them at once;
    q->items[q->tail] = chunk; // audio_chunk_t have id and dbfs members -> we assign into items[q->tail]
    q->tail = (q->tail + 1) % q->capacity;
    q->count++;
    return true; 
}

bool chunk_queue_pop(chunk_queue_t *q, audio_chunk_t *out) {
    if (q == NULL || out == NULL || q->count == 0) return false; // check q NULL first, then check count of q later
    *out = q->items[q->head]; // we get data, since out and items have the same datatype
    q->head = (q->head + 1) % q->capacity; 
    q->count--;
    return true;
}

size_t chunk_queue_size(const chunk_queue_t *q) {
    if (q == NULL) return 0;
    return q->count;
}

uint8_t crycare_compute_xor_checksum(const crycare_packet_t *pkt) {
    /*Workflow:
    * 1. Check if pkt is NULL or not
    * 2. create a pointer variable that store and point to every bytes of struct. 
    * 3. create an uint8_t sum variable to store XOR calculation
    * 4. loop and XOR every bytes of the struct
    * 5. return sum*/
    if (pkt == NULL) return 0;
    const uint8_t *bytes = (const uint8_t *) pkt; // typecast pkt (since pkt is a struct pointer with jump every 16 bytes instead of 1)
    uint8_t sum = 0;
    for (size_t i = 0; i < sizeof(crycare_packet_t) - 1; i++){ // since pkt is struct pointer -> 8 bytes
        sum ^= bytes[i]; // question: what does it work exactly?
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
    // init a complete crycare_packet_t after calculate dbfs and sed_id
    crycare_packet_t pkt = {0};
    pkt.magic = CRYCARE_MAGIC_BYTE; // why 0xCC?
    pkt.event_type = event_type;
    pkt.conf = conf;
    pkt.dbfs = dbfs;
    pkt.duty_cycle = duty_cycle;
    pkt.f0_hz = f0_hz;
    pkt.seq_id = seq_id;

    pkt.checksum = crycare_compute_xor_checksum(&pkt);
    return pkt;
    
}

bool crycare_verify_packet(const crycare_packet_t *pkt) {
    // workflow: if pkt not NULL and magic is the same as send and XOR is equal -> return
    if (pkt != NULL && pkt->magic == CRYCARE_MAGIC_BYTE && pkt->checksum == crycare_compute_xor_checksum(pkt)) return true;
    return false;
}
