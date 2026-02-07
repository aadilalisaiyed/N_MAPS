from fastapi import FastAPI
from pydantic import BaseModel
from typing import List
import pandas as pd
from model import train_models
from utils import extract_features

app = FastAPI(title="Disaster ML Service")
models = train_models()

class RequestItem(BaseModel):
    count: int
    situation: str
    needs: str
    location: str

@app.post("/predict")
def predict(requests: List[RequestItem]):
    df = pd.DataFrame([r.dict() for r in requests])

    df["needs"] = df["needs"].fillna("")
    df["city"] = df["location"].apply(lambda x: x.split(",")[-1].strip())

    X = extract_features(df)

    df["Police_Action"] = (models["Police_Label"].predict_proba(X)[:,1] >= 0.6).astype(int)
    df["Health_Action"] = (models["Health_Label"].predict_proba(X)[:,1] >= 0.6).astype(int)
    df["Food_Action"]   = (models["Food_Label"].predict_proba(X)[:,1] >= 0.6).astype(int)

    summary = df.groupby("city").agg(
        Police_Need=("Police_Action","sum"),
        Health_Need=("Health_Action","sum"),
        Food_Need=("Food_Action","sum"),
        Total_People=("count","sum")
    ).reset_index()

    return summary.to_dict(orient="records")
