# CryCare C11 JSON Telemetry & MQTT Offline Outbox — project kit

**C11 / 90 minutes / CryCare host replay.** Read [problem.md](problem.md), create `c/CMakeLists.txt` yourself, document your workflow in [REPORT.md](REPORT.md), then implement `c/src/mqtt_outbox.c`.

Provided: C11 header contract (`c/include/mqtt_outbox.h`), unfinished source stub (`c/src/mqtt_outbox.c`), STDIN/STDOUT driver (`c/src/main.c`), [data](../../data/2026-11-17/README.md), checker, packager, and report template.
**Intentionally NOT provided:** `c/CMakeLists.txt` — create it yourself so that `crycare_mqtt.exe` builds into `c/build/bin/`.

Run from this kit directory after creating `c/CMakeLists.txt`:

~~~text
cmake -S c/src -B c/build -G Ninja -DCMAKE_BUILD_TYPE=Release
cmake --build c/build --config Release
python check_public_tests.py -- c/build/bin/crycare_mqtt.exe
python package_submission.py
~~~
