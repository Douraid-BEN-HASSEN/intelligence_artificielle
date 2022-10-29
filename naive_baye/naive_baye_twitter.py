"""
# IMPORTATION & CLASSE
"""

# +--------+
# | CLASSE |
# +--------+
import json
import requests

import urllib3
from bs4 import BeautifulSoup
from nltk.tokenize import regexp_tokenize
import nltk
from nltk.tokenize import word_tokenize
from nltk.corpus import stopwords
from nltk.stem import WordNetLemmatizer

# retourne une liste de tweet en lien avec un hashtag
def getTweets(hashtag):
  url = 'https://api.twitter.com/2/tweets/search/recent?max_results=100&query=' + hashtag
  payload= {}
  headers = {
    'Authorization': 'X',
    'Cookie': 'X'
  }

  response = requests.request('GET', url, headers=headers, data=payload)
  
  return json.loads(response.text)['data']

def getTokens(sentences):
    nltk.download('punkt')
    return [word for word in regexp_tokenize(sentences,pattern='\w+')]

def getBagOfWords(tokens):
  bagOfWords = []
  for token in tokens:
    for word in token:
      if word not in bagOfWords:
        bagOfWords.append(word)
  return bagOfWords

"""# CODE

Récupération de tweets positifs (#Ecologie) et négatifs (#greenwashing)
"""

# getTweets
hashtagPos = 'Ecologie'
tweetsPos = getTweets(hashtagPos)

hashtagNeg = 'greenwashing'
tweetsNeg = getTweets(hashtagNeg)

sentencesPos = []
sentencesNeg = []

# save tweets
with open('tweetsPos.txt', 'w') as f:
    for tweet in tweetsPos:
        f.write(json.dumps(tweet))
        f.write(',\n')

with open('tweetsNeg.txt', 'w') as f:
    for tweet in tweetsNeg:
        f.write(json.dumps(tweet))
        f.write(',\n')

for tweet in tweetsPos:
  sentencesPos.append(tweet['text'].lower())

for tweet in tweetsNeg:
  sentencesNeg.append(tweet['text'].lower())

"""# NAIVE BAYE

### --- Preprocessing ---
"""

# --- Preprocessing ---
# nettoyage des datas
contentPos = ''
index = 0
for sentence in sentencesPos:
  sentence = sentence.replace('RT ', '').replace('rt ', '').replace('#', '')
  finalSentence = ''
  for token in sentence.split():
    if ('@' not in token) and ('http' not in token):
      finalSentence += token + ' '
  sentencesPos[index] = finalSentence
  contentPos += finalSentence + ' '
  index += 1

contentNeg = ''
index = 0
for sentence in sentencesNeg:
  sentence = sentence.replace('RT ', '').replace('rt ', '').replace('#', '')
  finalSentence = ''
  for token in sentence.split():
    if ('@' not in token) and ('http' not in token):
      finalSentence += token + ' '
  sentencesNeg[index] = finalSentence
  contentNeg += finalSentence + ' '
  index += 1

# bagOfWords
tokensPos = getTokens(contentPos)
tokensNeg = getTokens(contentNeg)

bagOfWords = getBagOfWords([tokensPos, tokensNeg])

print('bagOfWords =>', bagOfWords)

"""### --- Vectorization ---"""

# --- Vectorization ---
bagOfWordsDict = {}
for word in bagOfWords:
  bagOfWordsDict[word] = 0

bagOfWordsPosDict = bagOfWordsDict.copy()
bagOfWordsNegDict = bagOfWordsDict.copy()
nOccurencePos = 0
nOccurenceNeg = 0

vectors = []
for sentence in sentencesPos:
  v = bagOfWordsDict.copy()
  v['TYPE'] = sentence
  v['CLASS'] = '+'
  for word in bagOfWords:
    if word in sentence:
      v[word] += 1
      bagOfWordsDict[word] += 1
      bagOfWordsPosDict[word] += 1
      nOccurencePos += 1
  vectors.append(v)

for sentence in sentencesNeg:
  v = bagOfWordsDict.copy()
  v['TYPE'] = sentence
  v['CLASS'] = '-'
  for word in bagOfWords:
    if word in sentence:
      v[word] += 1
      bagOfWordsDict[word] += 1
      bagOfWordsNegDict[word] += 1
      nOccurenceNeg += 1
  vectors.append(v)

# affichage des 10 premiers vecteurs
for v in vectors[0:10]:
  print(v)

"""### --- Apprentissage ----"""

# --- Apprentissage ----
# Calcul des probalités pour les 2 classe (+ et -) de chaque mot
# probs = { word1: {pos: X, neg: X}, wordx: {pos: X, neg: X}.... }
probs = {}
nWord = len(bagOfWords)
for word in bagOfWords:
  probs[word] = { 'pos': (bagOfWordsPosDict[word]+1)/(nWord+nOccurencePos), 'neg': (bagOfWordsNegDict[word]+1)/(nWord+nOccurenceNeg) }

print(probs)

"""### --- Evaluation ----"""

# --- Evaluation ----
# test sur des anciennes données
def predict(sentence):
  pPos = len(sentencesPos) / (len(sentencesPos) + len(sentencesNeg))
  pNeg = len(sentencesNeg) / (len(sentencesPos) + len(sentencesNeg))

  scorePos = pPos
  scoreNeg = pNeg
  for word in sentence.split():
    if word in bagOfWords:
        scorePos *= probs[word]['pos']
        scoreNeg *= probs[word]['neg']

  if scorePos > scoreNeg:
    return '+'
  else:
    return '-'

print(predict(sentencesPos[0]), '=>', sentencesPos[0])
print(predict(sentencesNeg[0]), '=>', sentencesNeg[0])

# test sur des nouvelles données
# matrice confusion
dataset = []

evaluation = open("evaluation.txt", "r").read()
for n in range(0, len(evaluation.split('[SEPARATOR]'))):
  if n < 15:
    dataset.append({
            'sentence': evaluation.split('[SEPARATOR]')[n],
            'reel': '+',
            'prediction': ''
            })
  else:
    dataset.append({
            'sentence': evaluation.split('[SEPARATOR]')[n],
            'reel': '-',
            'prediction': ''
            })

confusion_matrix = [
    { 'pos': 0, 'neg': 0 },
    { 'pos': 0, 'neg': 0 }
]

for d in dataset:
  if d['reel'] == '+':
    result = predict(d['sentence'])
    if d['reel'] == result:
      confusion_matrix[0]['pos'] += 1
    else:
      confusion_matrix[0]['neg'] += 1
  else:
    result = predict(d['sentence'])
    if d['reel'] == result:
      confusion_matrix[1]['neg'] += 1
    else:
      confusion_matrix[1]['pos'] += 1

print('confusion_matrix =>', confusion_matrix)

# accuracy
accuracy = (confusion_matrix[0]['pos'] + confusion_matrix[1]['neg']) / 100

# precision
precision = (confusion_matrix[0]['pos'] / (confusion_matrix[0]['pos'] + confusion_matrix[0]['neg'] ))

# negative predictive value

#f1-score
f1_score = (confusion_matrix[0]['pos']/(confusion_matrix[0]['pos'] + ((confusion_matrix[0]['neg'] + confusion_matrix[1]['neg'])/2)))

print('accuracy =>', str(accuracy*100) + '%')
print('precision =>', str(precision*100) + '%')
print('f1_score =>', str(f1_score*100) + '%')

"""# Graphe"""

import networkx as nx
import matplotlib.pyplot as plt

tpositive = []
for tweet in tweetsPos:
  tpositive.append(tweet['id'])

tnegative = []
for tweet in tweetsNeg:
  tnegative.append(tweet['id'])
  
d = {'POS': tpositive[0:10], 'NEG': tnegative[0:10]}  # dict-of-dicts single edge (0,1)
G = nx.Graph(d)

subax1 = plt.subplot(121)
nx.draw(G, with_labels=True, font_weight='bold')

"""# Conclusion
La precision est relativement basse, l'accuracy & le score F1 aussi, mais ils sont assez proche.

Le réseau manque de données
"""
