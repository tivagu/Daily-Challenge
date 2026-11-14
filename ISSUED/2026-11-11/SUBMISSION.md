# Submission contract — EDGE-2026-11-11

Self-authored local C++17 exercise. Implement cpp/include/audio_ring_buffer.hpp and cpp/src/audio_ring_buffer.cpp. Keep the driver interface, CMake config and fixtures intact. Evaluation supplies STDIN; do not read public fixture files from your implementation.

Run the checker, fill REPORT.md, then run python package_submission.py. It creates submission/EDGE-2026-11-11-cpp.zip; numbered suffixes preserve existing bundles.

ZIP root:

~~~text
CMakeLists.txt
include/audio_ring_buffer.hpp
src/audio_ring_buffer.cpp
src/main.cpp
README.md
REPORT.md
manifest.json
~~~

The packager checks file presence and rejects the starter marker. It does not validate correctness or report completeness. Public answers, environments, build files and previous ZIPs are excluded. Document both learner tests in REPORT.md, even if you also keep files separately.

From the extracted archive run cmake -S . -B build and cmake --build build --config Release with an installed generator. The executable is build/bin/pcm_ring.exe (Windows) or build/bin/pcm_ring (Linux/macOS).

Submit ZIP/source paths, exact commands, test results and the same-day debrief in this chat. Partial work may be submitted as source paths plus debrief. No firmware, model or external upload required.
