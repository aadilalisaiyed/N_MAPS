import json, torch
import torch.nn.functional as F
from transformers import DistilBertTokenizerFast, DistilBertForSequenceClassification
from preprocess import clean_message_bert

tokenizer = DistilBertTokenizerFast.from_pretrained("saved_model")
model = DistilBertForSequenceClassification.from_pretrained("saved_model")
model.eval()

with open("saved_model/labels.json") as f:
    LABELS = json.load(f)

msg = "people trapped under debris"
text = clean_message_bert(msg)

inputs = tokenizer(text, return_tensors="pt", truncation=True, padding=True)
with torch.no_grad():
    out = model(**inputs)

probs = F.softmax(out.logits, dim=1)[0]
print("Prediction:", LABELS[probs.argmax().item()])
