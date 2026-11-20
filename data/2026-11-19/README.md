# Data & Schema — EDGE-2026-11-19

Five synthetic public replay fixtures in `public/`:

1. `01_sample.{in,out}`: Basic `BUILD_TOPIC`, `BUILD_LWT`, matching `ROUTE` (`ACK_MOM`), and non-matching device topic (`IGNORE_TOPIC`).
2. `02_topic_validation.{in,out}`: Topic truncation (`buf_size` too small), slash `/` inside `device_id` or `channel` (`TOPIC_ERR`), and LWT with invalid `device_id` (`LWT_ERR`).
3. `03_topic_routing.{in,out}`: Exact topic matching (`crycare/<device_id>/cmd` vs `/telemetry`, extra suffix `/cmd/extra`, wrong prefix).
4. `04_command_parsing.{in,out}`: Valid `ACK_MOM`, `PING_REQ`, and `SET_THRESHOLD` (negative values like `-45`, boundary `-120`, `0`, and reordered JSON keys).
5. `05_invalid_commands.{in,out}`: Unknown `"cmd"`, missing `"seq_id"`, `SET_THRESHOLD` missing `"value"`, and out-of-range `"value"` (`-121` and `10`) returning `BAD_CMD`.
