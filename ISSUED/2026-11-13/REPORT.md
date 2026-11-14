# EDGE-2026-11-13 — learner report

## Before coding (Workflow & CMake)
- This is a daily challenge compute dbfs and send/receive from both S3 and C3
- since INMP4451 send 24 bits in 32 bits (noise) therefore we need to normalize into 16 bits total -> using normalize_i2s_sample()
- since we have samples and num_samples -> we compute dbfs (for a near comparation)
## Evidence
- Compiler/CMake versions and exact commands:
- Public cases passed/failed/not run:
- Test 1 (learner) — input, expected output and why:
- Test 2 (learner) — input, expected output and why:
- `CMakeLists.txt` design notes (why each directive is needed):

## Same-day debrief
- What I wrote and verified:
- One bug/surprising detail I caught:
- Hints/docs used:
- Minutes spent (out of 240):
- Still incomplete or uncertain:
- Source/ZIP path:
