import os, json, torch
from sklearn.preprocessing import LabelEncoder
from sklearn.model_selection import train_test_split
from transformers import (
    DistilBertTokenizerFast,
    DistilBertForSequenceClassification,
    Trainer,
    TrainingArguments
)

from preprocess import clean_message_bert
from data import generate_dataset

print("Generating data...")
data = generate_dataset(250)

texts = [clean_message_bert(x[0]) for x in data]
labels = [x[1] for x in data]

train_texts, val_texts, train_labels, val_labels = train_test_split(
    texts, labels, test_size=0.2, random_state=42
)

label_encoder = LabelEncoder()
label_encoder.fit(train_labels)

train_labels_enc = label_encoder.transform(train_labels)
val_labels_enc = label_encoder.transform(val_labels)

tokenizer = DistilBertTokenizerFast.from_pretrained("distilbert-base-uncased")

train_enc = tokenizer(train_texts, truncation=True, padding=True, return_tensors="pt")
val_enc = tokenizer(val_texts, truncation=True, padding=True, return_tensors="pt")

class DisasterDataset(torch.utils.data.Dataset):
    def __init__(self, enc, lbl):
        self.enc = enc
        self.lbl = lbl

    def __getitem__(self, idx):
        item = {k: v[idx] for k, v in self.enc.items()}
        item["labels"] = torch.tensor(self.lbl[idx])
        return item

    def __len__(self):
        return len(self.lbl)

train_ds = DisasterDataset(train_enc, train_labels_enc)
val_ds = DisasterDataset(val_enc, val_labels_enc)

model = DistilBertForSequenceClassification.from_pretrained(
    "distilbert-base-uncased",
    num_labels=len(label_encoder.classes_)
)

args = TrainingArguments(
    output_dir="results",
    num_train_epochs=4,
    per_device_train_batch_size=8,
    eval_strategy="epoch",
    save_strategy="no",
    report_to="none"
)

trainer = Trainer(
    model=model,
    args=args,
    train_dataset=train_ds,
    eval_dataset=val_ds
)

trainer.train()

# ---- SAVE MODEL ----
os.makedirs("saved_model", exist_ok=True)
model.save_pretrained("saved_model")
tokenizer.save_pretrained("saved_model")

with open("saved_model/labels.json", "w") as f:
    json.dump(label_encoder.classes_.tolist(), f)

print("Model trained & saved")
