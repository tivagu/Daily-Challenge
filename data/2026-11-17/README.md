# Data & Schema — EDGE-2026-11-17

Five synthetic public replay fixtures in `public/`:

1. `01_sample.{in,out}`: Enqueue while offline (`BROKER_DOWN` default), `PUBLISH_NEXT` returns `NONE`, then `BROKER_UP`, `PUBLISH_NEXT`, `PUBACK`, and `SIZE`.
2. `02_disconnect_retry.{in,out}`: Publish succeeds, `BROKER_DOWN` occurs before `PUBACK`, `BROKER_UP` retries the exact same oldest event (`seq_id = 10`).
3. `03_outbox_full_and_ack_mismatch.{in,out}`: Capacity rejection (`FULL`), second `PUBLISH_NEXT` blocked while in-flight (`NONE`), mismatched `PUBACK 999` (`ACK_ERR`), then valid `PUBACK 1` (`ACK_OK`).
4. `04_wraparound_drain.{in,out}`: Circular FIFO wraparound across fill/ack/refill/drain cycles.
5. `05_truncation_and_invalid.{in,out}`: `snprintf` boundary (`buf_size == len` -> `TRUNCATED`, `buf_size == len + 1` -> `LEN 95`), unknown `event_type`, and `PUBACK` with nothing in-flight (`ACK_ERR`).
