# TP Agents intelligents

## Exercie 1 :

#### Résultats :
![1](img/1.jpg)

#### Courbe d’apprentissage :
![2](img/2.jpg)

![3](img/3.jpg)

*On peut voir que pour pendant la phase d’apprentissage, le réseau fait beaucoup d’erreurs et au fils des itérations ça diminue car le réseau apprend et ne se trompe plus*

#### Généralisation :
![4](img/4.jpg)

*On peut voir que sur un % de bruit très faible, le réseau arrive quand même à trouver le bon résultat :*

![5](img/5.jpg)

*Ensuite, on peut voir que le nombre d’erreurs est croissant car il n’a pas été suffisamment entrainé*

## Exercie 2 :
![6](img/6.jpg)

*Pour la seconde étape, j’ai entrainé le réseau sur beaucoup plus d’itérations*
*Cette courbe représente une descente de gradient, plus on avance, moins le gain est important.*
*On peut continuer sur plus d’itération, mais le temps de calcul n’est pas intéressant pour le gain, j’ai donc stoppé à un palier précis (0.0001)*

![7](img/7.jpg)

*La courbe de généralisation*
*Aperçu approximative de l’hyperplan :*

![8](img/8.jpg)

*Les distances sont équidistantes, ce qui fait que le nombre d’erreur pour 0 & 1 sont quasiment les mêmes et la confusion entre les chiffres est réduite (ne confond pas un « 1 » pour un « 0 » et inversement)*


## Exercie 3 :
#### Apprentissage :

![9](img/9.jpg)

#### Généralisation :

![10](img/10.jpg)

*Les courbes sont bien séparées, le réseau de neurone est capable de différentier les différents chiffres.*
