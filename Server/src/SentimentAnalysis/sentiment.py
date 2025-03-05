from vaderSentiment.vaderSentiment import SentimentIntensityAnalyzer
import sys

def analyze_sentiment(text):
    analyzer = SentimentIntensityAnalyzer()
    
    # Custom Words
    analyzer.lexicon.update({
        "suck": -3.0,
        "terrible": -3.5,
        "awful": -4.0,
        "stupid": -3.0,
        "hate": -4.0
    })
    
    scores = analyzer.polarity_scores(text)
    if scores['compound'] < -0.5:
        print("negative")
    elif scores['compound'] > 0.5:
        print("positive")
    else:
        print("neutral")

if __name__ == "__main__":
    analyze_sentiment(sys.argv[1])
