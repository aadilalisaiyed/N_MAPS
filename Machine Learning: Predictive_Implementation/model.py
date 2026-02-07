from pathlib import Path
import pandas as pd
from sklearn.linear_model import LogisticRegression
from sklearn.pipeline import Pipeline
from sklearn.preprocessing import StandardScaler
from utils import extract_features

BASE_DIR = Path(__file__).resolve().parent
DATA_PATH = BASE_DIR.parent / "ml_server" / "data" / "data1.json"   # 👈 FIXED NAME

def train_models():
    df = pd.read_json(DATA_PATH)

    df["count"] = df["count"].astype(int)
    df["needs"] = df["needs"].fillna("")

    df["Police_Label"] = df["needs"].str.contains("Rescue", case=False).astype(int)
    df["Health_Label"] = df["needs"].str.contains("Medical", case=False).astype(int)
    df["Food_Label"]   = df["needs"].str.contains("Food", case=False).astype(int)

    X = extract_features(df)

    models = {}

    for target in ["Police_Label", "Health_Label", "Food_Label"]:
        y = df[target]

        model = Pipeline([
            ("scaler", StandardScaler()),
            ("lr", LogisticRegression())
        ])

        model.fit(X, y)
        models[target] = model

    return models
