"""Learner entry point. Implement the CLI and inference contract in problem.md."""

import argparse
import csv
import json
from pathlib import Path


def main():
  # 1. Nhận tham số từ dòng lệnh (CLI)
  parser = argparse.ArgumentParser()
  parser.add_argument("--features", required=True)
  parser.add_argument("--model", required=True)
  parser.add_argument("--output", required=True)
  args = parser.parse_args()

  # 2. Đọc thông số mô hình từ model.json
  with open(args.model, "r", encoding="utf-8") as f:
    model = json.load(f)

  feature_names = model["feature_names"]
  classes = model["classes"]
  weights = model["weights"]
  bias = model["bias"]

  # 3. Đảm bảo thư mục cha của file output đã tồn tại trước khi mở file ghi
  output_path = Path(args.output)
  output_path.parent.mkdir(parents=True, exist_ok=True)

  # 4. Mở đồng thời file features để đọc và file output để ghi (Streaming O(1) RAM)
  with (
      open(args.features, "r", encoding="utf-8", newline="") as f_in,
      open(output_path, "w", encoding="utf-8", newline="") as f_out,
  ):
    reader = csv.DictReader(f_in)
    writer = csv.writer(f_out)

    # Ghi dòng tiêu đề bắt buộc
    writer.writerow(["sample_id", "label"])

    # Duyệt từng dòng dữ liệu cảm biến
    for row in reader:
      sample_id = row["sample_id"]

      # Lấy các giá trị đặc trưng THEO ĐÚNG THỨ TỰ của model.feature_names
      data_feature = [int(row[name]) for name in feature_names]

      # Danh sách lưu điểm của các class cho RIÊNG dòng hiện tại
      scores = []

      # Duyệt qua từng class i
      for i in range(len(classes)):
        # Khởi tạo điểm ban đầu của class i bằng đúng bias[i] (chỉ cộng 1 lần!)
        class_score = bias[i]

        # Cộng dồn tích (weight * feature) cho từng đặc trưng j
        for j in range(len(feature_names)):
          class_score += weights[i][j] * data_feature[j]

        scores.append(class_score)

      # Tìm điểm lớn nhất
      max_score = max(scores)

      # Hàm .index(max_score) tự động trả về vị trí ĐẦU TIÊN đạt max_score
      # (Giải quyết trọn vẹn quy tắc hòa điểm: ưu tiên class xuất hiện trước!)
      best_class_index = scores.index(max_score)
      best_label = classes[best_class_index]

      # Ghi kết quả của dòng này ra file output ngay lập tức
      writer.writerow([sample_id, best_label])


if __name__ == "__main__":
  main()