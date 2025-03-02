from nltk.sentiment.vader import SentimentIntensityAnalyzer

def get_compound_score(text):
    analyzer = SentimentIntensityAnalyzer()
    return analyzer.polarity_scores(text)["compound"]
