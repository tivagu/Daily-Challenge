# Static PCM Ring Buffer — project kit

**C++17 / 60 minutes / CryCare host replay.** Read [problem.md](problem.md), post your workflow, then implement the AudioRingBuffer header and source. The supplied driver already handles the command protocol.

Provided: interface, unfinished source, driver, CMake, [data](../../data/2026-11-11/README.md), checker, packager, and report template. Your work: buffer state/operations, two tests, and the debrief. Record your test inputs and expectations in REPORT.md; you may also keep them as separate .in/.out files.

Run from this kit directory, using the installed compiler and Ninja:

~~~text
cmake -S cpp -B cpp/build -G Ninja -DCMAKE_BUILD_TYPE=Release
cmake --build cpp/build --config Release
python check_public_tests.py -- cpp/build/bin/pcm_ring.exe
python package_submission.py
~~~

On Linux/macOS pass ./cpp/build/bin/pcm_ring. With another installed generator, select it when configuring a fresh build directory. The starter compiles but exits with status 2 until implemented. No ESP-IDF setup is needed for this host exercise.

The checker resolves public fixtures using challenge.json. For your own test, supply its text to the executable's STDIN and compare STDOUT with your expected lines. Fill REPORT.md even if partial. Follow the [submission contract](SUBMISSION.md) to package and hand off your work.
