"""Public artifact checker; contains no inference implementation."""
import argparse
import csv
import json
from pathlib import Path
import subprocess
import sys
import tempfile


def read_csv(path):
    with path.open(encoding="utf-8", newline="") as stream:
        return list(csv.reader(stream))


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--python", default=sys.executable)
    parser.add_argument("--timeout", type=float, default=10)
    args = parser.parse_args()
    if args.timeout <= 0:
        parser.error("timeout must be positive")
    root = Path(__file__).resolve().parent
    config = json.loads((root / "challenge.json").read_text(encoding="utf-8"))
    cases = (root / config["data_dir"] / "public").resolve()
    fixtures = sorted(cases.glob("*.features.csv"))
    if not fixtures:
        print("Checker error: no fixtures", file=sys.stderr)
        return 2
    passed = 0
    for features in fixtures:
        name = features.name.removesuffix(".features.csv")
        model = cases / (name + ".model.json")
        expected = cases / (name + ".expected.csv")
        if not model.is_file() or not expected.is_file():
            print("Checker error: incomplete fixture " + name, file=sys.stderr)
            return 2
        with tempfile.TemporaryDirectory(prefix="edge-infer-") as temporary:
            output = Path(temporary) / "new-parent" / "predictions.csv"
            command = [args.python, str(root / "src/infer.py"), "--features", str(features), "--model", str(model), "--output", str(output)]
            try:
                result = subprocess.run(command, cwd=temporary, capture_output=True, text=True, encoding="utf-8", errors="replace", timeout=args.timeout)
                if result.returncode:
                    print(f"FAIL {name}: exit {result.returncode}")
                    print(result.stderr[:1000], file=sys.stderr)
                elif not output.is_file():
                    print(f"FAIL {name}: output CSV missing")
                elif read_csv(output) != read_csv(expected):
                    print(f"FAIL {name}: header, ID/order or label mismatch")
                else:
                    print(f"PASS {name}")
                    passed += 1
            except subprocess.TimeoutExpired:
                print(f"FAIL {name}: safety timeout")
            except (OSError, csv.Error, UnicodeError) as exc:
                print(f"FAIL {name}: {exc}")
    print(f"Public fixtures: {passed}/{len(fixtures)} passed")
    return 0 if passed == len(fixtures) else 1


if __name__ == "__main__":
    raise SystemExit(main())
