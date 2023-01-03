/*
Projet minishell - Licence 3 Info - PSI 2022

Nom : DEHORS & LORMIER
Prénom : Pauline & Nicolas  
Num. étudiant : 22000055 & 21502501
Groupe de projet : Groupe N°8
Date : 09/10/2022

Gestion des commandes internes du minishell (implémentation).

*/

#include "builtin.h"

extern int errno;

// A modifié, pour rajouter les files descriptor;

int is_builtin(const char *cmd)
{
  int status = 0;
  if(strcmp("cd", cmd) == 0)
    status = 1;
  else if(strcmp("exit", cmd) == 0)
    status = 1;
  else if(strcmp("export", cmd) == 0)
    status = 1;

  return status;
}

int builtin(processus_t *proc)
{
  if(strcmp("cd", proc->path) == 0)
    cd(proc->argv[1], proc->out);
  else if(strcmp("exit", proc->path) == 0)
    exit_shell(EXIT_SUCCESS, proc->out);
  else if(strcmp("export", proc->path) == 0)
    export(proc->argv[0], proc->argv[1], proc->out);
  
  return EXIT_SUCCESS;
}

int cd(const char *path, int fderr)
{
  if((chdir(path)) != 0)
  {
    if(fderr != STDOUT_FILENO) {
      dup2(fderr, STDERR_FILENO);
      close(fderr);
    }

    char buffer[256] = "Impossible de se déplacer vers le dossier demandé.\n";
    write(STDERR_FILENO, buffer, strlen(buffer));
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}

int export(const char *var, const char *value, int fderr)
{
  char buffer[256];
  if(fderr != STDOUT_FILENO) {
    dup2(fderr, STDERR_FILENO);
    close(fderr);
  }

  if(setenv(var, value, 0) != 0)
  {
    strcpy(buffer, "Impossible de créer la variable d'environnement.\n");
    write(STDERR_FILENO, buffer, strlen(buffer));
    return EXIT_FAILURE;
  }
  strcpy(buffer, "la variable d'environnement à bien été ajouté.\n");
  write(STDERR_FILENO, buffer, strlen(buffer));
  return EXIT_SUCCESS;
}

int exit_shell(int ret, int fderr)
{
  exit(ret);
  return EXIT_SUCCESS;
}