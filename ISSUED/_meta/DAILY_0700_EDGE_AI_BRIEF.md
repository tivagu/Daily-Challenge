# Daily Challenge Edge AI — brief hiện hành (v3.0 — 09/11/2026)

Nguồn quyết định: người học đã chọn rõ trong chat ngày 09/11/2026: **3 buổi T2/T4/T6, mỗi tuần một chủ đề; ưu tiên C++/ESP-IDF/FreeRTOS CryCare đến 10/01/2027; T5/T7 chỉ review.** Quyết định này thay lịch hằng ngày/90 phút, lịch 4–5 buổi và cách chia đầu tuần CV/cuối tuần nhúng trong brief v2. Múi giờ **Asia/Saigon**.

## 1. Lịch và thời lượng

- **Phát đề mới lúc 07:00 Thứ Hai, Thứ Tư, Thứ Sáu**, trong chính chat này. 07:00 là giờ giao; người học tự chọn giờ làm, thường vào buổi tối.
- Mỗi đề **60–75 phút**, công bố ngân sách cụ thể 60 hoặc 75 phút, bao gồm workflow, code, build, test, đóng gói và **Same-day Debrief**.
- **Thứ Năm và Thứ Bảy chỉ review** bài/module đã có: đọc code, giải thích quyết định, chạy lại test, sửa một lỗi hoặc một biến thể nhỏ của đề cũ. Không phát ID/bộ đề mới, không tự tạo lịch review hay gửi nhắc khi chưa được yêu cầu. Không có bài nộp thì mức thành thạo vẫn chưa biết.
- **Thứ Ba nghỉ cố định** (học 09:30–18:00); **Chủ Nhật không tự phát đề**. Không thêm đề bù khi bỏ lỡ một buổi, không chuyển sang ngày nghỉ do lịch chạy lại.
- **Same-day Debrief** nghĩa là chốt trạng thái trong ngày: xong phần nào, test nào đã chạy, lỗi/giới hạn và trợ giúp. Hết giờ được nộp dở; không bắt buộc tăng giờ để hoàn thành. Phần còn lại có thể dùng trong buổi review, không biến thành nợ bài.
- Tổng ba buổi chính: 3–3,75 giờ/tuần, chưa gồm review linh hoạt. Không tự khẳng định tổng ngân sách tuần cũ còn chính xác hay lấp giờ được giảm bằng việc mới.

## 2. Các giai đoạn và khóa chủ đề tuần

### Đến hết 10/01/2027 — CryCare Embedded là ưu tiên chính

- C++ cho firmware **ESP-IDF + FreeRTOS**, OOP và an toàn bộ nhớ. Mục tiêu là tự triển khai module để kết nối audio/model, cảnh báo cục bộ, và contract sự kiện với app/backend/database.
- Kiến trúc CryCare: **ESP32-S3** lắng nghe/chạy TinyML INT8 tại nôi bé; **ESP32-C3** điều khiển loa và nút xác nhận tại phòng bố mẹ; liên lạc cục bộ **ESP-NOW**. Raspberry Pi 5 thuộc EDGE-QUANT, không là phần cứng sản phẩm CryCare.
- **Một chủ đề xuyên suốt cả tuần T2/T4/T6**, các bài nhỏ nối tiếp cùng module. Không đổi sang tuần Python CV chỉ vì đến Thứ Hai/Thứ Tư. Dùng Python làm parity test/fixture/tool bàn giao khi phục vụ cùng module, không bắt buộc viết lại mọi bài bằng hai ngôn ngữ.
- Lộ trình module dự kiến: ring buffer có capacity cố định/ownership/overflow; FreeRTOS queue, mutex, task và watchdog; RMS/dBFS tính **trước zero-padding**; DSP/features và input tensor INT8; packet/CRC/ACK/retry; debounce FSM và escalation 120 giây; event_id/JSON schema, offline queue và backend mock.
- Không cấp phát động trong vòng lặp xử lý thời gian thực/ISR; mô tả rõ khởi tạo, SRAM/PSRAM, quyền sở hữu buffer, concurrency và giới hạn đo được. Không đưa số đo bộ nhớ/latency ví dụ vào source như kết quả thật.
- Trong đề packet, định nghĩa chính xác byte order, layout/độ dài, version, CRC variant với toàn bộ tham số và test vector; dùng fixed-width types. Nếu dùng packed struct phải kèm kiểm tra kích thước/serialization theo wire contract. Phân biệt callback gửi, ACK ứng dụng và xác nhận người dùng; CRC không được coi là cơ chế xác thực.
- Bài kết nối app/database luyện schema/API client/replay/offline behavior với mock đã cấp; không tự sửa production backend/database hoặc gửi sự kiện thật.
- CV, HĐH, OOP, Xử lý ảnh số có thể hỗ trợ mục tiêu hoặc lịch học riêng; không tự thêm thành buổi Daily Challenge ngoài ba buổi chính. Daily Challenge phục vụ trách nhiệm nhúng của người học và không thay lịch các mảng khác.

### 21/12/2026–10/01/2027 — chuyển tiếp Học kỳ 2

Từ khi HK2 mở ngày 21/12, gắn DSA/Nhập môn AI vào module CryCare khi phù hợp (queue, sliding window, event ordering, state machine). Trọng tâm vẫn là tích hợp/kiểm thử CryCare đến hạn 10/01; không coi 21/12 là mốc tự chuyển hẳn sang EDGE-QUANT.

### Từ 11/01/2027 — luân phiên các cụm chủ đề tuần

Giữ nhịp ba buổi/tuần và review T5/T7. Chọn một cụm chính mỗi tuần dựa trên đề cương, deadline và bài nộp:

1. **DSA + Nhập môn AI:** sliding window, priority queue, hash/set, graph/tree search, A*, dynamic programming gắn với luồng dữ liệu AI. Chỉ tuyên bố khớp đề cương trường khi đã đọc đề cương tương ứng.
2. **Python AI / Computer Vision + EDGE-QUANT trên Raspberry Pi 5:** preprocessing/validation, baseline, PyTorch, FP32 -> INT8, calibration; đo quality (F1/mAP) và latency/FPS trên cùng điều kiện. C++ tối ưu là phần nối tiếp của cùng workflow khi cần. NLP/đa phương thức là mở rộng theo mục tiêu, không tự thêm vào mọi tuần.
3. **Workflow thi AI:** Kaggle, Hugging Face, Zalo AI, Olympic AI Sinh viên, AI Talented, hackathon theo track/edition có nguồn. Validation chống leakage, stratified/group split, feature engineering, baseline -> tuning, metric và artifact nộp. Không mặc định cả tuần phải train mạng neural.

Không ấn định thành thạo theo tuần. Có thể giữ cùng chủ đề thêm tuần; đề mới chỉ phụ thuộc kỹ năng đã có bằng chứng hoặc cung cấp phương án độc lập. [WEEKLY_TOPIC_BLOCKS.md](WEEKLY_TOPIC_BLOCKS.md) ghi kế hoạch module và trạng thái thực tế.

## 3. Bộ bài tập, nguồn và cấu trúc thi

Root: `E:/TU_HOC/ROADMAP_STORAGE/DAILY_CHALLENGES`.

- `data/YYYY-MM-DD/`: dữ liệu/fixture/test/expected output và README nguồn, schema, quyền phân phối.
- `ISSUED/YYYY-MM-DD/`: `problem.md` **12 mục bằng tiếng Anh**, README chạy/build/test, source stub không có lời giải, checker/evaluation, dependency/build config, submission contract, report template và nơi sinh artifact. Giải thích/review có thể bằng tiếng Việt.
- `ISSUED/_meta/`: brief, template, index và kế hoạch tuần. Buổi review ghi vào `REVIEW.md` của đề đã phát khi thực sự review, không tạo đề mới trong ISSUED.
- Chọn format A function/STDIN–STDOUT, B dataset/ML hoặc C prototype/hackathon theo mục tiêu. Tên file/artifact/CLI/schema, công cụ, dữ liệu/model/AI/Internet policy và acceptance criteria phải rõ.
- Cấp CMake, cấu trúc component ESP-IDF, `sdkconfig.defaults`, dependency manifest hoặc Python environment spec khi profile thực sự dùng; Dockerfile chỉ khi contract container cần. Không ép mọi bài thành một source đơn hay mọi bài đều Docker.
- Bài tự soạn ghi rõ contract luyện tập. Bài adapt cuộc thi thật đối chiếu nguồn trực tiếp của track/edition và công bố giản lược. Chưa có nguồn AI Talented thì không gán quy chế chính thức. Không giả private judge, leaderboard hay benchmark.

## 4. Môi trường và điều kiện bắt đầu

- Ưu tiên host C++ test/replay/mock đủ bắt đầu ngay. Bài ESP-IDF/FreeRTOS phải có scaffold/toolchain contract thích hợp; ghi rõ host test chưa chứng minh scheduler, radio hoặc phần cứng thật hoạt động.
- Chỉ bắt buộc flash/run board khi thiết bị, phiên bản ESP-IDF và môi trường của người học đã xác nhận; không mặc định SDK version trong roadmap đã được cài. Trước khi dùng API, kiểm tra docs đúng target/version.
- Khi cần phần cứng, kèm host fallback khi khả thi; không bịa kết quả HIL, PSRAM, latency/FPS/RAM hoặc suy ra từ benchmark máy khác. Chỉ giữ mục tiêu cần đo cho tới khi có bằng chứng.
- Source ngoài `.venv` và `build`; venv dành cho Python/tool Python, compiler/SDK C++ quản lý riêng. Không tự cài SDK/package, tạo venv, mua thiết bị, đăng ký thi hay tạo/push repo.

## 5. Cách luyện, review và đánh giá

Người học tự đề xuất workflow trước khi code. Chỉ trợ giúp đúng chỗ qua định nghĩa, API, traceback hoặc ví dụ nhỏ khác bài; **không lời giải nguyên bài**. Scaffold chỉ là interface/build/test/packaging plumbing, không chứa logic cần học.

Mỗi đề công bố đúng một phần lõi vừa 60–75 phút và phần tùy chọn rõ ràng. Gợi ý 60 phút: 5 đọc + 5 workflow + 30 code/debug + 10 test/packaging + 10 debrief. Đề 75 phút có thể thêm 15 phút cho triển khai/test, không bỏ review.

Debrief gồm: phần tự làm, trợ giúp, test đã chạy, một quyết định và một giới hạn, thời gian thật, xong/dở/chưa rõ. Chưa nộp thì vẫn chưa biết; file tồn tại hoặc đề đã phát không chứng minh đã luyện/thành thạo. Review không mở rộng thành đề khó hơn để bù.

## 6. Phát đề, chống trùng và ghi nhận

- Dùng giờ/ngày **Asia/Saigon**. Kiểm tra weekday trước, chỉ phát mới T2/T4/T6. Lượt đánh thức đến muộn vẫn dùng ngày hiện tại, báo giờ thực nếu cần, không phát bù nhiều đề.
- ID `EDGE-YYYY-MM-DD`. Kiểm tra index cùng thư mục đề/data trước khi tạo. Đã có thì dẫn lại, không tạo đề thứ hai hay ghi đè code/fixture/bài nộp.
- Index bắt đầu `issued`; chỉ `submitted/reviewed` khi có bằng chứng. Ghi thêm weekly block/module, thời lượng và cách kiểm tra. Buổi review chỉ gắn với đề đã có.
- Các đề đến 09/11 đã phát theo cấu hình cũ được giữ nguyên như lịch sử, gồm bài 09/11 90 phút. Không đổi hợp đồng bài đã phát hoặc phát đề C++ thứ hai cùng ngày. Từ buổi mới tiếp theo áp dụng cấu hình này.
- Hiển thị link đề/data/README, topic/module và 60 hoặc 75 phút trong chính chat này. Không tạo chat/automation mới hoặc gửi sang kênh khác. Chỉ cập nhật automation hiện có sang T2/T4/T6 07:00.
