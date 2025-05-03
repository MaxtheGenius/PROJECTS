# Author: Maxime Pol Marcet.
# I train a machine learning model to predict employee churn and save evaluation outputs.

from churn_model import load_data, preprocess_data, train_model, evaluate_model, save_model, plot_feature_importance
import os
from datetime import datetime

# I define the path to the input dataset.
DATA_PATH = "sample_data.csv"

if not os.path.exists(DATA_PATH):
    raise FileNotFoundError(f"{DATA_PATH} not found. Please provide the dataset.")

# I load and preprocess the dataset.
data = load_data(DATA_PATH)
X_train, X_test, y_train, y_test = preprocess_data(data)

# I train the model.
model = train_model(X_train, y_train)

# I evaluate the trained model.
report = evaluate_model(model, X_test, y_test)

print("Model Evaluation Report:")
print(report)

# I generate and display the feature importance chart.
plot_feature_importance(model, X_train.columns)

# I prepare folders and generate file names with timestamps.
current_time = datetime.now().strftime("%d-%m-%Y at %H:%Mh")
os.makedirs("reports", exist_ok=True)
os.makedirs("models", exist_ok=True)
filename = f"reports/evaluation_report_{current_time.replace(' at ', '_').replace(':', '-')}.txt"
model_filename = f"models/churn_model_{current_time.replace(' at ', '_').replace(':', '-')}.pkl"

# I write the evaluation report to a text file.
with open(filename, "w") as f:
    f.write(f"Model Evaluation Report - {current_time}\n")
    f.write(report)
    print(f"Report saved to {filename}.")

# I save the trained model to a file.
save_model(model, path=model_filename)
print(f"Model saved to {model_filename}.")

# I append a log entry for this training run.
with open("training_log.txt", "a") as log:
    log.write(f"[{current_time}] Report: {filename}, Model: {model_filename}\n")
