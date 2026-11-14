# Weekly topic blocks — cấu hình 09/11/2026

**Lịch:** T2/T4/T6 07:00 Asia/Saigon; 60–75 phút/bài; T5/T7 review theo bài thực tế; không đề mới T3/CN. Một chủ đề mỗi tuần. Bảng dưới là thứ tự dự kiến, **không phải bằng chứng thành thạo hay các đề đã phát trước**. Điều chỉnh/giữ tuần theo bài nộp, không tự tăng độ khó hoặc tạo backlog.

| Tuần bắt đầu | Chủ đề duy nhất dự kiến | Module / đầu ra kiểm tra được |
| :--- | :--- | :--- |
| 09/11/2026 | CryCare: ring buffer audio có capacity cố định | 09/11 là bài Python metrics đã phát trước đổi cấu hình; giữ lịch sử. 11/11 dự kiến C++ host ring buffer khởi điểm; 13/11 biến thể overflow/wrap-around dựa trên phản hồi, không giả định đã làm bài 11/11. |
| 16/11/2026 | Buffer ownership và producer/consumer | Lifetime, frame boundaries, mô hình concurrency; host tests rồi adapter task khi đủ điều kiện. |
| 23/11/2026 | ESP-IDF/FreeRTOS audio task pipeline | Queue/mutex/timeout/watchdog; scaffold đúng SDK xác nhận; host mock trước nếu chưa có board/SDK. |
| 30/11/2026 | Audio energy gate | PCM/RMS/dBFS trước padding, silence/short-window parity, giới hạn số học. |
| 07/12/2026 | DSP -> TinyML input contract | Shape/layout/features và quantization input; parity fixtures, không viết lại cả pipeline trong một buổi. |
| 14/12/2026 | ESP-NOW packet và delivery | Serialization/CRC variant/test vector, ACK ứng dụng, retry/dedup; mock loss trước HIL. |
| 21/12/2026 | C3 alarm control | Debounce FSM, xác nhận người dùng, escalation 120s; gắn DSA/state machine vào cùng module, không đổi sang tuần CV. |
| 28/12/2026 | Sự kiện và app/backend boundary | event_id, JSON schema, offline queue/retry và mock API; không ghi database thật. |
| 04/01/2027 | Tích hợp và fault injection CryCare | E2E replay, mất gói/mất mạng, resource evidence, chuẩn bị hạn 10/01. |

**Từ 11/01/2027:** chọn cả tuần cho một khối: (A) DSA/Nhập môn AI theo đề cương; (B) Python CV/PyTorch, calibration INT8 + C++ khi phù hợp cho EDGE-QUANT Pi 5; (C) workflow thi AI theo nguồn cụ thể. Không ấn định vòng quay cứng khi chưa có deadline/bài nộp. Từ 21/12–10/01 vẫn ưu tiên CryCare.

Mỗi đề mới ghi `weekly_block`, `module`, `depends_on` (nếu có bằng chứng), `session_minutes`, profile, acceptance tests và phần learner tự làm. Nếu bài trước chưa nộp, bài sau dùng cùng chủ đề nhưng có đầu vào/scaffold độc lập đủ bắt đầu.

Review T5/T7: tham chiếu đề cũ, ghi vào `ISSUED/YYYY-MM-DD/REVIEW.md` khi có hoạt động; ghi rõ lỗi/ý hiểu đã kiểm chứng và phần chưa biết, không tự đổi trạng thái từ issued sang reviewed vì đến ngày review. Không tự tạo automation review.
