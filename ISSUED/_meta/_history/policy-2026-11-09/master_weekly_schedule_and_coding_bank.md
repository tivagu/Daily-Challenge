# LỊCH TỰ HỌC & DAILY CHALLENGE THEO WORKFLOW ỨNG DỤNG

> **Cập nhật Master Plan Năm 2 (Phiên bản 4.0 — 08/11/2026):** Đồng bộ hóa với [YEAR2_MASTER_ROADMAP_AND_GOVERNANCE.md](YEAR2_MASTER_ROADMAP_AND_GOVERNANCE.md) và [docs/handoff.md](docs/handoff.md).
> - **Mốc Học kỳ:** Kết thúc Học kỳ 1 vào đầu tháng 12/2026; **2 tuần vàng (07/12 – 20/12/2026)** dồn toàn lực nhúng C++ cho **CryCare (`ESP32-S3` + `ESP32-C3`)**; Học kỳ 2 (`DSA`, `Nhập môn AI`, `Lập trình Web & Ứng dụng`) bắt đầu ngày **21/12/2026**; hạn chót Vòng loại 2 CryCare là **10/01/2027**.
> - **Nhịp Daily Challenge mới:** Giảm xuống **4–5 buổi/tuần** (60–75 phút/buổi vào tối Thứ Hai, Thứ Tư, Thứ Năm, Thứ Sáu, Chủ Nhật). **Tuyệt đối NGHỈ Daily Challenge vào tối Thứ Ba** (do lịch học trên trường 09:30–18:00). Mọi đề đều gắn trực tiếp với `CryCare`, `Computer Vision (EDGE-QUANT)` và `DSA` theo quy tắc *Zero Vibe-Coding* & *Same-day Debrief*.

**Phiên bản:** 4.0 — ngày 08/11/2026, múi giờ Asia/Saigon.  
**Ưu tiên:** hiểu workflow, tự triển khai, luyện phản xạ đọc đề → chọn cách làm → code → kiểm tra → bàn giao.  
**Cách đọc:** lịch chỉ ghi khung giờ và mảng học. Nội dung chuyên môn được chỉ định trong phần Daily Challenge; các mảng còn lại theo đầu việc thực tế tại [YEAR2_MASTER_ROADMAP_AND_GOVERNANCE.md](YEAR2_MASTER_ROADMAP_AND_GOVERNANCE.md).

## 1. Phạm vi và ngân sách tuần (Chuẩn 55 giờ/tuần — ~8 giờ/ngày)

Giữ năm mảng cốt lõi: **Trường/Môn nền tảng (`DSA`, `AI`, `Web`, `HĐH`, `DIP`), Dự án Trọng tâm theo Pha (`CryCare` / `EDGE-QUANT`), Computer Vision của Thầy & Seminar, Tiếng Anh (IELTS 7.0–7.5), và Daily Challenge**. Một phần việc chỉ được tính thời gian một lần.

| Hoạt động | Giờ/tuần | Tỷ lệ | Ghi chú điều phối |
| :--- | ---: | :---: | :--- |
| Học trên trường + Bài tập môn học | 16,0 | 29% | Tận dụng giờ trống tại lớp (chiều Thứ 3 15:30–18:00, chiều Thứ 4 12:00–18:00, cả ngày Thứ 5) để làm dứt điểm bài trường |
| Dự án Trọng tâm (`CryCare` Pha 1–2 / `EDGE-QUANT` Pha 3–4) | 15,0 | 27% | 5 ca Deep Work × 3 giờ (`ESP32-S3/C3` cho CryCare; `Raspberry Pi 5` cho nén mô hình CV) |
| Computer Vision của Thầy & Seminar | 8,0 | 15% | `Vectorized IoU`, `MOT17-02 + BoT-SORT`, tích hợp trực tiếp làm Baseline cho `EDGE-QUANT` |
| Tiếng Anh (IELTS 7.0–7.5) | 7,0 | 13% | 21:00–22:00 mỗi tối (3 buổi cùng Mentor + 4 buổi tự luyện phản xạ Speaking/Listening) |
| Daily Challenge (4–5 buổi/tuần) | 5,5 | 10% | 19:00–20:15 tối T2, T4, T5, T6, (T7/CN linh hoạt); **Nghỉ tối Thứ Ba** |
| Buffer & Họp Đội Archeon / Review lịch | 3,5 | 6% | Họp đội CryCare, cập nhật `docs/handoff.md` và dự phòng sự cố phần cứng |
| **Tổng ngân sách** | **55,0** | **100%** | **Trung bình ~8 giờ/ngày — bền vững suốt Năm 2** |

 Đây là mức chuẩn bền vững. Nghỉ ngắn và ghi chú nằm trong block. Buffer không dùng được giữ trống. Khi lịch thực tế thay đổi, chuyển/giảm block linh hoạt thay vì cộng thêm giờ.

## 2. Lịch tuần: giờ nào học mảng nào

### 2.0. Lịch Tác Chiến Đặc Biệt Tuần 09/11 – 15/11/2026 (Giải quyết 2 môn trường + CV Thầy + CryCare)

- **Thứ Hai (09/11):** Sáng làm *Thuyết trình Hệ điều hành*; Chiều sửa lỗi zero-padding `rms/dbfs` trong `CryCare` + code `Vectorized IoU` trên Colab (16:30–17:30); Tối Daily Challenge #1 (19:00–20:15) + Tiếng Anh Mentor (21:00–22:00).
- **Thứ Ba (10/11):** 09:30–15:00 học kiến thức mới trên trường; **15:30–18:00 (làm việc riêng tại trường):** chạy `MOT17-02 + BoT-SORT` trên Colab & hoàn thiện Slide HĐH; **Tối NGHỈ Daily Challenge**, ôn bài thuyết trình HĐH + Tiếng Anh tự học (21:00–21:45).
- **Thứ Tư (11/11):** 09:30–12:00 học bắt buộc trên trường; **13:00–18:00:** rà soát Proof-of-Work CV trình Thầy & dự **Seminar PyTorch**; Tối Daily Challenge #2 (19:00–20:15) + Tiếng Anh Mentor (21:00–22:00).
- **Thứ Năm (12/11):** **09:30–16:30 (làm việc riêng tại trường):** Ca sáng viết dứt điểm *Báo cáo cuối kỳ Xử lý ảnh số (DIP)*, Ca chiều chốt xong 100% *Thuyết trình Hệ điều hành*; Tối Daily Challenge #3 (19:00–20:15) + Tiếng Anh tự học (21:00–21:45).
- **Thứ Sáu (13/11):** Sáng huấn luyện `Softmax` & `DS-CNN` cho `CryCare`; Chiều viết khung `FinalReport_CV.pdf`; Tối Daily Challenge #4 (19:00–20:15) + Tiếng Anh tự học (21:00–21:45).
- **Thứ Bảy (14/11):** Sáng test phần cứng `ESP32-S3` (I2S Mic) + `ESP32-C3` (`ESP-NOW`); Chiều tổng duyệt bài HĐH/DIP & họp đội Archeon; Tối Tiếng Anh Mentor (21:00–22:00).
- **Chủ Nhật (15/11):** Sáng tổng kết tuần & cập nhật `docs/handoff.md`; Chiều (12:00–20:30) **Nghỉ ngơi hoàn toàn**; Tối Tiếng Anh Tổng hợp Ngày 7 (20:30–21:30).

### 2.1. Ban ngày và Daily Challenge (Nhịp chuẩn hằng tuần)

| Ngày | Khung giờ | Hoạt động | Nhóm ngân sách | Giờ |
| :--- | :--- | :--- | :--- | ---: |
| Thứ Hai | 09:00–12:00 | Dự án Trọng tâm (`CryCare` / `EDGE-QUANT`) | Dự án | 3,00 |
| Thứ Hai | 12:00–13:15 | Họp Đội Archeon (`CryCare`) | Buffer/Họp | 1,25 |
| Thứ Hai | 14:00–17:00 | Computer Vision của Thầy & Baseline `EDGE-QUANT` | CV/Seminar | 3,00 |
| Thứ Hai | 19:00–20:15 | Daily Challenge #1: Triển khai module gắn với CryCare/CV/DSA | Coding | 1,25 |
| Thứ Ba | 09:30–15:00 | Học kiến thức mới trên trường | Trường | 5,00 |
| Thứ Ba | 15:30–18:00 | Làm bài tập trường / CV trên máy tính tại trường | Trường | 2,50 |
| Thứ Ba | 19:00–20:30 | **NGHỈ DAILY CHALLENGE** — Ôn bài trường nhẹ nhàng | Trường | 1,50 |
| Thứ Tư | 09:30–12:00 | Học bắt buộc trên trường | Trường | 2,50 |
| Thứ Tư | 13:00–15:00 | Chuẩn bị Proof-of-Work CV & bài trường tại lớp | CV/Seminar | 2,00 |
| Thứ Tư | 15:00–18:00 | Seminar PyTorch / Làm việc cùng Giảng viên | CV/Seminar | 3,00 |
| Thứ Tư | 19:00–20:15 | Daily Challenge #2: Vận dụng, debug & kiểm thử biên (Same-day Debrief) | Coding | 1,25 |
| Thứ Năm | 09:30–12:00 | Giải quyết dứt điểm bài tập/báo cáo môn trường tại lớp | Trường | 2,50 |
| Thứ Năm | 13:30–16:30 | Dự án Trọng tâm (`CryCare` / `EDGE-QUANT`) tại trường | Dự án | 3,00 |
| Thứ Năm | 19:00–20:15 | Daily Challenge #3: Thuật toán & Cấu trúc dữ liệu (`DSA` / Edge Stream) | Coding | 1,25 |
| Thứ Sáu | 09:00–12:00 | Dự án Trọng tâm (`CryCare` Audio ML & Firmware) | Dự án | 3,00 |
| Thứ Sáu | 13:30–16:30 | Dự án Trọng tâm (`CryCare` / `EDGE-QUANT`) | Dự án | 3,00 |
| Thứ Sáu | 16:30–17:30 | Buffer & Review lịch | Buffer/Họp | 1,00 |
| Thứ Sáu | 19:00–20:15 | Daily Challenge #4: Tối ưu hóa Vectorization / Memory / Quantization | Coding | 1,25 |
| Thứ Bảy | 09:00–12:00 | Thực nghiệm Phần cứng (`ESP32-S3/C3` HIL hoặc `Pi 5`) | Dự án | 3,00 |
| Thứ Bảy | 13:30–15:30 | Ôn tập môn trường (`DSA`, `AI`, `Web`) | Trường | 2,00 |
| Thứ Bảy | 15:30–16:45 | Buffer dự phòng phần cứng | Buffer/Họp | 1,25 |
| Chủ nhật | 09:00–11:30 | Tổng kết tuần, cập nhật `docs/handoff.md` & Daily Challenge #5 (Tuỳ chọn) | Coding/Review | 0,50 |

### 2.2. Tiếng Anh và nghỉ

| Ngày | Khung giờ | Hoạt động |
| :--- | :--- | :--- |
| Thứ Hai, thứ Tư, thứ Bảy | 21:00–22:00 | Tiếng Anh — Mentor (Tập trung Speaking 7.0+ & Listening; thêm R/W từ tháng 01/2027) |
| Thứ Ba, thứ Năm, thứ Sáu | 21:00–21:45 | Tiếng Anh — Tự học (Phản xạ Collocation + Ôn tập cuốn chiếu Ngày 3 & 5) |
| Chủ nhật | 20:30–21:30 | Tiếng Anh — Tổng hợp Ngày 7 (Weekly Synthesis Speaking Test) |
| Thứ Ba, Thứ Bảy | 19:00–20:30 | Nghỉ Daily Challenge |
| Chủ nhật | 12:00–20:30 | Nghỉ ngơi hoàn toàn / Thể thao / Gia đình |
| Hằng ngày | Sau 22:00 | Kết thúc lịch học, ngủ đủ giấc để bảo vệ trí nhớ dài hạn |

Các khoảng trống dành cho sinh hoạt, nghỉ và di chuyển. Nếu công việc thường xuyên tràn giờ nghỉ, giảm mini-case hoặc block tự học linh hoạt. Không xếp ca bù mặc định vào Chủ nhật.

## 3. Daily Challenge: ứng dụng và dạng bài thi

### 3.1. Mục tiêu

Mỗi bài giúp hiểu một phần workflow và thực hiện được bằng code. Phạm vi gồm **preprocessing, tabular ML, NLP, CV, audio, validation, metric, debugging, inference và nộp kết quả**. Bài lập trình ngắn dùng tình huống ứng dụng để rèn parsing, hàm, dict/set, sort/search, mảng và edge cases.

Ưu tiên một phần việc dùng được: kiểm tra dữ liệu, chống leakage, xây baseline, sửa pipeline, xuất đúng artifact hoặc tạo prototype nhỏ. Chỉ viết từ đầu bằng NumPy khi cần hiểu một phép tính; không bắt buộc thay thư viện chuẩn bằng code tự viết trong mọi bài ML.

Ba dạng sau là **format luyện tập thống nhất do tài liệu này đề xuất**. Mỗi đề ghi nguồn gốc; không gọi đề tự soạn là đề chính thức hoặc mô phỏng chính xác một edition chưa đối chiếu.

### 3.2. Ba format

| Format | Đầu vào/yêu cầu | Bàn giao | Kiểm tra |
| :--- | :--- | :--- | :--- |
| **A — Function / STDIN–STDOUT** | Contract hàm hoặc đọc/ghi, constraints, samples | Hàm/script/chương trình đúng interface | Sample, edge cases và test bổ sung |
| **B — Dataset / ML notebook** | Dữ liệu, schema, target, split, metric, giới hạn công cụ/compute | Notebook/script chạy lại được, dự đoán; model nếu đề yêu cầu; báo cáo ngắn | Artifact, validation, leakage và tái lập |
| **C — Prototype / Hackathon** | Tình huống, input/output, phạm vi tính năng và tài nguyên | Prototype tối thiểu, cách chạy, demo và giới hạn | Acceptance cases và rubric công bố trong đề |

Format A rèn phản xạ code. Format B rèn workflow dữ liệu/ML. Format C rèn tạo giải pháp và trình bày bằng chứng. Buổi ngắn chỉ làm một phần đủ nhỏ của B/C.

### 3.3. Các tham chiếu người học ưu tiên

| Tham chiếu | Cách áp dụng |
| :--- | :--- |
| Olympic AI sinh viên | ML/CV/NLP; validation, baseline, metric, code chạy lại và bàn giao. [Thể lệ OlpAI 2026](https://www.olp.vn/olympic-ai-sinh-vi%C3%AAn/quy-ch%E1%BA%BF-k%E1%BA%BF-ho%E1%BA%A1ch). |
| Kaggle | Chọn đề để luyện dataset → validation → model → metric → file nộp; schema lấy từ Data/Evaluation của đề đó. [Ví dụ trang cuộc thi](https://www.kaggle.com/competitions/titanic). |
| Zalo AI Challenge | Đọc development kit, dữ liệu, evaluation và yêu cầu code/model của track; không mặc định chỉ nộp CSV. [Trang chính thức](https://challenge.zalo.ai/). |
| AI Talented | Giữ là hướng tham chiếu theo tên người học nêu. Chưa có link/edition cụ thể nên chưa gán format hay quy chế chính thức; trước mắt luyện kỹ năng ứng dụng dùng chung. |
| Hackathon | Soạn bài theo format C; rubric, demo và phạm vi ghi trong đề, rồi đối chiếu event cụ thể khi có. |
| HackerRank | Function hoặc STDIN–STDOUT, constraints, sample và test. [Hướng dẫn soạn đề](https://support.hackerrank.com/articles/7816600113-defining-the-problem-details:-coding-and-database-engineer-questions). |

Nguồn đối chiếu ngày 05/11/2026; đây là tham chiếu học tập, không xác nhận đăng ký. Quy định AI, Internet, thư viện, dữ liệu và model lấy theo đề/edition được chọn. IOAI có thể là nguồn bài học thêm; điều kiện tham gia riêng cần đối chiếu [quy chế](https://ioai-official.org/wp-content/uploads/2026/05/IOAI2026-Contest-Rules.docx-1.pdf).

## 4. Format chuẩn cho từng đề

Template hiện hành: [DAILY_CHALLENGE_TEMPLATE.md](DAILY_CHALLENGES/ISSUED/_meta/DAILY_CHALLENGE_TEMPLATE.md).

1. **Challenge card:** ID, tên, chủ đề, format, mức hỗ trợ, thời lượng buổi và nguồn gốc.
2. **Scenario & objective:** vấn đề ứng dụng, vị trí trong workflow và mục tiêu.
3. **Task:** việc cần làm, phạm vi vừa buổi.
4. **Input / dataset:** vị trí dữ liệu, schema, kiểu, shape/đơn vị, nhãn và nguồn gốc.
5. **Output / submission:** interface/file, cột, thứ tự ID, kiểu và cách chạy.
6. **Constraints & resources:** quy mô, công cụ, dữ liệu/model được dùng, giới hạn tài nguyên đo được nếu cần.
7. **Samples:** input và expected output; sample submission ML ghi rõ chỉ minh họa định dạng.
8. **Verification / evaluation:** test/edge cases hoặc metric, split, baseline, acceptance criteria; ghi rõ nếu chưa có private judge.
9. **Your workflow:** người học tự đề xuất bước và lý do trước khi code.
10. **Hints & API references:** mở theo nhu cầu, không kèm lời giải nguyên bài.
11. **Deliverables & short review:** code/artifact, cách chạy, trợ giúp, một lựa chọn và một giới hạn.
12. **Follow-up variation:** thay đổi nhỏ để làm trong buổi ôn.

Đề bằng tiếng Anh; giải thích có thể bằng tiếng Việt. Buổi học thường được tra docs/hỏi gợi ý. Mô phỏng thi ghi riêng công cụ được phép. Không giảm gợi ý chỉ vì đã qua một số tuần.

## 5. Nhịp luyện và ngân hàng chủ đề

| Buổi | Cách luyện |
| :--- | :--- |
| Thứ Hai — A | Đọc đề, đề xuất workflow, triển khai một phần việc |
| Thứ Ba — ôn A | Sửa lỗi/thử biến thể; không bắt buộc viết lại toàn bộ |
| Thứ Năm — B | Vận dụng, debug hoặc kiểm tra bước tiếp theo |
| Thứ Sáu — ôn B | Edge cases, giải thích hoặc đổi điều kiện |
| Thứ Bảy — mini-case | Ghép bước đã luyện; được dùng để hoàn thiện A/B |
| Thứ Tư, Chủ nhật | Nghỉ Daily Challenge |

Mỗi tuần chọn một chủ đề chính. Có thể luân phiên preprocessing/validation → tabular → NLP → CV/audio → tổng hợp. Đây là thứ tự linh hoạt; được giữ một chủ đề qua nhiều tuần. Bài khó chia qua nhiều buổi hoặc thay block đã có, không cộng giáo trình ngoài ngân sách 4,5 giờ coding/tuần.

| Nhóm | Ví dụ challenge ứng dụng | Format |
| :--- | :--- | :--- |
| Preprocessing/EDA | Dữ liệu thiếu/trùng, manifest, schema/nhãn, feature engineering nhỏ | A/B |
| Validation/leakage | Group split, fit preprocessing trên train, tìm lỗi validation | A/B |
| Tabular ML | Baseline, metric lớp lệch, so sánh thay đổi có đối chứng | B |
| NLP | Chuẩn bị văn bản, biểu diễn, phân loại/tìm kiếm, phân tích lỗi | A/B |
| CV | Đọc ảnh, shape/kênh màu, preprocessing, baseline và inference | A/B |
| Audio | Metadata, độ dài/sample rate, đặc trưng và phân loại nhỏ | A/B |
| PyTorch/debug | Tensor/dtype, train/eval, training loop, lưu/nạp model | A/B |
| Phản xạ lập trình | Parsing, đếm/nhóm bằng dict/set, sort/search, chuỗi/mảng trong bài dữ liệu | A |
| Submission/tái lập | ID, shape/label, notebook chạy lại, inference budget | A/B |
| Prototype | Một luồng input → xử lý → output, acceptance cases và demo | C |

## 6. Bộ đề khởi động có dữ liệu

Thư mục: [DAILY_CHALLENGES](E:/TU_HOC/ROADMAP_STORAGE/DAILY_CHALLENGES/README.md). Đề **tự soạn**, dữ liệu tổng hợp; không đại diện độ khó hoặc điểm số thi thật.

| Buổi | Đề | Nội dung |
| :--- | :--- | :--- |
| A | [DC001 — Manifest Gatekeeper](E:/TU_HOC/ROADMAP_STORAGE/DAILY_CHALLENGES/DC001_MANIFEST_GATEKEEPER.md) | Hàm preprocessing: văn bản/nhãn, ID trùng, dữ liệu sạch và audit |
| Ôn A | Biến thể DC001 | Thứ tự bản ghi, đầu vào rỗng hoặc toàn bộ bị loại |
| B | [DC002 — Group Leakage Detector](E:/TU_HOC/ROADMAP_STORAGE/DAILY_CHALLENGES/DC002_GROUP_LEAKAGE.md) | STDIN–STDOUT: phát hiện group qua nhiều split; parsing, dict/set |
| Ôn B | Biến thể DC002 | Thêm dòng cùng group/split; kết quả có đổi không? |
| Mini-case | [DC003 — Ticket Classification](E:/TU_HOC/ROADMAP_STORAGE/DAILY_CHALLENGES/DC003_TICKET_CLASSIFICATION.md) | NLP baseline, split có sẵn, macro-F1 và submission |

DC001/DC002 dùng Python standard library; DC002 cũng có thể làm bằng C++. DC003 cần pandas/scikit-learn. Nếu môi trường hoặc kiến thức chưa phù hợp, dùng mini-case để hoàn thiện A/B và dời DC003 sang buổi sau; không thêm ca học.

DC003 có train sạch riêng, không phụ thuộc lời giải DC001. Dữ liệu nhỏ dùng để luyện quy trình; không đặt ngưỡng điểm để kết luận thành thạo NLP. Bộ đề có public fixtures; chưa có private judge.

## 7. Tiến bộ và điều chỉnh

Trong giờ review đã có, ghi ngắn phần tự làm, lỗi đã hiểu/sửa, trợ giúp và việc tràn giờ. Đánh giá riêng hiểu workflow, chọn cách làm và triển khai/kiểm tra.

Giảm gợi ý khi tự xử lý được biến thể và giải thích quyết định. Có thể cung cấp một dòng API, giải thích traceback hoặc ví dụ nhỏ ở tình huống khác; không đưa lời giải nguyên bài để chép. Không mặc định cung cấp workflow hoàn chỉnh trước khi người học thử đề xuất.

Nếu nhiều buổi tràn giờ hoặc khó hồi phục, giảm mini-case, ôn nhẹ hoặc bỏ buổi linh hoạt; không tạo nợ giờ. Mô phỏng thi dài hơn phải thay block đã có và dùng quy chế của kỳ thi cụ thể.
