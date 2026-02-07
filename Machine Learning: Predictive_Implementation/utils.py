def extract_features(df):
    text = df["situation"].str.lower()

    df["kw_trapped"] = text.str.contains("trapped|stuck|flood|water").astype(int)
    df["kw_injury"]  = text.str.contains("injur|bleed|fever|pain|asthma").astype(int)
    df["kw_hungry"]  = text.str.contains("food|hungry|no food|water shortage").astype(int)
    df["kw_safe"]    = text.str.contains("safe|okay|fine").astype(int)

    X = df[[
        "count",
        "kw_trapped",
        "kw_injury",
        "kw_hungry",
        "kw_safe"
    ]]

    return X
