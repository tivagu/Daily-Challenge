#include "mqtt_outbox.h"

#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const char *telemetry_event_name(uint8_t event_type) {
    switch (event_type)
    {
    case 1:
        return "CRY_ALERT";
    case 2:
        return "ACK_MOM";
    case 3:
        return "PING";
    default:
        return NULL;
    }
}

int format_telemetry_json(
    const telemetry_event_data_t *ev,
    char *out_buf,
    size_t buf_size
) {
    /*WORKFLOW:
    * 1. Since this is my very first time coding JSON in C, i don't know which libary to structure JSON in c
    * 2. all i figure out are: stdio.h -> snprintf(variable_1, variable_2, variable_3,...) -> JSON basically mean char[]
    * 3. check the condition whether: buf_size ==0 or pointer is invalid or invalid event_type return -1
    * 4. finally wirte a single-line JSON string no extra spaces*/
    if (out_buf == NULL || ev == NULL || telemetry_event_name(ev->event_type) == NULL || buf_size == 0) return -1;
    int result = snprintf(out_buf, buf_size, "{\"device_id\":\"%s\",\"event\":\"%s\",\"seq_id\":%" PRIu32",\"ts\":%"PRIu32",\"conf\":%"PRIu8",\"dbfs\":%"PRId8",\"f0_hz\":%"PRIu16"}", ev->device_id, telemetry_event_name(ev->event_type), ev->seq_id, ev->ts_sec, ev->conf, ev->dbfs, ev->f0_hz); // ques: how do we present "" in printf()?    
    if(result < 0 || (size_t) result  >= buf_size) return -1;
    return result;
}

bool outbox_init(mqtt_outbox_t *box, size_t capacity) {
    if (box == NULL|| capacity > OUTBOX_MAX_CAPACITY || capacity < 1) return false;
    box->capacity = capacity;
    box->head = 0;
    box->count = 0;
    box->tail = 0;
    box->broker_connected = false;
    box->has_in_flight = false;
    box->in_flight_seq = 0;
    //ques: what about items if no parameters include telemetry_event_data_t?
    return true;
}

void outbox_set_broker_status(mqtt_outbox_t *box, bool connected) {
    if(box == NULL) return;
    box->broker_connected = connected;
    if (connected == false){
        box->has_in_flight = false;
        box->in_flight_seq = 0;
    }
    
}

bool outbox_enqueue(mqtt_outbox_t *box, const telemetry_event_data_t *ev) {
   if (box == NULL || ev == NULL || box->count == box->capacity) return false;
    box->items[box->tail] = *ev;
    box->tail = (box->tail + 1) % box->capacity;
    box->count++;
   return true;
}

bool outbox_next_publish(mqtt_outbox_t *box, char *json_buf, size_t buf_size) {
    if (box == NULL || json_buf == NULL || buf_size == 0 || box->count == 0 || !box->broker_connected || box->has_in_flight) return false;
    const telemetry_event_data_t event = box->items[box->head]; 
    int result = format_telemetry_json(&event, json_buf,buf_size);
    // ques: when will we know broker is connected? what situations do we need to active it? has_in_flight mean a JSON file has sent to BackEnd and wait to have a respone back? then how can we know "respone back"?
    if (box->has_in_flight == false && result != -1) {
        box->has_in_flight = true;
        box->in_flight_seq = event.seq_id;
        return true; // QUESTION: WHY RESULT ALWAYS == BUF_SIZE? (SINCE OUR FUNCTION HAS ALREADY CHECK <0 AND > BUF_SIZE)
    }
        return false;
}

bool outbox_ack(mqtt_outbox_t *box, uint32_t seq_id) {
    if (box == NULL || !box->has_in_flight  || box->count == 0) return false;
    if(box->in_flight_seq == seq_id){
        box->count--;
        box->head = (box->head + 1) % box->capacity;
        box->has_in_flight = false;
        return true;
    }
    return false;
}

size_t outbox_size(const mqtt_outbox_t *box) {
    if (box == NULL) return 0;
    return box->count;    
}
