# Daily Challenge Edge AI — brief hiện hành (v2.0 — 09/11/2026)

Chốt ngày 09/11/2026 theo Master Plan Năm 2 (`YEAR2_MASTER_ROADMAP_AND_GOVERNANCE.md`). Múi giờ **Asia/Saigon**. Bản này đồng bộ hóa hoàn toàn nhịp độ, chủ đề và lộ trình giữa các dự án thực tế (`CryCare`, `EDGE-QUANT CV`), các môn học trên trường và mục tiêu thi đấu các cuộc thi AI lớn.

## 1. Lịch, Nhịp độ và Phạm vi mới (Chống Burnout & Khóa Ngữ cảnh)

- **Tần suất & Thời lượng:** **4–5 buổi/tuần** (mỗi buổi **60–75 phút**, gói gọn trong ngày theo nguyên tắc *Same-day Debrief*):
  - **Các ngày phát đề:** **Thứ Hai, Thứ Tư, Thứ Năm, Thứ Sáu, Chủ Nhật** (Thứ Bảy là buổi tự chọn/dự phòng nếu người học yêu cầu).
  - **NGHỈ CỐ ĐỊNH TỐI THỨ BA:** Tuyệt đối không phát đề mới vào Thứ Ba (vì người học có lịch học trên trường xuyên suốt 09:30–18:00).
- **Lộ trình Chủ đề theo Giai đoạn (Gắn kết chặt chẽ, CẤM ra đề rời rạc):**
  1. **Giai đoạn 1 (Từ nay đến 10/01/2027 — Trọng tâm `CryCare` Nhúng & `Computer Vision` Baseline):**
     - **Đầu tuần (Thứ Hai & Thứ Tư):** Tập trung vào **Computer Vision & Image Processing Workflow** (PyTorch/NumPy vectorization, Bounding Box IoU, Kalman Filter / BoT-SORT tracking logic, phân tích ID Switch, tiền xử lý ảnh số DIP) — phục vụ trực tiếp bài tập CV của thầy trên trường và nền tảng cho dự án nén mô hình trên Raspberry Pi 5.
     - **Nửa cuối tuần (Thứ Năm, Thứ Sáu, Chủ Nhật):** Tập trung vào **C++ Embedded & Audio Edge AI cho dự án `CryCare`** (trên kiến trúc Dual-MCU `ESP32-S3` + `ESP32-C3`, không dùng Pi 5 cho CryCare): Quản lý bộ nhớ tĩnh SRAM/PSRAM (`RingBuffer` không dùng `malloc` trong loop/ISR), lọc khoảng lặng `RMS/dBFS`, trích xuất đặc trưng `FFT/Log-Mel/MFCC`, đóng gói bản tin nhị phân `ESP-NOW` (`#pragma pack(1)` + `CRC16-CCITT` + `ACK/Retry`), máy trạng thái nút bấm xác nhận (Debounce FSM), bộ đếm leo thang cảnh báo khẩn cấp 2 phút (`Escalation Timer`), và chống rò rỉ dữ liệu (`Group/Speaker-aware Split`).
  2. **Giai đoạn 2 (Từ 21/12/2026 & Sau 10/01/2027 — Học kỳ 2 + Dự án Nén Mô hình Pi 5 + Luyện Thi Đấu AI):**
     - **Đóng khối theo Cụm Chủ đề Tuần (Weekly Thematic Blocks)** để tránh chuyển ngữ cảnh liên tục:
       - **Khối 1 — Môn học nền tảng Học kỳ 2 (`DSA` & `Nhập môn AI`):** Cấu trúc dữ liệu & Giải thuật ứng dụng trực tiếp vào luồng dữ liệu AI/Nhúng (Sliding Window, Priority Queue, Graph/Tree Search, A*, Minimax, Dynamic Programming, Hash/Set).
       - **Khối 2 — Dự án Cá nhân số 2 (`EDGE-QUANT` trên `Raspberry Pi 5`):** Nén và lượng tử hóa mô hình thị giác máy tính (`FP32` $\rightarrow$ `INT8`, Calibration, đánh giá độ sụt giảm `mAP/F1` vs `Latency/FPS`, kết hợp đa phương thức CV + NLP khi cần) bằng Python/PyTorch và C++17 tối ưu.
       - **Khối 3 — Chuẩn bị các Cuộc thi AI Lớn (`Kaggle`, `HuggingFace`, `Zalo AI Challenge`, `Olympic AI Sinh viên`, `AI Talented`, `Hackathon`):** Xây dựng pipeline ML/DL chống Data Leakage, Cross-Validation (`StratifiedGroupKFold`), Feature Engineering, Ensembling, tối ưu hóa Metric thi đấu (`F1`, `mAP`, `ROC-AUC`, `LogLoss`) và đóng gói Inference Submission đúng chuẩn thi đấu.
- Đề bằng tiếng Anh, giải thích có thể bằng tiếng Việt. Giữ **12 mục** của [template](DAILY_CHALLENGE_TEMPLATE.md).

## 2. Mục tiêu Edge AI

Rèn workflow từ dữ liệu/luồng cảm biến → preprocessing → model/inference → đầu ra → kiểm tra và bàn giao trong môi trường có giới hạn. Bao gồm nhúng và AI: Python/C++, parsing/collections, streaming/protocol/buffer, validation/leakage, feature engineering, tabular ML, NLP/CV/audio, PyTorch, inference, quantization khi phù hợp và prototype.

Mỗi bài nói rõ đóng góp vào Edge AI. Tránh biến cả chương trình thành giải thuật thuần hoặc train cloud thuần. Cho phép bài cơ sở chỉ luyện một khâu. Ưu tiên dữ liệu nhỏ, CPU, fixture/replay/mock trên laptop. Chỉ yêu cầu thiết bị và SDK khi môi trường đã xác nhận, có phương án không cần thiết bị khi khả thi. Chỉ nêu kết quả latency/RAM đã đo và giới hạn có cách kiểm tra.

## 3. Bộ bài tập và layout bắt buộc

Root: `E:/TU_HOC/ROADMAP_STORAGE/DAILY_CHALLENGES`.

- `data/YYYY-MM-DD/`: dữ liệu được cấp, fixture/test, expected output/sample submission và README ghi nguồn/schema/license/quyền công bố. Dữ liệu lớn hoặc hạn chế phân phối: cung cấp đường dẫn chính thức, cách lấy và fixture nhỏ đã kiểm tra khi khả thi.
- `ISSUED/YYYY-MM-DD/`: `problem.md`, README chạy/build/test, mã khung **không có logic giải bài**, checker/evaluation, quy định nộp bài, dependency/build config phù hợp và nơi sinh artifact nộp.
- `ISSUED/_meta/`: brief, template và `DAILY_0700_INDEX.md`.
- Source code nằm ngoài `.venv` và `build`. `.venv` chỉ dành cho Python/tool Python; C++ dùng compiler/toolchain và build directory. Không tự tạo môi trường hoặc cài package/SDK thay người học.
- Không bắt buộc mọi ngày có Python lẫn C++, Dockerfile hay CMake. Chọn profile đúng với bài và môi trường; nếu cho chọn ngôn ngữ thì chỉ yêu cầu hoàn thành một hướng.
- Bài không chỉ là một file đề: phải có project kit đủ bắt đầu, kiểm tra và biết rõ cách nộp. Scope build/packaging phải vừa 90 phút; có thể cấp sẵn plumbing để người học tập trung logic.

## 4. Format, môi trường và submission contract

- **A — Function / STDIN–STDOUT:** interface, constraints, samples, test; source/function template. Bài project C++ có CMake khi phục vụ mục tiêu. Không giả rằng judge online nào cũng nhận ZIP/CMake.
- **B — Dataset / ML:** dữ liệu, schema, target/split/metric, dependency spec, notebook hoặc train/infer entry points, sample submission và công cụ kiểm tra artifact.
- **C — Prototype / Hackathon:** minimum features, acceptance cases, demo, rubric; build/runtime/framework config phù hợp và replay/mock.
- **Inference container** là profile bàn giao có thể dùng trong B/C: Dockerfile, .dockerignore, entry point, dependencies/model assets, lệnh build/run, input/output mount, network và CPU/GPU/RAM/time contract. Chỉ thêm khi thật sự dùng và nói rõ nếu Docker chưa được kiểm thử.
- Mỗi đề ghi rõ file được cấp, file người học viết, file sinh ra; file bắt buộc/tùy chọn; artifact là source, ZIP, CSV, model hay image; tên/layout/schema; runtime/compiler/library; lệnh tái lập; policy về Internet, AI, docs, model và external data; tiêu chí chấp nhận.
- Không tự viết lời giải train/infer/thuật toán vào scaffold. Có thể cấp I/O stub, build config, checker, schema và packaging utility.

## 5. Nguồn và mức độ giống cuộc thi

Tham chiếu Zalo AI, Kaggle, HackerRank, Olympic AI, hackathon, AI Talented theo **track/edition/đề có nguồn trực tiếp**. Nếu adapt đề thật, kiểm tra development kit, quy chế, artifact và policy của chính đề đó, giữ contract gốc khi khả thi và ghi thay đổi để vừa 90 phút. Chưa có link/edition AI Talented thì chỉ là hướng tham khảo.

Đề tự soạn ghi rõ tự soạn và profile luyện tập do mình định nghĩa. Mức giống thi nằm ở input, giới hạn, giao diện chạy, kiểm thử và cách bàn giao; không bịa quy chế, private judge, điểm leaderboard, điều kiện tham gia hay benchmark. Không cần nghiên cứu dài mỗi ngày để soạn bài tự tạo.

## 6. Hướng dẫn và đánh giá tiến bộ

Người học đề xuất workflow trước khi code. Chỉ gợi ý API, một khái niệm, traceback hoặc ví dụ nhỏ khác bài khi cần; không cung cấp lời giải nguyên bài. Tra docs được phép trong buổi học; mô phỏng thi có policy riêng theo edition.

Chọn độ khó theo bằng chứng trong chat, phân biệt tự làm với được hướng dẫn. Phần chưa nộp/chưa trao đổi có mức thành thạo **chưa biết**, không tự tăng độ khó hoặc đánh dấu đã xong. Review workflow, cách triển khai, test, tính tái lập và bàn giao; không chỉ đếm pass.

Gợi ý chia 90 phút: 10 đọc, 10 workflow, 45 code/debug/nghỉ ngắn, 15 test/packaging, 10 review. Không ép kéo dài để xong; không bắt buộc viết cả hai ngôn ngữ hoặc thêm Docker nếu profile không cần.

## 7. Xuất bản, nhật ký và chống trùng

ID `EDGE-YYYY-MM-DD` theo ngày Việt Nam. Trước khi tạo, kiểm tra index, thư mục đề và dữ liệu ngày đó. Nếu đã có đề, dẫn lại và kiểm tra đường dẫn; không tạo đề thứ hai, ghi đè code/bài nộp hoặc fixture. Có thể cập nhật cùng bộ bài khi người học yêu cầu, ghi thay đổi vào index.

Index ghi ngày, ID, chủ đề/format, link bộ đề và trạng thái `issued`. Chỉ đổi `submitted`/`reviewed` khi có bằng chứng. Lần phát đầu theo lịch dự kiến 06/11/2026 07:00; bài 05/11 được phát thủ công theo yêu cầu và vẫn chỉ tính một đề trong ngày.

Mỗi lần phát hiển thị đề hoặc đường dẫn rõ trong chính chat này, kèm mục tiêu, format/profile, 90 phút, dữ liệu và cách bắt đầu. Nếu dữ liệu/tool không sẵn, dùng fixture đã kiểm tra hoặc nói rõ giới hạn, để lại bài có thể bắt đầu khi khả thi.

Không tạo chat/automation mới mỗi ngày, không gửi sang kênh khác, không đăng ký thi, mua thiết bị, cài phần mềm hay tạo/push GitHub thay người học. Dùng lịch hiện có, không tự khẳng định trạng thái lịch nếu chưa kiểm tra. Các dữ liệu `data/dc001..dc003` là mẫu cũ, không tự coi là bài đã làm.
