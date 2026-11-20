#include "mqtt_router.h"

#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

bool mqtt_build_topic(
    const char *device_id,
    const char *channel,
    char *out_buf,
    size_t buf_size
) {
    if (device_id == NULL || channel == NULL || out_buf == NULL || buf_size == 0 || strchr(device_id, '/') != NULL || strchr(channel, '/') != NULL || *device_id == '\0' || *channel == '\0' || strchr(device_id, ' ') != NULL || strchr(channel, ' ') != NULL) return false;
    //ques: how can we compare "contains" a letter in str? -> we use strchr(var, value) -> return an index to the first value in var, else return NULL
    int result = snprintf(out_buf, buf_size, "crycare/%s/%s", device_id, channel); // snprintf trả về int không tính chuỗi \0 do đó khi kiểm tra cần dùng >=
    if (result < 0 || (size_t) result >= buf_size )  return false; // buf_size is size_t therefore use size_t for unwarning
    return true;
}

bool mqtt_build_lwt(
    const char *device_id,
    mqtt_lwt_config_t *lwt
) {
    if (lwt == NULL || device_id == NULL) return false;
    // ques: how can we compare "contains" a letter of str in c?
    if (!mqtt_build_topic(device_id, "status", lwt->topic, MAX_TOPIC_LEN)) return false;
    int is_payloaded = snprintf(lwt->payload, MAX_PAYLOAD_LEN, "{\"status\":\"offline\"}");
    lwt->qos = 1;
    lwt->retain = true;
    if (is_payloaded < 0 || is_payloaded >= MAX_PAYLOAD_LEN) return false;
    return true;
}

bool mqtt_topic_matches_cmd(
    const char *topic,
    const char *expected_device_id
) {
    // ("crycare/%s/cmd", *expected_device_id); cannot use these in C (only for python)
    if (topic == NULL) return false;
    char result[MAX_TOPIC_LEN]; // array is also a pointer variable -> if parameter require a pointer -> we don't need to declare * to an array since it's already a pointer
    if (mqtt_build_topic(expected_device_id, "cmd", result, MAX_TOPIC_LEN)){
        if (!strcmp(topic, result)) return true; // return 0 if true otherwise other values
    }
    return false;
}

bool get_str(const char *data, const char *expected_word){
    const char *start = strchr(data, '"');
    start++;
    const char *end = strchr(start, '"');
    size_t len = (size_t) (end - start);
    if (strncmp(start, expected_word, len) == 0 && len == strlen(expected_word)) return true;
    return false;
}
bool mqtt_parse_command_json(
    const char *json_str,
    mqtt_command_t *out_cmd
) {
    /*What i have learned in this function:
    * 1. strstr(x, y): return a const char* to find an index that exactly match with str y from a const char* x
    * 2. strchr(x,y): the same with strstr() but char y not str y
    * 3. strtol(x, end_ptr, 10): return a long variable with end_ptr represent index, x represent a start index and 10 is decimal.
    * 4. strtoul(x, end_ptr, 10): the same with strtol() but its value has to be size_t no minus variables!;
    * 5. an array is already a pointer itself. Therefore when attach an array to a parameter requiring a pointer -> just attach it without * or &
    * 6. when print an entire string, we use %s, and print its variables without any * or & since const char * or array is already a pointer, we assign its address to present values
    */
    if (out_cmd == NULL || json_str == NULL) {out_cmd->valid = false; return false;}
    const char *pos_cmd = strstr(json_str, "\"cmd\"");
    const char *pos_seq = strstr(json_str, "\"seq_id\"");
    const char *pos_value = strstr(json_str, "\"value\"");
    if (pos_cmd == NULL || pos_seq == NULL) {out_cmd->valid = false; return false;}
    const char *cmd = strchr(pos_cmd, ':');
    const char *seq = strchr(pos_seq, ':');
    char *end_seq = NULL;
    long sequence = strtoul(seq + 1, &end_seq, 10);
    if (end_seq != seq + 1) out_cmd->seq_id = sequence;
    char *end_ptr = NULL;
    long value = 0;
    if (get_str(cmd, "SET_THRESHOLD")){
        out_cmd->cmd_type = 2;
        if (pos_value == NULL) {out_cmd->valid=false; return false;}
        const char *start = strchr(pos_value, ':');
        start++;
        value = strtol(start, &end_ptr,10);
        if (end_ptr != start){
            if (value >= -120 && value <= 0)out_cmd->value = (int32_t) value;
            else {out_cmd->valid = false; return false;}
        }
        
    }
    else if (get_str(cmd, "ACK_MOM")){  
        out_cmd->cmd_type = 1;
        out_cmd->value = 0;
    }
    else if (get_str(cmd, "PING_REQ")){
        out_cmd->cmd_type = 3;
        out_cmd->value = 0;
    }
    else{
        out_cmd->valid = false;
        return false;    
    }
    out_cmd->valid= true;
    // ques: if we use pointer to store string. Then how will we use switch case? -> we use strcmp() instead switch
    return true;
}
