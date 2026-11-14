"""Learner entry point. Implement the CLI and inference contract in problem.md."""
# things i have learned: using json.load() with its parameter need to be a pointer not a str
import json
import csv
import argparse
from pathlib import Path
parser = argparse.ArgumentParser()
parser.add_argument("--features", required=True)
parser.add_argument("--model", required=True)
parser.add_argument("--output", required=True)
args = parser.parse_args()
def main():
    with open(args.model, "r", encoding="utf-8") as f:
        model = json.load(f)
    feature_name = model["feature_names"]
    parameter_bias = {"weights" : model["weights"], "bias": model["bias"]}
    score = []
    with open(args.features, "r", encoding="utf-8") as f:
        reader = csv.DictReader(f)
        for row in reader:
            sample_id = row["sample_id"]
            data_feature = [int(row["mean_abs_accel"]), int(row["peak_count"]), int(row["gyro_energy"])]
            for i in range(len(model["classes"])):
                sum = 0
                for j in range(len(feature_name)):
                    sum += parameter_bias["bias"][i] + sum(parameter_bias["weights"][i][j] * data_feature[j])
                score.append(sum)     
            max_Score = max(score)
            
if __name__ == "__main__":
    main()
