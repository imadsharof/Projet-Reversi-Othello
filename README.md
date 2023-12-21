# INFOH304 Projet

Groupe No 14
* François Nathan (@nfrancoi)
* SHAROF Imad (@isharof)
* ESIEV Kerim (@kesteves)

Le jeu de Reversi, connu sous le nom d’Othello dans sa version commerciale, est un jeu de
plateau opposant deux joueurs, nommés “noir” et “blanc” [wik(2017)]. Le plateau de jeu est constitué
d’une grille carrée de 64 cases (8 sur 8), sur lesquels les joueurs peuvent disposer des pions bicolores,
noirs d’un côté et blancs de l’autre. La position de départ du plateau est illustrée à la figure 1.
Figure 1 : Position de départ du jeu Reversi / Othello.
A partir de là, chaque joueur, en commençant par le noir, pose l’un après l’autre un pion de
sa couleur sur le plateau de sorte à capturer au moins un pion de la couleur de son adversaire.
La capture de pions survient lorsqu’un joueur place un de ses pions à l’extrémité d’un alignement
de pions adverses contigus et dont l’autre extrémité est déjà occupée par un de ses propres pions. Les
alignements considérés peuvent être une colonne, une ligne, ou une diagonale. Si le pion nouvellement
placé vient fermer plusieurs alignements, il capture tous les pions adverses des lignes ainsi fermées.
La capture se traduit par le retournement des pions capturés. Ces retournements n’entraînent pas
d’effet de capture en cascade : seul le pion nouvellement posé est pris en compte.
Lors de chaque tour, si un joueur ne peut pas capturer de pion adverse, quelle que soit la position
où il placerait son pion, il doit passer son tour. Si aucun des deux joueurs ne peut capturer de
nouveaux pions, la partie se termine. Le joueur ayant le plus grand nombre de pions de sa couleur
sur le plateau remporte alors la partie.

<img width="143" alt="image" src="https://github.com/imadsharof/Projet-Reversi-Othello/assets/100962495/18cdafa5-cd03-4aa5-b5e0-8fc21aea9cb2">


