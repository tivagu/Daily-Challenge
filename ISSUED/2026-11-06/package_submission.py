"""Package learner source and predictions; not an inference solution or judge."""
import csv
import json
from pathlib import Path
import sys
from zipfile import ZipFile, ZIP_DEFLATED


def rows(path):
    with path.open(encoding="utf-8", newline="") as stream:
        return list(csv.reader(stream))


def main():
    root = Path(__file__).resolve().parent
    config = json.loads((root / "challenge.json").read_text(encoding="utf-8"))
    data = (root / config["data_dir"]).resolve()
    sources = {"src/infer.py": root / "src/infer.py", "requirements.txt": root / "requirements.txt", "REPORT.md": root / "REPORT.md", "predictions/submission.csv": root / "outputs/submission.csv"}
    if any(not source.is_file() for source in sources.values()):
        raise ValueError("Required source/report/prediction file missing; see SUBMISSION.md")
    if "STARTER_NOT_IMPLEMENTED" in sources["src/infer.py"].read_text(encoding="utf-8"):
        raise ValueError("Implement the starter before packaging")
    with (data / "features.csv").open(encoding="utf-8", newline="") as stream:
        ids = [row["sample_id"] for row in csv.DictReader(stream)]
    predictions = rows(sources["predictions/submission.csv"])
    classes = json.loads((data / "model.json").read_text(encoding="utf-8"))["classes"]
    if not predictions or predictions[0] != ["sample_id", "label"]:
        raise ValueError("Prediction header must be sample_id,label")
    if any(len(row) != 2 for row in predictions[1:]) or [row[0] for row in predictions[1:]] != ids or any(row[1] not in classes for row in predictions[1:]):
        raise ValueError("Prediction shape, ID order or labels violate the contract")
    destination = root / "submission"
    destination.mkdir(exist_ok=True)
    for version in range(1, 10001):
        suffix = "" if version == 1 else f"-{version}"
        bundle = destination / (config["id"] + "-python" + suffix + ".zip")
        try:
            archive = ZipFile(bundle, "x", compression=ZIP_DEFLATED)
        except FileExistsError:
            continue
        with archive:
            for name, source in sources.items():
                archive.write(source, name)
            archive.writestr("README.md", "# Portable inference submission\n\nPython 3.10+, standard library only. Evaluator supplies model and features externally.\n\npython src/infer.py --features INPUT.csv --model MODEL.json --output OUTPUT.csv\n\nSee REPORT.md for learner evidence. Packaging does not establish correctness.\n")
            archive.writestr("manifest.json", json.dumps({"id": config["id"], "runtime": "Python 3.10+", "entry_point": "src/infer.py", "profile": "dataset-inference", "origin": "self-authored"}, indent=2) + "\n")
        print(f"Created: {bundle}")
        return 0
    raise ValueError("Too many existing bundles")


if __name__ == "__main__":
    try:
        raise SystemExit(main())
    except (OSError, ValueError, KeyError, csv.Error) as exc:
        print(f"Cannot package: {exc}", file=sys.stderr)
        raise SystemExit(2)
