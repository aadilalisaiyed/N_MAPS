import re
import emoji

def clean_message_bert(text):
    text = text.lower()
    text = re.sub(r"http\S+|www\S+", "", text)
    text = emoji.demojize(text)
    text = text.replace(":", " ").replace("_", " ")
    return re.sub(r"\s+", " ", text).strip()
