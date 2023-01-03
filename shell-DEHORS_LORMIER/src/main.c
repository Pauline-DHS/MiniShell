/*
Projet minishell - Licence 3 Info - PSI 2022

Nom : DEHORS & LORMIER
Prénom : Pauline & Nicolas  
Num. étudiant : 22000055 & 21502501
Groupe de projet : Groupe N°8
Date : 15/10/2022

Interface du mini shell.

*/

#include <stdio.h>
#include <stdlib.h>

#include "parser.h"
#include "processus.h"
#include "builtin.h"

int main(int argc, char* argv[]) {
  char cmdline[MAX_LINE_SIZE]; // buffer des lignes de commandes
  char* cmdtoks[MAX_CMD_SIZE]; // "mots" de la ligne de commandes
  char currentDir[255];
  processus_t processes[MAX_CMD_SIZE];
  processus_t* current;
  
  while(1)
  {
    // Effacer les contenus de cmdline, cmdtoks et processes
    memset(cmdline, '\0', sizeof(char) * MAX_LINE_SIZE); // efface le bloc mémoire

    for(int i = 0; i < MAX_CMD_SIZE; i++){
      cmdtoks[i] = NULL;
    }

    // Initialiser les valeurs par défaut dans processes (stdin, stdout, stderr, ...)
    for(unsigned int i = 0; i < MAX_CMD_SIZE; i++)
      init_process(processes + i);

    // Afficher le dossier courant et le lancement de la ligne de commande
    printf("\033[92m");
    fprintf(stdout, "minishell: ");
    printf("\033[94m");
    fprintf(stdout, "%s: $ ", getcwd(currentDir, 255));
    printf("\033[0m"); 

    // Lire une ligne dans cmdline - Attention fgets enregistre le \n final
    if (fgets(cmdline, MAX_LINE_SIZE, stdin)==NULL) break;
    cmdline[strlen(cmdline)-1]='\0';

    // Traiter la ligne de commande
    //   - supprimer les espaces en début et en fin de ligne	==> fonction trim
    trim(cmdline);

    //   - ajouter d'éventuels espaces autour de ; ! || && & ...	==> fonction separate
    separate_s(cmdline,";+?",MAX_LINE_SIZE);

    //   - supprimer les doublons d'espaces		==> fonction clean 
    clean(cmdline);

    //   - traiter les variables d'environnement		==> fonction substenv
    substenv(cmdline,MAX_LINE_SIZE);

    // Découper la ligne dans cmdtoks ==> fonction strcut 
    strcut(cmdline,' ',cmdtoks,MAX_CMD_SIZE);

    // Transformer le tableau de chaine d'instruction en processus_t
    parse_cmd(cmdtoks, processes, MAX_CMD_SIZE);

    // Executer l'ensemble des processus
    for(current = processes; current != NULL;)
    {
      // Tester si la commande est une commande builtin ou non
      if(is_builtin(current->path))
      {
        // Si oui, on lance la fonction interne du shell dans le processus courant
        builtin(current);
      } else {
        exec_process(current);
      }

      if(current->next != NULL)
      {
        current = current->next;
        continue;
      }
      if(current->next_success != NULL)
      {
        current = current->next_success;
        continue;
      }
      // Est-ce que le processus suivant se lance si le premier n'est pas bon et le signe est un && ?
      if(current->next_failure != NULL)
      {
        current = current->next_failure;
      } else {
        current = NULL;
      }
    }
  }

  exit_shell(0, 1);
  fprintf(stdout, "\nGood bye!\n");
  return 0;
}
