/*
Projet minishell - Licence 3 Info - PSI 2022

Nom : DEHORS & LORMIER
Prénom : Pauline & Nicolas  
Num. étudiant : 22000055 & 21502501
Groupe de projet : Groupe N°8
Date : 15/10/2022

Gestion des processus (implémentation).

*/

#include "processus.h"

int exec_process(processus_t* p) {
    if(strcmp(p->argv[0], "\0") != 0)
    {
      // On lance un nouveau processus
      p->pid = fork();
      int id = p->pid;

      // Processus fils
      if(p->pid == 0)
      {      
        // Préparation de la chaîne pour lancer la commande
        char* filepath = (char*) malloc(sizeof(char) * (strlen("/bin/") + strlen(p->argv[0])));
        strcat(filepath, "/bin/");
        strcat(filepath, p->argv[0]);

        if(p->out != STDOUT_FILENO) {
          dup2(p->out, STDOUT_FILENO);
          close(p->out);
        }
        
        if(p->in != STDIN_FILENO) {
          dup2(p->in, STDIN_FILENO);
          fcntl(p->in, F_SETFL, O_NONBLOCK);
          close(p->in);
        }

        // Lancement de la commande - A voir si execvp lance un sous-process
        execvp(filepath, p->argv);

        // Libérer la mémoire de la chaîne créée
        free(filepath);
      }
      // Processus père
      else
      {
        // Variable pour stocker le pid à attendre
        pid_t wait_this_pid;

        // Variable pour stocker le statut du processus fils
        int status;

        // Si le programme doit attendre l'ancien processus
        if(p->wait == 1)
        {
          do {
            wait_this_pid = waitpid(id, &status, 0);
            //Gestion des statuts de sortie du processus fils
              if(WIFEXITED(status)) {
              return EXIT_SUCCESS;
             } else if (WIFSIGNALED(status)) {
              return EXIT_SUCCESS;
             } else if (WIFSTOPPED(status)) {
              return EXIT_SUCCESS;
             } else if (WIFCONTINUED(status)) {
              return EXIT_SUCCESS;
             }
          } while (!WIFEXITED(status) && !WIFSIGNALED(status));
        }
      }
    }
  return EXIT_SUCCESS;
}

int init_process(processus_t* p) {
  p->pid = -1;
  p->in = STDIN_FILENO;
  p->out = STDOUT_FILENO;
  p->err = STDERR_FILENO;
  p->status = 0;
  p->wait = 1;
  p->path = NULL;

  for (size_t i = 0; i < MAX_CMD_SIZE; i++)
  {
    p->argv[i] = NULL;
  }
  
  p->next = NULL;
  p->next_failure = NULL;
  p->next_success = NULL;

  return EXIT_SUCCESS;
}

int parse_cmd(char* tokens[], processus_t* processes, size_t max) {
  unsigned compteurProcess = 0;
  unsigned compteurToken = 0;
  unsigned compteurArgv = 0;
  char* verif = (char*) malloc(sizeof(char) * 256);
  
  // Tant que ce n'est pas la fin de la commande
  while(tokens[compteurToken] != NULL && compteurToken < max)
  {
    processes[compteurProcess].path = tokens[compteurToken];

    do{
      processes[compteurProcess].argv[compteurArgv] = tokens[compteurToken];
      compteurToken++;
      compteurArgv++;
      if(tokens[compteurToken] != NULL) {
        strcpy(verif, tokens[compteurToken]);
      }
    }
    // Tant qu'on est toujours sur le même processus 
    while(strcmp(verif, "|") != 0 && strcmp(verif, "||") != 0 && strcmp(verif,"&&") != 0 && strcmp(verif,"&") != 0 && strcmp(verif, "&>") != 0 && strcmp(verif, ">") != 0 && strcmp(verif, ">>") != 0 && strcmp(verif, "<&") != 0 && strcmp(verif, "<") != 0 && strcmp(verif, "<<") != 0 && tokens[compteurToken] != NULL);

    // Définir si on attend la fin du process ou non
    if (strcmp(verif, "&") == 0)
    {
      processes[compteurProcess].wait = 0;
    }

    if (strcmp(verif, "&>") == 0)
    {
      compteurToken++;
      int fd = atoi(tokens[compteurToken]);
      processes[compteurProcess].out = fd;
    }

    if (strcmp(verif, "<&") == 0)
    {
      compteurToken++;
      int fd = atoi(tokens[compteurToken]);
      processes[compteurProcess].in = fd;
    }

    if (strcmp(verif, ">") == 0)
    {
      compteurToken++;
      processes[compteurProcess].out = open(tokens[compteurToken], O_WRONLY | O_TRUNC | O_CREAT, 0644);
    }

    if (strcmp(verif, ">>") == 0)
    {
      compteurToken++;
      processes[compteurProcess].out = open(tokens[compteurToken], O_WRONLY | O_APPEND | O_CREAT, 0644);
    }

    if (strcmp(verif, "<") == 0)
    {
      compteurToken++;
      processes[compteurProcess].in = open(tokens[compteurToken], O_RDONLY);
    }

    if (strcmp(verif, "<<") == 0)
    {
      // On décale le compteur pour récup la clé
      compteurToken++;
      char key[MAX_CMD_SIZE];
      strcpy(key, tokens[compteurToken]);

      // Buffer pour stocker les entrées utilisateurs
      char buffer[MAX_LINE_SIZE];

      int file = open(tokens[compteurToken], O_WRONLY | O_TRUNC | O_CREAT, 0644);

      if (file == -1) {
          fprintf(stderr, "Erreur\n");
            return 1;
      }
  
      char key_with_return[MAX_CMD_SIZE];
      strcpy(key_with_return, key);
      strcat(key_with_return, "\n");

      while(strcmp(buffer, key_with_return) != 0)
      {
        fgets(buffer, MAX_LINE_SIZE, stdin);
        if(strcmp(buffer, key_with_return) != 0)
        {
          write(file, buffer, strlen(buffer));
        }
      }

      close(file);
      processes[compteurProcess].in = open(tokens[compteurToken], O_RDONLY);

      remove(tokens[compteurToken]);
    }

    // Définir la suite de l'execution des processus
    if(strcmp(verif, "&&") == 0)  
    {
      processes[compteurProcess].next_success = processes + compteurProcess+1;
    }
    
    if (strcmp(verif, "|") == 0)
    {
      // On créer le tube
      int tube[2];
      pipe(tube);
      
      // Les envoyer en tant que file descriptor dans les processus
      processes[compteurProcess].out = tube[1];
      processes[compteurProcess + 1].in = tube[0];

      processes[compteurProcess].next = processes + compteurProcess+1;
    }

    if (strcmp(verif, "||") == 0)
    {
      processes[compteurProcess].next_failure = processes + compteurProcess+1;
    }

    // Passer au processus suivant
    memset(verif, '\0', sizeof(char) * 256);
    compteurProcess++;
    compteurToken++;
    compteurArgv = 0;
  } 

  free(verif);

  return 0;
}