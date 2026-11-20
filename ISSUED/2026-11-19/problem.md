# EDGE-2026-11-19 — CryCare C11 MQTT Topic Router & Command Parser

## 1. Challenge card

- Weekly block (16–22 November): CryCare Cloud Telemetry & Bidirectional MQTT.
- Module: C11 host replay of MQTT topic construction, Last Will and Testament (LWT) configuration, downlink topic matching, and downlink JSON command parsing.
- Format A: STDIN–STDOUT with a C11 project handoff. Self-authored practice; no `CMakeLists.txt` is supplied (write it yourself).
- Session: **90 minutes (1.5 hours)** minimum, including CMake setup, design, coding, testing, and same-day debrief.
- Tools: C11 standard library (`<stdio.h>`, `<stdlib.h>`, `<string.h>`, `<stdint.h>`, `<stdbool.h>`), CMake, Ninja, and a host C compiler. Python 3.10+ runs the checker and packager.

## 2. Scenario & objective

In `EDGE-2026-11-17`, you implemented the uplink JSON serializer and offline outbox on ESP32-S3. Now CryCare needs **bidirectional MQTT**:
1. Standardized hierarchical topics (`crycare/<device_id>/<channel>`) so the Broker routes messages cleanly across multiple devices.
2. A **Last Will and Testament (LWT)** configuration so the Broker automatically publishes an `"offline"` status if the device loses power unexpectedly.
3. Downlink command reception: verifying that an incoming MQTT message is addressed to this device's command topic and parsing the JSON command payload into a typed C struct.

## 3. Header Overview & Function Contracts

Create `c/CMakeLists.txt` to build executable `crycare_router` into `bin/` (`c/build/bin/crycare_router.exe` on Windows), then implement all functions declared in `c/include/mqtt_router.h` inside `c/src/mqtt_router.c`.

### 3.1 `c/include/mqtt_router.h` (Types & Constants)
- Constants: `MAX_TOPIC_LEN` (`64`), `MAX_PAYLOAD_LEN` (`64`), `MAX_DEVICE_ID_LEN` (`32`).
- **`mqtt_cmd_type_t` (`enum`):**
  - `CMD_UNKNOWN` (`0`)
  - `CMD_ACK_MOM` (`1`)
  - `CMD_SET_THRESHOLD` (`2`)
  - `CMD_PING_REQ` (`3`)
- **`mqtt_command_t` (`struct`):** Parsed downlink command:
  - `mqtt_cmd_type_t cmd_type`
  - `uint32_t seq_id`
  - `int32_t value` (threshold in dBFS for `CMD_SET_THRESHOLD`, `0` otherwise)
  - `bool valid` (`true` if parsed successfully, `false` otherwise)
- **`mqtt_lwt_config_t` (`struct`):** Last Will & Testament configuration registered at MQTT connect:
  - `char topic[MAX_TOPIC_LEN]`
  - `char payload[MAX_PAYLOAD_LEN]`
  - `uint8_t qos`
  - `bool retain`

### 3.2 Functions to Implement in `c/src/mqtt_router.c`
- `bool mqtt_build_topic(const char *device_id, const char *channel, char *out_buf, size_t buf_size)`
  - **Input:** `device_id`, `channel`, destination buffer `out_buf`, and capacity `buf_size`.
  - **Output:** Writes `crycare/<device_id>/<channel>` into `out_buf` and returns `true`. Returns `false` if any pointer is `NULL`, `device_id` or `channel` is empty or contains `'/'` or `' '`, `buf_size == 0`, or the formatted topic is truncated.
- `bool mqtt_build_lwt(const char *device_id, mqtt_lwt_config_t *lwt)`
  - **Input:** `device_id` and pointer to `mqtt_lwt_config_t`.
  - **Output:** Populates `lwt->topic` as `crycare/<device_id>/status`, `lwt->payload` as `{"status":"offline"}`, `lwt->qos = 1`, `lwt->retain = true`, and returns `true`. Returns `false` if inputs are invalid or topic construction fails.
- `bool mqtt_topic_matches_cmd(const char *topic, const char *expected_device_id)`
  - **Input:** Incoming MQTT `topic` and this device's `expected_device_id`.
  - **Output:** `true` iff `topic` exactly matches `crycare/<expected_device_id>/cmd`; `false` otherwise.
- `bool mqtt_parse_command_json(const char *json_str, mqtt_command_t *out_cmd)`
  - **Input:** Null-terminated JSON string `json_str` and pointer `out_cmd`.
  - **Output:** Parses `"cmd"` (`"ACK_MOM"`, `"SET_THRESHOLD"`, or `"PING_REQ"`), `"seq_id"` (unsigned 32-bit integer), and `"value"` (signed integer).
    - `"cmd"` and `"seq_id"` are always required.
    - For `CMD_SET_THRESHOLD`, `"value"` is required and must lie in `[-120, 0]`.
    - For `CMD_ACK_MOM` and `CMD_PING_REQ`, `"value"` is optional and defaults to `0`.
    - Sets `out_cmd->valid = true` and returns `true` on success; sets `out_cmd->valid = false` and returns `false` on any missing/invalid field or malformed input.

## 4. Input / dataset

STDIN starts with `<expected_device_id> <Q>`, followed by `Q` lines:
- `BUILD_TOPIC <device_id> <channel> <buf_size>`
- `BUILD_LWT <device_id>`
- `ROUTE <topic> <json_payload_on_same_line>`

[Data and schema](../../data/2026-11-19/README.md): five synthetic public input/output pairs.

## 5. Output / submission

One line per command:
- `BUILD_TOPIC`: `TOPIC <topic_str>` or `TOPIC_ERR`
- `BUILD_LWT`: `LWT <topic> qos=<qos> retain=<0|1> <payload>` or `LWT_ERR`
- `ROUTE`:
  - If topic does not match `crycare/<expected_device_id>/cmd`: `IGNORE_TOPIC`
  - Else if JSON fails validation: `BAD_CMD`
  - Else: `CMD <CMD_NAME> seq=<seq_id> val=<value>` (where `<CMD_NAME>` is `ACK_MOM`, `SET_THRESHOLD`, or `PING_REQ`)

Required handoff: C11 project and evidence, per [SUBMISSION.md](SUBMISSION.md).

## 6. Constraints & resources

- `0 <= Q <= 5000`.
- No dynamic memory allocation inside `mqtt_router.c`.
- Single-threaded host exercise: no live Mosquitto broker or ESP-IDF required.

## 7. Samples

Input (`01_sample.in`):

~~~text
s3_01 4
BUILD_TOPIC s3_01 telemetry 64
BUILD_LWT s3_01
ROUTE crycare/s3_01/cmd {"cmd":"ACK_MOM","seq_id":42}
ROUTE crycare/s3_02/cmd {"cmd":"ACK_MOM","seq_id":43}
~~~

Expected output (`01_sample.out`):

~~~text
TOPIC crycare/s3_01/telemetry
LWT crycare/s3_01/status qos=1 retain=1 {"status":"offline"}
CMD ACK_MOM seq=42 val=0
IGNORE_TOPIC
~~~

## 8. Verification / evaluation

Follow [README.md](README.md) to build and run the checker across 5 public fixtures (`01_sample` through `05_truncation_and_lwt`). Add two learner tests in `REPORT.md`.

## 9. Your workflow — before coding

Document your design in `REPORT.md` before coding.

## 10. Hints & API references — on request

Ask about C11 string/number parsing APIs (`strstr`, `strtol`, `strtoul`, `strcmp`, `snprintf`).

## 11. Deliverables & short review

Submit `c/CMakeLists.txt`, `c/src/mqtt_router.c`, `c/include/mqtt_router.h`, supplied `c/src/main.c`, two custom tests in `REPORT.md`, and checker output.

## 12. Follow-up variation

Optional review: test a `SET_THRESHOLD` JSON where `"value"` is missing or equal to `5` (out of `[-120, 0]`) and confirm `BAD_CMD` is emitted.
