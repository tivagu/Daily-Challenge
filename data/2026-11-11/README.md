# Data — EDGE-2026-11-11

Six synthetic PCM command traces authored for this task. They contain no audio recordings, personal data, trained model or device measurements. These fixtures may be stored with your learning repository.

Each public .in file begins with logical capacity C and command count Q, followed by Q commands (PUSH signed_int16, POP or SIZE). Each matching .out contains one result per command. The zero-command fixture has a zero-byte expected output. Signed PCM samples are decimal integers; traces have no sample-rate or timing assumptions.

| Case | Main check |
| :--- | :--- |
| 01_sample | FIFO and rejection followed by a successful push. |
| 02_zero_commands | No output. |
| 03_capacity_one | Full and empty at the smallest capacity. |
| 04_full_preserves_data | Repeated rejection preserves size and order. |
| 05_wraparound | Multiple fill/drain cycles preserve FIFO order. |
| 06_capacity_32 | Maximum capacity and signed sample endpoints. |

See the [problem](../../ISSUED/2026-11-11/problem.md) for semantics and the [kit README](../../ISSUED/2026-11-11/README.md) for the checker. Keep fixtures unchanged. No hidden cases or physical-device tests are implied.
