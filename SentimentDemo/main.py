from nltk.sentiment.vader import SentimentIntensityAnalyzer


def get_compound_score(text):
    analyzer = SentimentIntensityAnalyzer()
    return analyzer.polarity_scores(text)["compound"]

def main():
  print(get_compound_score("Hello"))
  print(get_compound_score("No I disagree"))
  print (get_compound_score("I agree with that point"))
  print (get_compound_score("That point is really stupid"))
  print(get_compound_score("I hate you and I hate your kind"))

if __name__ == "__main__":
   main()



