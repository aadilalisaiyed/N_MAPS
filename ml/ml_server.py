from fastapi import FastAPI
from pydantic import BaseModel
import json, torch
import torch.nn.functional as F
from transformers import DistilBertTokenizerFast, DistilBertForSequenceClassification
from preprocess import clean_message_bert

app = FastAPI()

tokenizer = DistilBertTokenizerFast.from_pretrained("saved_model")
model = DistilBertForSequenceClassification.from_pretrained("saved_model")
model.eval()

with open("saved_model/labels.json") as f:
    LABELS = json.load(f)

class Input(BaseModel):
    message: str

@app.post("/predict")
def predict(data: Input):
    text = clean_message_bert(data.message)
    inputs = tokenizer(text, return_tensors="pt", truncation=True, padding=True)

    with torch.no_grad():
        out = model(**inputs)

    probs = F.softmax(out.logits, dim=1)[0]
    idx = probs.argmax().item()

    return {
        "prediction": LABELS[idx],
        "confidence": round(probs[idx].item(), 3)
    }
