# Data — EDGE-2026-11-06

Synthetic integer sensor features and hand-authored linear model coefficients, created for this exercise. No personal data, external dataset, trained model or measured device performance. You may store these small authored fixtures with your learning repository.

- `features.csv`: sample_id plus mean_abs_accel, peak_count, gyro_energy (synthetic units, integer 0-10).
- `model.json`: ordered feature_names/classes, integer weights K by F and bias length K. Scores may be negative; they are not probabilities.
- `sample_submission.csv`: output schema only; PLACEHOLDER labels must be replaced by your predictions.
- `public/*.features.csv`, `*.model.json`, `*.expected.csv`: five public cases. Expected labels test the defined calculation, not real classifier accuracy.
- No train/val/test split, hidden labels or private judge. Feature extraction is already done.

Full schema and tie rule: [problem.md](../../ISSUED/2026-11-06/problem.md). Run instructions: [README](../../ISSUED/2026-11-06/README.md). Keep supplied fixtures unchanged; write outputs in your project outputs/ directory.
