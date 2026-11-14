# Submission contract — EDGE-2026-11-06

Self-authored local dataset/inference profile. These filenames and ZIP rules are defined for this exercise, not attributed to a specific competition.

Implement Python 3.10+ standard-library `src/infer.py`. Entry point accepts `--features`, `--model`, `--output` paths and creates an ordered `sample_id,label` CSV. No model training, network or external assets. Model/input are passed at runtime and must not be hardcoded.

## Artifact

After testing, generate `outputs/submission.csv` for the supplied top-level features/model, fill `REPORT.md`, then run `python package_submission.py`.

ZIP root:

```text
src/infer.py
requirements.txt
README.md
REPORT.md
predictions/submission.csv
manifest.json
```

Packager creates `submission/EDGE-2026-11-06-python.zip`, choosing a numbered suffix if a bundle already exists. It rejects the starter marker and checks the top-level prediction header, ID order and allowed labels. It does not prove correctness, enforce the complexity goal or grade report completeness.

Excluded: public expected answers, test runner, venv/cache, input datasets, previous bundles and model file. The evaluator supplies a compatible model and input features externally. A standalone README in the bundle gives the CLI; the supplied project README remains in the repository.

Submit source/ZIP path, command, public fixture results, two self-designed tests and report in this chat. GitHub is storage for the work; it is not automatically an official submission endpoint. No container/image is required.
