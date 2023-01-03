/*
Projet minishell - Licence 3 Info - PSI 2022

Nom : DEHORS & LORMIER
Prénom : Pauline & Nicolas  
Num. étudiant : 22000055 & 21502501
Groupe de projet : Groupe N°8
Date : 09/10/2022

Gestion des commandes internes du minishell (implémentation).

*/

#ifndef _BUILTIN_H
#define _BUILTIN_H

#include "processus.h"

#include <string.h>
#include <stdio.h>

#include <stdlib.h>
#include <unistd.h>
#include <errno.h>

/*
 La chaîne passée représente-t-elle une commande
 interne ?
 
 cmd : chaîne de caractères.
 
 Retourne 1 si la chaîne passée désigne une
 commande interne (cd, exit, ...)
*/
int is_builtin(const char* cmd);

/*
  Exécute la commande interne
 
  proc : process_t dont le contenu a été initialisé
         au moment du parsing
 
  Retourne 0 si tout s'est bien passé, la valeur de
  retour de la commande ou -1 si la commande n'est
  pas reconnue.
*/
int builtin(processus_t* proc);

/*
  Change directory : change le répertoire de travail
  courant du minishell.

  path : le chemin vers lequel déplacer le CWD
  fderr : le descripteur de la sortie d'erreur pour
          affichage éventuel (erreur du cd)
  
  Retourne le code de retour de l'appel système.
*/
int cd(const char* path, int fderr);

/*
  Ajout/modification d'une variable d'environnement.
 
  var : nom de la variable
  value : valeur à lui donner
  fderr : le descripteur de la sortie d'erreur pour
          affichage éventuel
 
  Retourne le code de retour de l'appel système.
*/
int export(const char* var, const char* value, int fderr);

/*
  Quitter le minishell
 
  ret : code de retour du minishell
  fdout : descripteur de la sortie standard pour
          l'affichage éventuel d'un message de sortie
 
  Retourne un code d'erreur en cas d'échec.
*/
int exit_shell(int ret, int fderr);

#endif