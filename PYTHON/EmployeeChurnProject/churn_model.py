# Author: Maxime Pol Marcet.
# I define all functions required to train, evaluate and explain a machine learning model for employee churn prediction.

import pandas as pd
from sklearn.model_selection import train_test_split
from sklearn.ensemble import RandomForestClassifier
from sklearn.metrics import classification_report
import joblib
import matplotlib.pyplot as plt
import seaborn as sns

# I load the dataset from a CSV file.
def load_data(csv_path):
    data = pd.read_csv(csv_path)
    return data

# I preprocess the dataset by separating features and labels.
def preprocess_data(data):
    X = data.drop(columns=["EmployeeID", "LeftCompany"])
    y = data["LeftCompany"]
    return train_test_split(X, y, test_size=0.2, random_state=42)

# I train a Random Forest classifier.
def train_model(X_train, y_train):
    clf = RandomForestClassifier(n_estimators=100, random_state=42)
    clf.fit(X_train, y_train)
    return clf

# I evaluate the trained model using a classification report.
def evaluate_model(model, X_test, y_test):
    y_pred = model.predict(X_test)
    return classification_report(y_test, y_pred)

# I save the model to disk using joblib.
def save_model(model, path="churn_model.pkl"):
    joblib.dump(model, path)

# I load a saved model from disk.
def load_model(path="churn_model.pkl"):
    return joblib.load(path)

# I plot the top N most important features from the trained model.
def plot_feature_importance(model, feature_names, top_n=10):
    importances = model.feature_importances_
    indices = importances.argsort()[-top_n:][::-1]
    top_features = [feature_names[i] for i in indices]
    top_importances = importances[indices]

    plt.figure(figsize=(10, 6))
    sns.barplot(x=top_importances, y=top_features, palette="viridis")
    plt.title("Top Feature Importances")
    plt.xlabel("Importance")
    plt.ylabel("Feature")
    plt.tight_layout()
    plt.show()
