/*
Projet minishell - Licence 3 Info - PSI 2022

Nom : DEHORS & LORMIER
Prénom : Pauline & Nicolas  
Num. étudiant : 22000055 & 21502501
Groupe de projet : Groupe N°8
Date : 13/10/2022

Gestion des processus.

*/

#ifndef _PROCESSUS_H
#define _PROCESSUS_H

#include <string.h>
#include <stdio.h>

#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>

#include <sys/types.h>
#include <sys/wait.h>

#include "parser.h"

/*
  Structure de représentation d'un processus à lancer.

  pid: 
  status:
  stdin, stdout, stderr:
  wait:
  fdclose:
  next:
  next_success:
  next_failure:
*/

typedef struct processus_t {
  pid_t pid;
  int status;
  int in, out, err;
  int wait;
  char* path;
  char* argv[MAX_CMD_SIZE];
  int fdclose[MAX_CMD_SIZE];
  struct processus_t* next;
  struct processus_t* next_success;
  struct processus_t* next_failure;
} processus_t;

/*
  Lancer la commande en fonction des attributs de
  la structure et initialiser les champs manquants.
 
  On crée un nouveau processus, on détourne
  éventuellement les entrée/sorties.
  On conserve le PID dans la structure du processus
  appelant (le minishell).
  On attend si la commande est lancée en "avant-plan"
  et on initialise le code de retour.
  On rend la main immédiatement sinon.
 
  p : un pointeur sur la structure contenant les
      informations pour l'exécution de la commande.
 
  Retourne 0 ou un code d'erreur.
 */

int exec_process(processus_t* p);

/*
  Initialiser une structure process_t avec les
  valeurs par défaut.
 
  p : un pointeur sur la structure à initialiser
 
  Retourne 0 ou un code d'erreur.
*/
int init_process(processus_t* p);

/*
  Remplit le tableau de commandes en fonction du contenu
  de tokens.
    Ex : {"ls", "-l", "|", "grep", "^a", NULL} =>
         {{path = "ls",
           argv = {"ls", "-l", NULL},
           bg = 1,
           ...},
          {path = "grep",
           argv = {"grep", "^a", NULL},
           bg = 0,
           ...}}
  tokens : le tableau des éléments de la ligne de
           commandes (peut être modifié)
  processes : le tableau dans lequel sont stockés les
              différentes structures représentant le
              commandes.
  max : le nombre maximum de commandes dans le tableau

  Retourne 0 ou un code d'erreur.
 */
int parse_cmd(char* tokens[], processus_t* processes, size_t max);

#endif
