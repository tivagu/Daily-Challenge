"""Package C++ learner files, without implementing or grading audio buffer logic."""
import json
from pathlib import Path
import sys
from zipfile import ZipFile, ZIP_DEFLATED


def package(root):
    config = json.loads((root / "challenge.json").read_text(encoding="utf-8"))
    sources = {"src/main.cpp": root / "cpp/src/main.cpp", "src/audio_ring_buffer.cpp": root / "cpp/src/audio_ring_buffer.cpp", "include/audio_ring_buffer.hpp": root / "cpp/include/audio_ring_buffer.hpp", "CMakeLists.txt": root / "cpp/CMakeLists.txt", "REPORT.md": root / "REPORT.md"}
    for source in sources.values():
        if not source.is_file():
            raise ValueError(f"Required file missing: {source}")
    if "STARTER_NOT_IMPLEMENTED" in sources["src/audio_ring_buffer.cpp"].read_text(encoding="utf-8"):
        raise ValueError("Implement the starter before packaging")
    destination = root / "submission"
    destination.mkdir(exist_ok=True)
    for version in range(1, 10001):
        suffix = "" if version == 1 else f"-{version}"
        bundle = destination / (config["id"] + "-cpp" + suffix + ".zip")
        try:
            archive = ZipFile(bundle, "x", compression=ZIP_DEFLATED)
        except FileExistsError:
            continue
        with archive:
            for name, source in sources.items():
                archive.write(source, name)
            archive.writestr("README.md", "# Static PCM Ring Buffer submission\n\nC++17 standard library. Configure: cmake -S . -B build\nBuild: cmake --build build --config Release\nRun: build/bin/pcm_ring.exe (Windows), build/bin/pcm_ring (Unix).\nChoose a CMake generator appropriate to your installed compiler.\nThe program reads STDIN and writes STDOUT; see REPORT.md for learner test evidence.\nPackaging does not establish correctness.\n")
            archive.writestr("manifest.json", json.dumps({"id": config["id"], "origin": "self-authored", "runtime": "C++17", "profile": config["profile"], "source": "src/audio_ring_buffer.cpp", "windows_entry_point": "build/bin/pcm_ring.exe", "posix_entry_point": "build/bin/pcm_ring"},indent=2)+"\n")
        return bundle
    raise ValueError("Too many existing bundle versions")


if __name__ == "__main__":
    try:
        print("Created:", package(Path(__file__).resolve().parent))
        print("Packaging is not a correctness check; attach test results and report.")
    except (OSError, ValueError, KeyError) as exc:
        print(f"Cannot package: {exc}", file=sys.stderr)
        raise SystemExit(2)
