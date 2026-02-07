import random

HELP_TYPES = {
    "medical": [
        "bleeding", "injured", "unconscious", "broken leg",
        "severe pain", "blood loss", "can't breathe"
    ],
    "rescue": [
        "trapped", "stuck", "under debris", "collapsed building",
        "cannot move", "buried", "blocked exit"
    ],
    "fire": [
        "fire", "smoke", "burning", "flames",
        "gas leak", "explosion", "heat unbearable"
    ],
    "relief": [
        "no food", "need water", "hungry",
        "no shelter", "cold night", "no supplies"
    ]
}

TEMPLATES = [
    "help i am {}",
    "urgent {}",
    "please {}",
    "need help {}",
    "{} please help"
]

EMOTIONS = ["", " !!!", " 😭", " urgently"]

def generate_dataset(samples_per_class=250):
    data = []
    for label, words in HELP_TYPES.items():
        for _ in range(samples_per_class):
            msg = random.choice(TEMPLATES).format(random.choice(words))
            msg += random.choice(EMOTIONS)
            data.append((msg, label))
    random.shuffle(data)
    return data
