# Daily Challenge Edge AI — repository học và nộp bài

Mỗi ngày phát một **bộ bài tập 90 phút** lúc 07:00 Asia/Saigon. Bạn tự đề xuất workflow, viết code, kiểm thử và bàn giao; mã khung không chứa lời giải. Cấu trúc và cách nộp được chọn theo từng bài, không dùng một cây thư mục cứng cho mọi cuộc thi.

## Cấu trúc chung

```text
DAILY_CHALLENGES/                 <- thư mục có thể dùng làm gốc repository GitHub
  README.md
  .gitignore
  data/
    2026-11-05/
      README.md                  <- nguồn dữ liệu, schema và cách dùng
      public/                    <- input .in và expected output .out của bài hôm nay
    dc001/, dc002/, dc003/        <- dữ liệu mẫu cũ, chưa gắn với ngày phát
  ISSUED/
    _meta/
      DAILY_0700_EDGE_AI_BRIEF.md
      DAILY_CHALLENGE_TEMPLATE.md
      DAILY_0700_INDEX.md
    2026-11-05/
      problem.md                 <- đề tiếng Anh, đủ 12 mục
      README.md                  <- hướng dẫn mở, chạy, build và kiểm tra
      challenge.json             <- ID, đường dẫn data và cấu hình bộ bài
      check_public_tests.py      <- chạy bài của bạn với test công khai
      package_submission.py      <- đóng gói theo contract của bài này
      SUBMISSION.md              <- phải nộp gì, tên file và cách đóng gói
      REPORT.md                  <- bạn ghi workflow, test, review và trợ giúp
      python/
        solution.py              <- mã khung, bạn tự triển khai
        requirements.txt
        .venv/                   <- bạn tạo khi cần, không commit
      cpp/
        CMakeLists.txt
        src/main.cpp             <- mã khung C++17, bạn tự triển khai
        build/                   <- file do compiler/CMake tạo, không commit
      submission/                <- ZIP sinh ra khi bạn đóng gói, không commit ZIP
```

`ISSUED` có nghĩa là **các bộ đề đã phát**. `data` chứa đầu vào, nhãn, mẫu nộp hoặc test được cấp cho từng ngày. Trong ảnh cũ, `cases` nằm chung với đề; 16 file của ngày 05/11 đã được chuyển sang `data/2026-11-05/public`, giữ nguyên nội dung.

`.in` là đầu vào thử; `.out` là kết quả đúng để đối chiếu, không phải file thực thi hay lời giải. `check_public_tests.py` chỉ chạy chương trình của bạn và so kết quả; `problem.md` là nội dung đề. Windows Explorer có thể đang ẩn đuôi `.py`/`.md`.

## Phần nào được cấp, phần nào bạn làm?

| Thành phần | Trách nhiệm |
| :--- | :--- |
| Đề, dữ liệu công khai, schema, samples, checker, quy định nộp | Được cấp cùng đề; giữ nguyên để đối chiếu. |
| File khung, dependency/build config | Được cấp đủ để bắt đầu, chưa có logic giải bài. |
| Workflow, xử lý dữ liệu/model/logic, test riêng, kết quả và review | Bạn tự thực hiện. |
| `submission/` | Nơi sinh artifact bàn giao, theo `SUBMISSION.md` của từng bài. |

Bài hôm nay cho chọn **Python hoặc C++**, chỉ cần hoàn thành một hướng. Không bắt buộc viết cả hai.

## Môi trường Python và C++

**Python:** `.venv` là môi trường Python, tách interpreter và các package. Code đặt trong `python/`, bên ngoài `.venv/`. Bạn có thể tạo `.venv` riêng cho mỗi bộ bài khi dependency khác nhau; không phải bắt buộc tạo một môi trường mới mỗi ngày. [Tài liệu Python venv](https://docs.python.org/3/library/venv.html).

Với Python Windows chuẩn, chạy từ thư mục `ISSUED/2026-11-05/python`:

```powershell
python -m venv .venv
.\.venv\Scripts\python.exe -m pip install -r requirements.txt
```

Không cần activate nếu gọi thẳng interpreter trong `.venv`. Một bản Python từ MSYS2 có thể dùng `bin/` thay vì `Scripts/`; xem vị trí interpreter thực tế hoặc chọn Python Windows bạn dùng trong IDE. Bài hôm nay chỉ cần standard library, nên không có package bên ngoài cần cài.

**C++:** môi trường chính gồm compiler/toolchain, phiên bản chuẩn C++, thư viện và cấu hình build; `cpp/build/` chứa kết quả build. `CMakeLists.txt` mô tả cách build, và CMake sinh cấu hình cho công cụ build như Ninja hoặc Visual Studio. [Tài liệu CMake](https://cmake.org/cmake/help/latest/manual/cmake.1.html).

Nếu một bài nhúng dùng Python để chạy tool hỗ trợ, bạn có thể tạo `.venv` cho các tool đó; compiler và SDK C++ vẫn được quản lý riêng. Bài ESP-IDF/PlatformIO sẽ dùng cấu trúc dự án của framework được chọn và có phương án mock/replay khi phù hợp.

## Cấu trúc thay đổi theo contract từng bài

Đây là các profile luyện tập. Khi dùng đề thật, ưu tiên chính development kit và quy chế của track/edition có nguồn trực tiếp; không tự gán một cấu trúc cho cả Zalo AI, Kaggle hay Olympic AI.

| Profile | File có thể cần | Artifact bàn giao được ghi rõ trong đề |
| :--- | :--- | :--- |
| Function / STDIN–STDOUT | Source hoặc function template; CMake nếu là bài luyện project C++ | Source/function đúng interface; bộ bài local có thể thêm ZIP tái lập. |
| Dataset / ML | `src/`, notebook, dependencies, train/infer entry points, sample submission | CSV/JSON theo schema; code/model/report nếu contract yêu cầu. |
| Inference container | `Dockerfile`, `.dockerignore`, dependencies, entry point, model assets manifest | Image hoặc source bundle; lệnh build/run, I/O và compute budget cụ thể. |
| Embedded / prototype | CMake/SDK config hoặc `platformio.ini`, `src/`, mock/replay, demo | Source, build/flash/run instructions và acceptance evidence. |

`Dockerfile` mô tả cách tạo container image, nên chỉ thêm khi container là phần bắt buộc hoặc mục tiêu học của bài. Nó không thay thế source, interface inference hay danh sách dependency. [Tài liệu Dockerfile](https://docs.docker.com/reference/dockerfile/).

Mỗi ngày có một **submission contract**: runtime/language, entry point, layout ZIP/image/source/CSV, tên file/schema, lệnh chạy, quyền dùng mạng/model/AI, dữ liệu được phép và tiêu chí kiểm tra. File bắt buộc, tùy chọn và file sinh ra được phân biệt rõ. Nếu scaffold được cấp sẵn thì phần packaging/build vẫn nằm trong ngân sách 90 phút.

## GitHub

Bạn tự tạo repository và chọn public/private. Thư mục này đã có `.gitignore` để bỏ `.venv`, build outputs, cache, checkpoint lớn và ZIP sinh ra. Commit source, cấu hình, hướng dẫn và review. Dữ liệu tổng hợp nhỏ của bài hôm nay có thể commit; với dữ liệu thi thật, làm theo giấy phép và quyền công bố của nguồn, dùng hướng dẫn tải/checksum nếu không đưa lên repo.

Không đặt code trong `.venv` hoặc `build`. Không coi GitHub repo là artifact nộp thi mặc định: artifact vẫn theo `SUBMISSION.md` của từng ngày. Chưa khởi tạo Git, tạo remote hay push trong lượt thiết lập này.

## Bắt đầu hôm nay

1. Đọc [đề 05/11/2026](ISSUED/2026-11-05/problem.md).
2. Gửi workflow dự kiến trong chat; chọn Python hoặc C++.
3. Làm theo [README của bài](ISSUED/2026-11-05/README.md) để code, chạy test và đóng gói.
4. Nộp code/artifact, kết quả test và review. Bài chưa nộp vẫn chưa có bằng chứng về mức thành thạo.

Quy định phát đề: [brief](ISSUED/_meta/DAILY_0700_EDGE_AI_BRIEF.md). Nhật ký: [index](ISSUED/_meta/DAILY_0700_INDEX.md).
