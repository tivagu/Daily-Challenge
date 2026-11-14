# EDGE-2026-11-11 — Static PCM Ring Buffer

## 1. Challenge card

- Weekly block (09–15 November): CryCare fixed-capacity audio buffering.
- Module: single-threaded FIFO for signed PCM samples, a host foundation for a later ESP32-S3 capture-to-DSP boundary.
- Format A: STDIN–STDOUT with a small C++ project handoff. Self-authored practice; no official competition rules are claimed.
- Session: **60 minutes** including same-day debrief. Foundation level; independent of prior submissions.
- Tools: C++17 standard library, CMake and an installed host compiler. Python 3.10+ runs the supplied checker and packager. Docs and targeted hints are allowed; write the buffer yourself. No external data, model, network, SDK or hardware is required.

## 2. Scenario & objective

Imagine that CryCare's listener receives decoded mono PCM samples. A downstream consumer needs the oldest unread sample first, but memory is bounded. Build the buffer between capture and DSP. The replay driver stands in for both producer and consumer on one thread, so today's evidence covers order and capacity.

## 3. Task

Implement AudioRingBuffer in cpp/include/audio_ring_buffer.hpp and cpp/src/audio_ring_buffer.cpp. Keep the public signatures; choose your own private state.

- The constructor receives a logical capacity C and starts empty.
- push(sample): if there is room, append the sample and return true. When full, return false and preserve all unread samples and the size.
- pop(sample): remove the oldest unread sample, assign it to the output reference and return true. If empty, return false and keep the buffer empty. The caller reads the reference only on success.
- size(): report the number of unread samples without modifying the buffer.
- Reuse storage across repeated fill and drain cycles. Do not shift all samples on pop.

The kit supplies command parsing, interface, build, checker and packager. You implement the buffer, add tests and write the debrief.

## 4. Input / dataset

STDIN starts with C Q (logical capacity and command count), then Q lines: PUSH x, POP, or SIZE. Fields are whitespace-separated. The driver handles valid inputs; malformed inputs are outside scope. x is a decimal signed 16-bit sample in [-32768, 32767]. There is no WAV header, timestamp or score.

[Data and schema](../../data/2026-11-11/README.md): six synthetic public input/output pairs.

## 5. Output / submission

The driver emits one line per command: PUSH prints OK or FULL; POP prints the oldest sample as a signed decimal or EMPTY; SIZE prints the current count. Zero commands emit no lines. Required handoff: C++ project and evidence, per [SUBMISSION.md](SUBMISSION.md).

## 6. Constraints & resources

- 1 <= C <= 32, 0 <= Q <= 100000.
- No more than 32 int16_t sample slots per buffer plus constant metadata. Honor the logical limit C even if backing storage is larger.
- No dynamic allocation in the buffer class, including construction. The supplied host driver's parsing and I/O are outside this rule. Every buffer operation must take worst-case O(1) time.
- This is a single-threaded host exercise. No ISR, scheduler, DMA, PSRAM, board build or microphone is tested. The 64-byte maximum sample payload follows from the slot count and PCM width, excluding metadata and padding; it is not a device measurement.
- Python is for tooling; no packages, Docker or venv are required. The checker timeout is a hang guard, not a timing benchmark.

## 7. Samples

Input:

~~~text
3 10
PUSH -120
PUSH 25
SIZE
PUSH 100
PUSH 200
POP
PUSH -7
POP
POP
POP
~~~

Expected output:

~~~text
OK
OK
2
OK
FULL
-120
OK
25
100
-7
~~~

The fourth push is rejected because three samples are unread. After one pop another push succeeds, and the older unread samples still leave first.

## 8. Verification / evaluation

Follow [README.md](README.md) to build and run the checker. It compares exact output lines while accepting platform line endings and an optional final newline. Six cases cover the sample, zero commands, capacity one, rejection without overwrite, repeated wraparound and signed endpoints at capacity 32. The starter builds and intentionally exits with STARTER_NOT_IMPLEMENTED.

Add two tests of your own and explain why the source meets the storage and per-operation bounds. Passing replay tests verifies visible FIFO behavior; it does not prove the bounds or hardware behavior. No private judge or leaderboard.

## 9. Your workflow — before coding

Send 3–5 bullets in chat: the state you plan to keep, an invariant, one edge case and how you will test it. Choose the representation yourself.

## 10. Hints & API references — on request

Ask about one C++ API, compiler message, FIFO invariant or test. No complete buffer implementation is supplied.

## 11. Deliverables & short review

Submit the implemented header/source, supplied driver and CMake config, two learner tests, checker results and the source bundle or partial work. Fill REPORT.md with exact commands, one decision and one limitation, time/space reasoning, help used, time spent and unfinished parts.

Suggested split: 5 min reading, 5 workflow, 30 coding/build, 10 testing/packaging and 10 debrief. Stop at 60 minutes; partial work can be reviewed later.

## 12. Follow-up variation

Optional review of this task: make a trace that rejects a push while full, frees a slot, and immediately pushes another sample. Predict the entire pop sequence. Keep the public cases unchanged.
