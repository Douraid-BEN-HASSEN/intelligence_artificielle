# Introduction
Implémentation de l'algorithme **NAIVE BAYE** dans le cadre d'un traitement de tweets. L'objectif est de déterminer si un tweet est classé comme 'positif' ou 'négatif' vis a vis du sujet de l'écologie.

![1](img/intro.JPG)

### --- Preprocessing ---
![1](img/1.PNG)

### --- Vectorization ---
![2](img/2.PNG)

### --- Apprentissage ----
![3](img/3.PNG)

### --- Evaluation ----
![4](img/4.1.PNG)
![5](img/4.2.PNG)

# Graphe
Ce graphe représente les tweets positifs qui ont été aimés par des utilisateurs qui ont aimés des tweets négatifs et inversement.
Il permet de mettre en évidence les évuentels liens entre les 2 classes.

![6](img/5.png)

# Conclusion
La precision est relativement basse, l'accuracy & le score F1 aussi, mais ils sont assez proche.

Le réseau manque de données
