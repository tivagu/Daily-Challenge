# EDGE-2026-11-17 — CryCare C11 JSON Telemetry & MQTT Offline Outbox

## 1. Challenge card

- Weekly block (16–22 November): CryCare Cloud Telemetry (JSON + MQTT Outbox).
- Module: single-threaded C11 host replay of safe `snprintf` JSON serialization, bounded offline outbox FIFO, and MQTT `PUBACK` / disconnect retry handling.
- Format A: STDIN–STDOUT with a C11 project handoff. Self-authored practice; no `CMakeLists.txt` is supplied (write it yourself).
- Session: **90 minutes (1.5 hours)** including CMake setup, workflow design, coding, tests, and same-day debrief.
- Tools: C11 standard library (`<stdio.h>`, `<stdint.h>`, `<stdbool.h>`, `<stddef.h>`), CMake, Ninja, and a host C compiler. Python 3.10+ runs the checker and packager. No ESP-IDF, Wi-Fi, or live MQTT broker is required.

## 2. Scenario & objective

In CryCare (`s3_listener`), after the AI/DSP pipeline confirms an event, ESP32-S3 must publish a JSON payload over MQTT to the backend broker so the server can store it in the database and notify parents.

However, Wi-Fi or the MQTT broker can temporarily disconnect. If S3 drops events when offline, or deletes an event before receiving `PUBACK`, alerts are lost. Your goal tonight is to build the C11 JSON formatter and bounded offline outbox that guarantees events are formatted safely and retained until acknowledged.

## 3. Header Overview & Function Contracts

Create `c/CMakeLists.txt` to build executable `crycare_mqtt` into `bin/` (`c/build/bin/crycare_mqtt.exe` on Windows), then implement all functions declared in `c/include/mqtt_outbox.h` inside `c/src/mqtt_outbox.c`.

### 3.1 `c/include/mqtt_outbox.h` (Types & Constants)
- Constants: `OUTBOX_MAX_CAPACITY` (`8`), `JSON_BUF_SIZE` (`256`), `DEVICE_ID_MAX_LEN` (`32`).
- **`telemetry_event_t` (`enum`):**
  - `TELEMETRY_CRY_ALERT` (`1`)
  - `TELEMETRY_ACK_MOM` (`2`)
  - `TELEMETRY_PING` (`3`)
- **`telemetry_event_data_t` (`struct`):** One event record queued for MQTT publishing:
  - `char device_id[DEVICE_ID_MAX_LEN]`
  - `uint8_t event_type`
  - `uint32_t seq_id`
  - `uint32_t ts_sec`
  - `uint8_t conf`
  - `int8_t dbfs`
  - `uint16_t f0_hz`
- **`mqtt_outbox_t` (`struct`):** Persistent outbox queue and MQTT publish state:
  - `telemetry_event_data_t items[OUTBOX_MAX_CAPACITY]` (fixed ring buffer storage)
  - `size_t capacity`, `size_t head`, `size_t tail`, `size_t count` (FIFO state)
  - `bool broker_connected` (current MQTT broker connection state)
  - `uint32_t in_flight_seq` (sequence ID of the currently unacknowledged published message)
  - `bool has_in_flight` (`true` while waiting for `PUBACK` on `in_flight_seq`)

### 3.2 Functions to Implement in `c/src/mqtt_outbox.c`
- `const char *telemetry_event_name(uint8_t event_type)`
  - **Input:** Numeric event code.
  - **Output:** `"CRY_ALERT"` for `1`, `"ACK_MOM"` for `2`, `"PING"` for `3`, or `NULL` for any other code.
- `int format_telemetry_json(const telemetry_event_data_t *ev, char *out_buf, size_t buf_size)`
  - **Input:** Pointer to `telemetry_event_data_t`, destination buffer `out_buf`, and its capacity `buf_size`.
  - **Output:** Writes a single-line JSON string with exact key order and no extra spaces:
    `{"device_id":"<id>","event":"<name>","seq_id":<u32>,"ts":<u32>,"conf":<u8>,"dbfs":<i8>,"f0_hz":<u16>}`
    Returns the character count written (excluding `'\0'`). Returns `-1` on invalid pointers, `buf_size == 0`, unknown `event_type`, or buffer truncation.
- `bool outbox_init(mqtt_outbox_t *box, size_t capacity)`
  - **Input:** Pointer to `mqtt_outbox_t` and logical capacity (`1 <= capacity <= OUTBOX_MAX_CAPACITY`).
  - **Output:** `true` on valid initialization (all FIFO, connection, and in-flight state reset to `0`/`false`); `false` otherwise.
- `void outbox_set_broker_status(mqtt_outbox_t *box, bool connected)`
  - **Input:** Pointer to `mqtt_outbox_t` and connection flag.
  - **Output:** Updates `broker_connected`. When `connected` becomes `false`, any unacknowledged in-flight state is cleared so the oldest item will be retried upon reconnect.
- `bool outbox_enqueue(mqtt_outbox_t *box, const telemetry_event_data_t *ev)`
  - **Input:** Pointer to `mqtt_outbox_t` and pointer to `telemetry_event_data_t`.
  - **Output:** Appends event to the FIFO in $O(1)$ time and returns `true`; returns `false` if inputs are invalid or queue is full.
- `bool outbox_next_publish(mqtt_outbox_t *box, char *json_buf, size_t buf_size)`
  - **Input:** Pointer to `mqtt_outbox_t`, destination JSON buffer, and buffer size.
  - **Output:** Formats the oldest queued event into `json_buf` **without removing it from the queue**, marks it in-flight, and returns `true` only when the broker is connected, no other message is in-flight, the queue is non-empty, and JSON formatting succeeds. Returns `false` otherwise.
- `bool outbox_ack(mqtt_outbox_t *box, uint32_t seq_id)`
  - **Input:** Pointer to `mqtt_outbox_t` and acknowledged `seq_id`.
  - **Output:** If a message is currently in-flight and its `in_flight_seq` matches `seq_id`, removes the oldest item from the queue, clears in-flight state, and returns `true`. Otherwise returns `false`.
- `size_t outbox_size(const mqtt_outbox_t *box)`
  - **Input:** Pointer to `mqtt_outbox_t`.
  - **Output:** Current number of queued items (or `0` if `NULL`).

## 4. Input / dataset

STDIN starts with `C Q` (`capacity` and command count), followed by `Q` commands:
- `BROKER_UP` / `BROKER_DOWN`
- `ENQUEUE <device_id> <event_type> <seq_id> <ts_sec> <conf> <dbfs> <f0_hz>`
- `PUBLISH_NEXT`
- `PUBACK <seq_id>`
- `SIZE`
- `FORMAT_SMALL <buf_size> <device_id> <event_type> <seq_id> <ts_sec> <conf> <dbfs> <f0_hz>`

[Data and schema](../../data/2026-11-17/README.md): five synthetic public input/output pairs.

## 5. Output / submission

One line per command:
- `BROKER_UP` / `BROKER_DOWN`: no output line.
- `ENQUEUE`: `OK` or `FULL`.
- `PUBLISH_NEXT`: `PUB <json_string>` or `NONE`.
- `PUBACK`: `ACK_OK` or `ACK_ERR`.
- `SIZE`: current queue count.
- `FORMAT_SMALL`: `LEN <n> <json_string>` or `TRUNCATED`.

Required handoff: C11 project and evidence, per [SUBMISSION.md](SUBMISSION.md).

## 6. Constraints & resources

- `1 <= C <= 8`, `0 <= Q <= 5000`.
- No dynamic memory allocation inside `mqtt_outbox.c`. All queue operations must run in $O(1)$ time.
- Single-threaded host exercise: no live Wi-Fi, Mosquitto broker, or ESP-IDF SDK required.

## 7. Samples

Input (`01_sample.in`):

~~~text
3 7
ENQUEUE s3_01 1 101 1000 92 -18 440
SIZE
PUBLISH_NEXT
BROKER_UP
PUBLISH_NEXT
PUBACK 101
SIZE
~~~

Expected output (`01_sample.out`):

~~~text
OK
1
NONE
PUB {"device_id":"s3_01","event":"CRY_ALERT","seq_id":101,"ts":1000,"conf":92,"dbfs":-18,"f0_hz":440}
ACK_OK
0
~~~

## 8. Verification / evaluation

Follow [README.md](README.md) to build and run the checker across the 5 public fixtures (`01_sample` through `05_truncation_and_invalid`). Add two learner tests in `REPORT.md`.

## 9. Your workflow — before coding

Write your workflow and edge-case plan in `REPORT.md` before coding.

## 10. Hints & API references — on request

Ask about `snprintf` return values, format specifiers (`<inttypes.h>`), or MQTT `PUBACK` concepts. No solved conditions will be given.

## 11. Deliverables & short review

Submit `c/CMakeLists.txt`, `c/src/mqtt_outbox.c`, headers, supplied `c/src/main.c`, two custom tests in `REPORT.md`, and checker output. Suggested 90-minute split: 10 min CMake, 10 min workflow, 45 min coding, 15 min testing, 10 min debrief.

## 12. Follow-up variation

Optional review: trace what happens when two events are queued, the first is published, `BROKER_DOWN` occurs before `PUBACK`, and then `BROKER_UP` + `PUBLISH_NEXT` runs again.
