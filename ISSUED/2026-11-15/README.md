# CryCare C11 Pitch Estimator & 5-State Alarm FSM — project kit

**C11 / 240 minutes / CryCare host replay.** Read [problem.md](problem.md), create `c/CMakeLists.txt` yourself, document your workflow in [REPORT.md](REPORT.md), then implement `c/src/alarm_fsm.c`.

Provided: C11 header contracts (`c/include/*.h`), unfinished source stub (`c/src/alarm_fsm.c`), STDIN/STDOUT driver (`c/src/main.c`), [data](../../data/2026-11-15/README.md), checker, packager, and report template.
**Intentionally NOT provided:** `c/CMakeLists.txt` — create it yourself so that `crycare_fsm.exe` builds into `c/build/bin/`.

Run from this kit directory after creating `c/CMakeLists.txt`:

~~~text
cmake -S c -B c/build -G Ninja -DCMAKE_BUILD_TYPE=Release
cmake --build c/build --config Release
python check_public_tests.py -- c/build/bin/crycare_fsm.exe
python package_submission.py
~~~

The starter compiles (once `CMakeLists.txt` is written) and exits with status 2 (`STARTER_NOT_IMPLEMENTED`) until implemented. No ESP-IDF or hardware is needed.
