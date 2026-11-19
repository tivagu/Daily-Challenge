"""Package C11 learner files, without implementing or grading outbox logic."""
import json
from pathlib import Path
import sys
from zipfile import ZipFile, ZIP_DEFLATED


def package(root):
    config = json.loads((root / "challenge.json").read_text(encoding="utf-8"))
    sources = {
        "src/main.c": root / "c/src/main.c",
        "src/mqtt_outbox.c": root / "c/src/mqtt_outbox.c",
        "include/mqtt_outbox.h": root / "c/include/mqtt_outbox.h",
        "CMakeLists.txt": root / "c/CMakeLists.txt",
        "REPORT.md": root / "REPORT.md",
    }
    for source in sources.values():
        if not source.is_file():
            raise ValueError(f"Required file missing: {source}")
    if "starter_not_implemented(" in sources["src/mqtt_outbox.c"].read_text(encoding="utf-8"):
        raise ValueError("Implement mqtt_outbox.c before packaging")
    destination = root / "submission"
    destination.mkdir(exist_ok=True)
    for version in range(1, 10001):
        suffix = "" if version == 1 else f"-{version}"
        bundle = destination / (config["id"] + "-c" + suffix + ".zip")
        try:
            archive = ZipFile(bundle, "x", compression=ZIP_DEFLATED)
        except FileExistsError:
            continue
        with archive:
            for name, source in sources.items():
                archive.write(source, name)
            archive.writestr(
                "README.md",
                "# CryCare C11 JSON Telemetry & MQTT Offline Outbox submission\n\n"
                "C11 standard library.\n"
                "Configure: cmake -S . -B build\n"
                "Build: cmake --build build --config Release\n"
                "Run: build/bin/crycare_mqtt.exe (Windows), build/bin/crycare_mqtt (Unix).\n",
            )
            archive.writestr(
                "manifest.json",
                json.dumps(
                    {
                        "id": config["id"],
                        "origin": "self-authored",
                        "runtime": "C11",
                        "profile": config["profile"],
                        "source": "src/mqtt_outbox.c",
                        "windows_entry_point": "build/bin/crycare_mqtt.exe",
                        "posix_entry_point": "build/bin/crycare_mqtt",
                    },
                    indent=2,
                )
                + "\n",
            )
        return bundle
    raise ValueError("Too many existing bundle versions")


if __name__ == "__main__":
    try:
        print("Created:", package(Path(__file__).resolve().parent))
        print("Packaging is not a correctness check; attach test results and report.")
    except (OSError, ValueError, KeyError) as exc:
        print(f"Cannot package: {exc}", file=sys.stderr)
        raise SystemExit(2)
