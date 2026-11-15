# EDGE-2026-11-15 — CryCare C11 Pitch Estimator & 5-State Alarm FSM

## 1. Challenge card

- Weekly block (09–15 November): CryCare DSP pitch estimation, packet replay guard, debounce FSM & alarm escalation.
- Module: single-threaded C11 host replay of Zero-Crossing Rate ($F_0$) estimation, 16-byte `crycare_packet_t` verification with `seq_id` duplicate rejection, and the 5-state `c3_alarm` Finite State Machine.
- Format A: STDIN–STDOUT with a C11 project handoff. Self-authored practice; no `CMakeLists.txt` is supplied (write it yourself).
- Session: **240 minutes (4 hours)** including CMake setup, workflow design, coding, tests, packaging, and same-day debrief.
- Tools: C11 standard library (`<stdint.h>`, `<stdbool.h>`, `<math.h>`), CMake, Ninja, and a host C compiler. Python 3.10+ runs the checker and packager. No ESP-IDF or hardware is required.

## 2. Scenario & objective

In `EDGE-2026-11-13`, you built the producer side: normalizing I2S samples, computing dBFS, queueing active chunks, and packing a 16-byte `crycare_packet_t` with an XOR checksum.

Today you will design the workflow and implementation for the next stage between **ESP32-S3 (`s3_listener`)** and **ESP32-C3 (`c3_alarm`)**:
1. **Pitch Estimation ($F_0$):** Estimate the fundamental frequency (in Hz) of a PCM audio chunk from its Zero-Crossing Rate (ZCR).
2. **Receiver State Machine (`alarm_fsm_t`):** While `crycare_packet_t` represents a single 16-byte wireless message at one instant, `alarm_fsm_t` represents the **persistent memory and state of the ESP32-C3 alarm receiver** across time (tracking configuration, current alarm state, buzzer duty cycle, consecutive cry streak, last accepted sequence ID, and timer timestamps).

## 3. Header Overview & Function Contracts

Create `c/CMakeLists.txt` to build executable `crycare_fsm` into `bin/` (`c/build/bin/crycare_fsm.exe` on Windows), then implement the functions in `c/src/alarm_fsm.c`. Design your own control flow and conditions from the requirements below.

### 3.1 `c/include/crycare_protocol.h` (Wireless Packet Contract)
- **`crycare_event_t` (`enum`):** Event codes carried by a packet (`CRYCARE_EVENT_CRY_ALERT = 1`, `CRYCARE_EVENT_ACK_MOM = 2`, `CRYCARE_EVENT_PING = 3`).
- **`crycare_packet_t` (`struct`, 16 bytes packed):** One wireless frame transmitted from S3 to C3.
- **Functions to implement:**
  - `uint8_t crycare_compute_xor_checksum(const crycare_packet_t *pkt)`
    - **Input:** Pointer to a 16-byte packet.
    - **Output:** 8-bit XOR checksum of the first 15 bytes (or `0` if pointer is `NULL`).
  - `crycare_packet_t crycare_pack_packet(uint8_t event_type, uint8_t conf, int8_t dbfs, uint16_t f0_hz, uint8_t duty_cycle, uint32_t seq_id)`
    - **Input:** Payload fields for a new packet.
    - **Output:** A 16-byte `crycare_packet_t` with `magic` set to `CRYCARE_MAGIC_BYTE` (`0xCC`), `reserved` bytes zeroed, payload populated, and `checksum` computed.
  - `bool crycare_verify_packet(const crycare_packet_t *pkt)`
    - **Input:** Pointer to a received 16-byte packet.
    - **Output:** `true` if the packet pointer is valid, has the expected magic byte, and matches its XOR checksum; `false` otherwise.

### 3.2 `c/include/alarm_fsm.h` (Pitch Estimator & C3 Receiver State Machine)
- **`fsm_state_t` (`enum`):** The 5 operating states of the C3 alarm receiver:
  - `FSM_STATE_IDLE` (`0`): Normal quiet monitoring (`duty_cycle = 0`).
  - `FSM_STATE_PRE_ALERT` (`1`): At least one qualified cry packet seen, but fewer than `debounce_k` in a row (`duty_cycle = 0`).
  - `FSM_STATE_ALERT_SOFT` (`2`): Confirmed cry alarm ringing at soft level (`duty_cycle = 50`).
  - `FSM_STATE_ALERT_ESCALATED` (`3`): Unacknowledged alarm escalated to maximum level (`duty_cycle = 100`).
  - `FSM_STATE_COOLDOWN` (`4`): Silenced after Mom's ACK for `cooldown_sec` seconds (`duty_cycle = 0`).
- **`fsm_step_status_t` (`enum`):** Return code of processing a packet (`FSM_STEP_OK`, `FSM_STEP_INVALID`, `FSM_STEP_DUPLICATE`).
- **`alarm_fsm_t` (`struct`):** Persistent receiver context holding:
  - Configuration parameters set at initialization: `debounce_k` (required consecutive cry frames), `escalate_after_sec` (seconds before soft alert escalates to 100%), `cooldown_sec` (duration of cooldown after ACK).
  - Runtime state tracked across packets: `state`, `duty_cycle`, `consecutive_hits`, `last_seq_id`, `has_seen_packet`, `alert_start_sec`, `cooldown_until_sec`.
- **Functions to implement:**
  - `uint16_t estimate_f0_hz(const int16_t *samples, size_t num_samples, uint32_t sample_rate_hz)`
    - **Input:** Array of signed 16-bit PCM samples, sample count $N$, and sample rate $f_s$ (Hz).
    - **Output:** Estimated fundamental frequency in Hz clamped to `[0, 65535]` (or `0` if inputs cannot form at least one interval).
    - **Domain formula:** Count zero-crossings $Z$ across the $N - 1$ adjacent sample pairs (a crossing occurs when one sample is strictly negative `< 0` and its neighbor is non-negative `>= 0`). Then:
      $$f_0 = \text{lround}\left(\frac{Z \times f_s}{2.0 \times (N - 1)}\right)$$
  - `bool is_cry_qualified(int8_t dbfs, uint8_t conf, uint16_t f0_hz)`
    - **Input:** Chunk energy `dbfs`, AI confidence `conf`, and pitch `f0_hz`.
    - **Output:** `true` only when energy is at least `-35` dBFS, confidence is at least `70`, and pitch lies within `[300, 600]` Hz; `false` otherwise.
  - `bool alarm_fsm_init(alarm_fsm_t *fsm, uint8_t debounce_k, uint32_t escalate_after_sec, uint32_t cooldown_sec)`
    - **Input:** Pointer to `alarm_fsm_t` and the 3 configuration parameters (`debounce_k` must be at least `1`).
    - **Output:** `true` on valid initialization (storing configuration and resetting all runtime state to initial idle defaults); `false` if invalid.
  - `fsm_step_status_t alarm_fsm_process_packet(alarm_fsm_t *fsm, const crycare_packet_t *pkt, uint32_t now_sec)`
    - **Input:** Pointer to `alarm_fsm_t`, pointer to incoming `crycare_packet_t`, and current timestamp `now_sec`.
    - **Output & Behavioral Requirements:**
      1. **Integrity & event validation:** Reject corrupted packets or unknown event types with `FSM_STEP_INVALID` without altering FSM state.
      2. **Replay protection:** After the first valid packet has been accepted, reject any packet whose `seq_id` is not strictly greater than `last_seq_id` with `FSM_STEP_DUPLICATE` without altering FSM state.
      3. **Cooldown expiry:** If the receiver is in `COOLDOWN` and `now_sec` has reached or passed `cooldown_until_sec`, return the receiver to `IDLE` before handling the new event.
      4. **Event rules:**
         - `ACK_MOM`: Silences any active/pending state (`duty_cycle = 0`, streak reset to `0`) and places the receiver into `COOLDOWN` for `cooldown_sec` seconds from `now_sec`.
         - `CRY_ALERT`:
           - Ignored while in `COOLDOWN`.
           - While in `IDLE` or `PRE_ALERT`: a qualified cry advances the consecutive streak (entering `PRE_ALERT` at `duty_cycle = 0`, or transitioning to `ALERT_SOFT` at `duty_cycle = 50` and recording `alert_start_sec` once the streak reaches `debounce_k`; if `escalate_after_sec == 0`, it escalates immediately to `ALERT_ESCALATED` at `duty_cycle = 100`). An unqualified packet breaks the streak and resets the receiver to `IDLE`.
           - While already ringing (`ALERT_SOFT` or `ALERT_ESCALATED`): the alarm **latches** (does not turn off on unqualified packets). If in `ALERT_SOFT` and elapsed time since `alert_start_sec` has reached `escalate_after_sec`, transition to `ALERT_ESCALATED` (`duty_cycle = 100`).
         - `PING`: Heartbeat packet; if currently in `ALERT_SOFT` and elapsed time since `alert_start_sec` has reached `escalate_after_sec`, transition to `ALERT_ESCALATED` (`duty_cycle = 100`).

## 4. Input / dataset

STDIN starts with `K E C Q` (`debounce_k`, `escalate_after_sec`, `cooldown_sec`, and command count), followed by `Q` commands:
- `F0 <sample_rate_hz> <N> <s_0> <s_1> ... <s_{N-1}>`
- `SEND_FRAME <now_sec> <seq_id> <event_type> <conf> <dbfs> <f0_hz>`
- `RAW_PKT <now_sec> <16 hex bytes>`
- `STATUS`

[Data and schema](../../data/2026-11-15/README.md): six synthetic public input/output pairs.

## 5. Output / submission

One line per command:
- `F0`: `F0 <hz>`
- `SEND_FRAME` / `RAW_PKT`: `INVALID`, `DUPLICATE`, or `<STATE_NAME> duty=<duty_cycle> hits=<consecutive_hits> ck=<checksum_2_hex>`
- `STATUS`: `<STATE_NAME> duty=<duty_cycle> hits=<consecutive_hits> seq=<last_seq_id>`

Required handoff: C11 project and evidence, per [SUBMISSION.md](SUBMISSION.md).

## 6. Constraints & resources

- `1 <= K <= 10`, `0 <= Q <= 10000`.
- `sizeof(crycare_packet_t) == 16` enforced at compile time via `_Static_assert`.
- No dynamic memory allocation inside `alarm_fsm.c`. Every FSM transition must run in $O(1)$ time and $O(1)$ space.

## 7. Samples

Input (`01_sample.in`):

~~~text
2 120 30 7
F0 16000 9 1000 -1000 1000 -1000 1000 -1000 1000 -1000 1000
SEND_FRAME 10 1 1 85 -20 450
SEND_FRAME 12 2 1 90 -18 460
SEND_FRAME 50 3 1 40 -50 120
SEND_FRAME 135 4 3 0 -120 0
SEND_FRAME 140 5 2 0 -120 0
SEND_FRAME 175 6 3 0 -120 0
~~~

Expected output (`01_sample.out`):

~~~text
F0 8000
PRE_ALERT duty=0 hits=1 ck=B6
ALERT_SOFT duty=50 hits=2 ck=B6
ALERT_SOFT duty=50 hits=2 ck=62
ALERT_ESCALATED duty=100 hits=2 ck=71
COOLDOWN duty=0 hits=0 ck=27
IDLE duty=0 hits=0 ck=41
~~~

## 8. Verification / evaluation

Follow [README.md](README.md) to build and run the checker across the 6 public fixtures (`01_sample` through `06_instant_escalation`). Add two learner tests in `REPORT.md`.

## 9. Your workflow — before coding

Document your workflow and state transition design in `REPORT.md` before coding.

## 10. Hints & API references — on request

Ask about any C11 syntax detail, compiler message, or domain concept. No solved steps or condition code will be given.

## 11. Deliverables & short review

Submit `c/CMakeLists.txt`, `c/src/alarm_fsm.c`, headers, supplied `c/src/main.c`, two custom tests in `REPORT.md`, checker output, and `submission/EDGE-2026-11-15-c.zip`.

## 12. Follow-up variation

Optional review: test what happens when a duplicate `ACK_MOM` arrives after a new `ALERT_SOFT` has started.
