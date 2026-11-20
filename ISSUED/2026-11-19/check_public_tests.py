"""Run a learner's STDIN/STDOUT program against the supplied public fixtures."""

import argparse
import json
from pathlib import Path
import subprocess
import sys


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--timeout", type=float, default=10.0)
    parser.add_argument("command", nargs=argparse.REMAINDER)
    args = parser.parse_args()
    command = args.command
    if command and command[0] == "--":
        command = command[1:]
    if not command:
        parser.error("provide your run command after --")
    if args.timeout <= 0:
        parser.error("--timeout must be positive")

    root = Path(__file__).resolve().parent
    try:
        config = json.loads((root / "challenge.json").read_text(encoding="utf-8"))
        case_dir = (root / config["data_dir"]).resolve()
    except (OSError, ValueError, KeyError, TypeError) as exc:
        print(f"Checker error: cannot load data location: {exc}", file=sys.stderr)
        return 2
    inputs = sorted(case_dir.glob("*.in"))
    if not inputs:
        print("Checker error: no public input files found.", file=sys.stderr)
        return 2
    passed = 0
    for input_path in inputs:
        output_path = input_path.with_suffix(".out")
        if not output_path.is_file():
            print(f"Checker error: missing {output_path.name}", file=sys.stderr)
            return 2
        expected = output_path.read_text(encoding="utf-8").splitlines()
        try:
            result = subprocess.run(
                command,
                input=input_path.read_text(encoding="utf-8"),
                capture_output=True,
                text=True,
                encoding="utf-8",
                errors="replace",
                timeout=args.timeout,
                check=False,
            )
        except subprocess.TimeoutExpired:
            print(f"FAIL {input_path.stem}: safety timeout ({args.timeout:g}s)")
            continue
        except OSError as exc:
            print(f"Cannot run submission: {exc}", file=sys.stderr)
            return 2

        if result.returncode != 0:
            print(f"FAIL {input_path.stem}: exit code {result.returncode}")
            if result.stderr:
                print(result.stderr[:2000], file=sys.stderr)
            continue

        actual = result.stdout.splitlines()
        if actual == expected:
            passed += 1
            print(f"PASS {input_path.stem}")
        else:
            print(f"FAIL {input_path.stem}")
            print("  expected:", repr("\n".join(expected)))
            print("  actual:  ", repr(result.stdout[:2000]))

    print(f"\nPublic fixtures: {passed}/{len(inputs)} passed.")
    return 0 if passed == len(inputs) else 1


if __name__ == "__main__":
    raise SystemExit(main())
