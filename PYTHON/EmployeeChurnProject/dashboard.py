# Author: Maxime Pol Marcet.
# I provide a Streamlit dashboard for uploading data, running predictions and visualizing churn risk and logs.

import streamlit as st
import pandas as pd
from churn_model import load_model

# I configure the Streamlit app layout and title.
st.set_page_config(page_title="Churn Risk Dashboard", layout="wide")
st.title("Employee Churn Risk Prediction")

# I define the navigation sidebar for selecting the view.
menu = st.sidebar.selectbox("Select a page", ["Prediction Dashboard", "Training Log"])

# I display the dashboard to upload employee data and get churn predictions.
if menu == "Prediction Dashboard":
    uploaded_file = st.file_uploader("Upload Employee Data (CSV)", type="csv")

    if uploaded_file:
        df = pd.read_csv(uploaded_file)
        model = load_model()
        X = df.drop(columns=["EmployeeID"])
        probs = model.predict_proba(X)[:, 1]

        df["ChurnRiskScore"] = probs
        df["RiskCategory"] = pd.cut(probs, bins=[0, 0.33, 0.66, 1.0], labels=["Low", "Medium", "High"])

        st.dataframe(df[["EmployeeID", "ChurnRiskScore", "RiskCategory"]].sort_values(by="ChurnRiskScore", ascending=False))
        st.bar_chart(df["RiskCategory"].value_counts())

# I display the log file with previous model training runs.
elif menu == "Training Log":
    st.subheader("Model Training History")
    try:
        with open("training_log.txt", "r") as file:
            logs = file.readlines()
        if logs:
            st.text("\n".join(logs))
        else:
            st.info("The training log is currently empty.")
    except FileNotFoundError:
        st.warning("No training log found. Run a model training session first.")
