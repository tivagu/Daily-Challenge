# EDGE-2026-11-06 — Portable Motion Inference Runner

## 1. Challenge card

- Topic: Python inference, model/data schema, deterministic predictions and submission artifacts.
- Edge AI role: replay precomputed sensor features through a tiny portable classifier and hand predictions to an evaluator.
- Format B: dataset/inference artifact; self-authored local contest-style contract.
- Origin: synthetic exercise, not an official Kaggle, Zalo AI or Olympic AI task. No external contest rules are claimed.
- Language/runtime: **Python 3.10+ standard library only**. PyTorch is not needed for this fixed small model.
- Budget: **90 minutes**, including workflow, implementation, tests, packaging and review; start when convenient.
- Difficulty: foundation checkpoint. Yesterday's exercise has no submitted/reviewed evidence, so no skill is assumed mastered. This task stands alone.
- Tools: documentation and targeted hints allowed; no full solution or generated complete implementation. No external data/models, network, GPU or hardware needed.

## 2. Scenario & objective

A virtual wearable has already converted each sensor window into three integer features. You receive a tiny exported linear classifier and must implement its inference entry point, preserving the data and artifact contracts.

The workflow practiced is **sensor features -> model forward calculation -> label -> ordered prediction file**. Feature extraction and model training are outside today's scope. The supplied coefficients are hand-authored for testing interoperability, not trained weights or evidence of real motion-recognition accuracy. Features have synthetic units.

## 3. Task

Implement `src/infer.py` with this command-line interface:

```text
python src/infer.py --features INPUT.csv --model MODEL.json --output OUTPUT.csv
```

For every input row:

1. Read its feature values by the names and order in `model.feature_names`; CSV columns may be reordered.
2. For class index `k`, compute the exact integer score `bias[k] + sum(weights[k][j] * feature[j])` across all features.
3. Select the class with the largest score. If scores tie, select the class that appears **first in model.classes**. Do not sort class names.
4. Write the selected label with the original `sample_id`, in the original input row order.

Load weights, biases, feature names and class order from the supplied model file; do not hardcode them or expected predictions. Scores are signed integers, not probabilities; do not apply a confidence threshold, softmax or training.

Support header-only input by producing header-only output. Create missing output parent directories. Success must exit with code 0; messages may go to STDERR. Supplied inputs satisfy the schema; handling malformed files is optional and not assessed today.

## 4. Input / dataset

Data: [../../data/2026-11-06/](../../data/2026-11-06/README.md).

- `features.csv`: three sample windows. Each row has a unique `sample_id` and three integer feature columns.
- `model.json`: `feature_names` (ordered list), `classes` (ordered list), `weights` (K by F integer matrix), and `bias` (K integers).
- `public/`: five test cases with paired feature CSV, model JSON and expected CSV. Cases cover ties, negative scores, different class labels/order, reordered feature columns and empty data.
- IDs are ASCII alphanumeric/underscore tokens, length 1-32; feature values are integers 0-10.
- Feature names for this task are `mean_abs_accel`, `peak_count`, `gyro_energy`. Classes are arbitrary nonempty ASCII tokens from the model, not fixed in code.
- No target labels, train/validation split, real sensor recording or training dataset are provided. Expected files are fixtures for this defined forward calculation, not held-out truth for model quality.

## 5. Output / submission

Create UTF-8 CSV with exactly these columns in this order:

```text
sample_id,label
```

One output row per input row, preserving IDs and order; labels must come from `model.classes`. No index column or extra columns. `sample_submission.csv` illustrates formatting only; its `PLACEHOLDER` values are not predictions and are not valid final labels.

Required handoff: implemented `src/infer.py`, supplied `requirements.txt`, `README.md`, filled `REPORT.md`, and `outputs/submission.csv` generated for the top-level features/model. Bundle them according to [SUBMISSION.md](SUBMISSION.md).

## 6. Constraints & resources

- `0 <= N <= 10000`, `F = 3`, `2 <= K <= 4`; model coefficients/biases are integers -100 to 100.
- CPU only; standard library, no pip install required. An optional Python venv is created by you, not supplied.
- Aim for `O(N*K*F)` compute and bounded working memory per row plus the model. Explain your implementation's actual complexity.
- No device latency/RAM benchmark is claimed. The checker uses a 10-second safety timeout per fixture to catch hangs, not to measure embedded inference performance.
- No Dockerfile, CMake, firmware or neural-network dependency is required by this profile.

## 7. Samples

Sample features, exactly `features.csv`:

```csv
sample_id,mean_abs_accel,peak_count,gyro_energy
s03,0,0,0
s01,1,3,0
s08,3,1,2
```

Sample model, exactly `model.json`:

```json
{"feature_names":["mean_abs_accel","peak_count","gyro_energy"],"classes":["idle","walk","run"],"weights":[[-2,-1,-1],[1,2,-1],[2,1,2]],"bias":[9,0,-4]}
```

Expected predictions:

```csv
sample_id,label
s03,idle
s01,walk
s08,run
```

For `s03`, all features are zero, so the scores equal the biases `[9, 0, -4]`. This explains one sample without implementing the inference runner.

## 8. Verification / evaluation

From this challenge directory, after implementation:

```text
python check_public_tests.py
python src/infer.py --features ../../data/2026-11-06/features.csv --model ../../data/2026-11-06/model.json --output outputs/submission.csv
```

Use `--python PATH_TO_PYTHON` on the checker when testing with a different interpreter. It runs your entry point against five public cases in temporary directories, checks exit status, exact CSV header, ID/row order and label values against each expected CSV. CSV line endings may differ. Output parent directories deliberately do not exist initially.

Fixtures: `01_sample`, `02_ties` (including a three-way tie), `03_reordered_columns`, `04_alternate_model` (negative scores and nonalphabetical tie order), `05_empty`.

Acceptance: all public cases pass; add at least two self-designed tests; explain model loading, tie behavior, order and complexity. Public pass rate measures implementation fidelity only, not real-world classifier quality. No private judge or leaderboard exists. Packaging does not grade the solution.

## 9. Your workflow — before coding

Send your proposed workflow in 4-6 bullets, including your understanding of model/data alignment, one failure risk and verification plan. You decide how to implement it. No completed workflow or forward-pass implementation is supplied.

## 10. Hints & API references — on request

Ask for targeted help with Python CSV/JSON APIs, command-line arguments, file paths, a traceback or one contract rule. Standard-library documentation may be consulted. Hints do not include the complete inference implementation.

## 11. Deliverables & short review

- Source/config and generated predictions as specified in `SUBMISSION.md`.
- Exact run command, runtime version, public results and two tests of your own with expected outputs.
- Filled `REPORT.md`: one decision, one limitation, help used, actual minutes, completed and uncertain parts; justify time/space complexity.
- Suggested budget: 10 minutes reading, 10 workflow, 45 code/debug, 15 tests/packaging and 10 review. Stop at 90 minutes and report partial work if needed; no mandatory catch-up debt.

## 12. Follow-up variation

Optional within the budget: permute the model's class order together with the corresponding weight rows and biases. Predict which tied cases change label and which non-tied cases stay unchanged. Do not modify the supplied public fixtures; use your own copies.
