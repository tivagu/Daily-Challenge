# EDGE-2026-11-13 — CryCare C11 Audio Pipeline & 16-Byte Protocol

## 1. Challenge card

- Weekly block (09–15 November): CryCare audio ingestion, energy gate, bounded queue & binary protocol.
- Module: single-threaded C11 host replay of ESP32-S3 I2S 32-bit $\to$ `int16_t` normalization, `int64_t` RMS $\to$ dBFS energy gate, bounded FIFO chunk queue, and 16-byte `crycare_packet_t` with XOR checksum.
- Format A: STDIN–STDOUT with a small C11 project handoff. Self-authored practice; no `CMakeLists.txt` is supplied (create it yourself).
- Session: **240 minutes (4 hours)** including CMake setup, workflow, coding, tests, packaging, and same-day debrief.
- Tools: C11 standard library (`<stdint.h>`, `<stdbool.h>`, `<math.h>`), CMake, Ninja (or installed generator), and a host C compiler. Python 3.10+ runs the checker and packager. No ESP-IDF, hardware, external data, or network is required.

## 2. Scenario & objective

In CryCare (`s3_listener`), Core 0 reads 32-bit raw I2S DMA words from the INMP441 microphone, normalizes each sample to signed 16-bit PCM, computes chunk energy in dBFS, and pushes active chunks into a bounded FreeRTOS-style queue for downstream AI/ESP-NOW processing. When an event is ready, a fixed **16-byte binary packet** (`crycare_packet_t`) is packed with an XOR checksum and transmitted to `c3_alarm`.

The supplied host driver (`c/src/main.c`) stands in for both the I2S producer and the packet consumer on one thread. Your job is to write `c/CMakeLists.txt` and implement all pipeline and protocol functions in C11.

## 3. Task

First create `c/CMakeLists.txt` from scratch to build executable `crycare_pipeline` into `bin/` (so `c/build/bin/crycare_pipeline.exe` on Windows; link `m` on Unix if needed). Then implement the functions declared in `c/include/crycare_protocol.h` and `c/include/audio_pipeline.h` inside `c/src/audio_pipeline.c`. Keep all public signatures and struct layouts intact.

- `normalize_i2s_sample(raw_sample)`: convert a 32-bit signed I2S word to `int16_t` via arithmetic right-shift by 16 (`(int16_t)(raw_sample >> 16)`).
- `compute_dbfs(samples, num_samples)`:
  - Accumulate sum of squares in **`int64_t`** (`int64_t s = samples[i]; sum_sq += s * s;`) so squaring `-32768` never overflows `int32_t`.
  - If `samples == NULL` or `num_samples == 0` or `sum_sq == 0`, return `-120` (avoid `log10(0)`).
  - Otherwise compute `rms = sqrt((double)sum_sq / (double)num_samples)` and `dbfs = lround(20.0 * log10(rms / 32768.0))`, clamped to `[-120, 0]`, returned as `int8_t`.
- `chunk_queue_init(q, capacity)`, `chunk_queue_push(q, chunk)`, `chunk_queue_pop(q, out)`, `chunk_queue_size(q)`:
  - Bounded FIFO of logical capacity `C` (`1 <= C <= 16`) using fixed storage `items[16]` (no `malloc` inside queue functions, worst-case $O(1)$ per operation).
  - `push` returns `false` without overwriting existing items when full. `pop` returns `false` when empty.
- `crycare_compute_xor_checksum(pkt)`: XOR the first 15 bytes (`0..14`) of `crycare_packet_t`.
- `crycare_pack_packet(...)`: fill `magic = 0xCC`, `reserved = {0, 0, 0, 0}`, all payload fields, and `checksum`.
- `crycare_verify_packet(pkt)`: return `true` iff `pkt != NULL`, `pkt->magic == 0xCC`, and `pkt->checksum == crycare_compute_xor_checksum(pkt)`.

The kit supplies headers, driver (`main.c`), checker, packager, and report template. You supply `c/CMakeLists.txt`, `c/src/audio_pipeline.c`, two custom tests, and the debrief.

## 4. Input / dataset

STDIN starts with `C T Q` (logical queue capacity, energy gate threshold in dBFS, and command count), followed by `Q` whitespace-separated commands:
- `INGEST <seq_id> <N> <raw_0> ... <raw_{N-1}>` (`N` signed 32-bit integers)
- `POP_PACK <event_type> <conf> <f0_hz> <duty_cycle>`
- `VERIFY <16 hex bytes separated by spaces>`
- `SIZE`

[Data and schema](../../data/2026-11-13/README.md): five synthetic public input/output pairs.

## 5. Output / submission

The driver emits one line per command:
- `INGEST`: if `dbfs >= T`, pushes `{seq_id, dbfs}` and prints `ACTIVE <dbfs> OK` or `ACTIVE <dbfs> FULL`; otherwise prints `SILENT <dbfs>` without pushing.
- `POP_PACK`: prints `EMPTY` if the queue is empty; otherwise pops the oldest chunk, packs `crycare_packet_t`, and prints `PKT <seq_id> <dbfs> <checksum_2_hex_upper>`.
- `VERIFY`: prints `VALID <event_type> <seq_id> <dbfs>` if valid, else `INVALID`.
- `SIZE`: prints the current unread chunk count.

Required handoff: C11 project and evidence, per [SUBMISSION.md](SUBMISSION.md).

## 6. Constraints & resources

- `1 <= C <= 16`, `-120 <= T <= 0`, `0 <= Q <= 10000`.
- `sizeof(crycare_packet_t) == 16` enforced at compile time via `_Static_assert`.
- No dynamic memory allocation inside `chunk_queue_*`, `compute_dbfs`, or `crycare_*` functions (only the supplied `main.c` driver uses `malloc` to read variable-length `INGEST` test lines).
- Every queue operation must run in worst-case $O(1)$ time.
- Single-threaded host exercise: no ISR, FreeRTOS task, DMA, PSRAM, ESP-IDF build, or hardware microphone is tested.

## 7. Samples

Input (`01_sample.in`):

~~~text
3 -35 6
INGEST 1 4 1073741824 -1073741824 1073741824 -1073741824
INGEST 2 4 65536 -65536 65536 -65536
SIZE
POP_PACK 1 92 440 80
SIZE
VERIFY CC 01 5C FA B8 01 50 00 00 00 00 01 00 00 00 83
~~~

Expected output (`01_sample.out`):

~~~text
ACTIVE -6 OK
SILENT -90
1
PKT 1 -6 83
0
VALID 1 1 -6
~~~

Explanation:
1. `INGEST 1 ...`: `1073741824 >> 16 = 16384` (half of `32768`), so RMS $= 16384$ and $20 \log_{10}(0.5) \approx -6\text{ dBFS} \ge -35$. Active chunk pushed $\to$ `ACTIVE -6 OK`.
2. `INGEST 2 ...`: `65536 >> 16 = 1`, so $20 \log_{10}(1 / 32768) \approx -90\text{ dBFS} < -35$. Dropped by energy gate $\to$ `SILENT -90`.
3. `SIZE` $\to$ `1` (only `seq_id = 1` is in the queue).
4. `POP_PACK 1 92 440 80`: pops `{seq_id=1, dbfs=-6}`, packs 16-byte packet with `magic=0xCC`, `event_type=1`, `conf=92 (0x5C)`, `dbfs=-6 (0xFA)`, `f0_hz=440 (B8 01)`, `duty_cycle=80 (0x50)`, `reserved=00 00 00 00`, `seq_id=1 (01 00 00 00)`. XOR of bytes `0..14` is `0x83` $\to$ `PKT 1 -6 83`.
5. `SIZE` $\to$ `0`.
6. `VERIFY ...`: `magic == 0xCC` and XOR of bytes `0..14` matches `0x83` $\to$ `VALID 1 1 -6`.

## 8. Verification / evaluation

Follow [README.md](README.md) to build and run the checker. Five public cases cover:
1. `01_sample`: basic ingest, energy gate, pack, and verify flow.
2. `02_zero_silence`: zero samples (`log10(0)` guard $\to$ `-120`), empty `INGEST`, empty `POP_PACK`.
3. `03_int64_no_overflow`: full-scale `-2147483648` (`-32768` after shift) that overflows `int32_t` if not widened to `int64_t`.
4. `04_queue_full_wraparound`: `FULL` rejection without overwrite and circular wraparound.
5. `05_packet_verify`: valid packet, corrupted checksum, and wrong magic byte.

Add two tests of your own in `REPORT.md`. Passing replay tests verifies host behavior; it does not prove DMA or hardware timing.

## 9. Your workflow — before coding

Write 3–5 bullets in `REPORT.md` before coding: the 5-step hardware/pipeline workflow in your own words, the `chunk_queue_t` wraparound invariant, and the overflow/silence edge cases you will test.

## 10. Hints & API references — on request

Ask about one C11 syntax detail, `CMakeLists.txt` directive, bit-shift behavior, or compiler warning. No complete solution is supplied.

## 11. Deliverables & short review

Submit your `c/CMakeLists.txt`, implemented `c/src/audio_pipeline.c`, headers, supplied `c/src/main.c`, two learner tests, checker output, and `REPORT.md` (or partial work). Suggested 4-hour split: 25 min `CMakeLists.txt`, 25 min workflow in `REPORT.md`, 100 min coding C11, 60 min testing/debugging, 30 min packaging & debrief.

## 12. Follow-up variation

Optional review: test a corrupted payload byte inside `VERIFY` (with original checksum unchanged) and confirm `INVALID` is returned. Keep the public fixtures unchanged.
