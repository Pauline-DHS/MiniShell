# MiniShell

# 🎯 Objectif de ce projet :

Ce projet consiste en la réalisation d’un shell Unix très élémentaire, permettant au minimum :
— Les commandes intégrées $ cd [dir]
$ exit [n]
— D’exécuter des commandes simple, en "avant plan" et en "arrière plan"
$ commande
$ commande &
$ commande1 ; commande2 ; commande3 & commande4 ; ...
— De rediriger les entrées sorties des commandes
$ commande > file.output
$ commande 2> file.error
$ commande < file.input
$ commande >> file.appout
$ commande 2>> file.apperr
$ commande >&2
$ commande 2>&1
$ commande1 | commande2 | commande3 | ...
— De la composition de toutes ces redirections
— De gérer les variables d’environnement
$ export VAR="ma variable"
$ echo $VAR
$ unset VAR
...
— De gérer les opérateurs !, && et || comme les gère le bash

Ce projet a été réaliser en binôme dans le cadre d'un projet universitaire en 2022.
