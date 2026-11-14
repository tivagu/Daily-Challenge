# Data & Schema — EDGE-2026-11-13

Five synthetic public replay fixtures in `public/`:

1. `01_sample.{in,out}`: Basic `INGEST`, `SIZE`, `POP_PACK`, and `VERIFY` flow.
2. `02_zero_silence.{in,out}`: Zero-amplitude I2S samples (`log10(0)` guard -> `-120` dBFS) and empty queue `POP_PACK`.
3. `03_int64_no_overflow.{in,out}`: Full-scale `-2147483648` (`int16_t = -32768`) samples that would overflow `int32_t` if squared without `int64_t`.
4. `04_queue_full_wraparound.{in,out}`: Bounded queue capacity rejection (`FULL`) and circular wraparound across multiple fill/drain cycles.
5. `05_packet_verify.{in,out}`: Valid packet, corrupted checksum byte, and wrong magic byte (`INVALID`).
