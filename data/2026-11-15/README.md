# Data & Schema — EDGE-2026-11-15

Six synthetic public replay fixtures in `public/`:

1. `01_sample.{in,out}`: Full lifecycle (`F0`, `PRE_ALERT` debounce, `ALERT_SOFT` latch, 120s `ALERT_ESCALATED`, `ACK_MOM` `COOLDOWN`, and cooldown expiry to `IDLE`).
2. `02_f0_edge_cases.{in,out}`: Zero-Crossing Rate edge cases (all zeros, `N < 2`, samples touching `0`, and 4 kHz square wave).
3. `03_debounce_reset_on_noise.{in,out}`: Unqualified noise packet resets `PRE_ALERT` streak (`hits` back to `0`, state back to `IDLE`).
4. `04_replay_and_corrupt.{in,out}`: Rejects duplicate `seq_id`, older `seq_id` (`DUPLICATE`), corrupted checksum, and invalid magic byte (`INVALID`) without altering FSM state.
5. `05_cooldown_ignores_cry.{in,out}`: Cry alert during `COOLDOWN` is ignored (`duty=0`); once `now_sec >= cooldown_until_sec`, new cry alerts start a fresh debounce cycle.
6. `06_instant_escalation.{in,out}`: Boundary thresholds (`dbfs=-35`, `conf=70`, `f0=300/600`) with `K=1, E=0` immediately entering `ALERT_ESCALATED duty=100`.
