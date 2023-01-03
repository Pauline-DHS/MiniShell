/*
	Projet minishell - Licence 3 Info - PSI 2022

	Nom : DEHORS & LORMIER
	Prénom : Pauline & Nicolas
	Num. étudiant : 22000055 & 21502501
	Groupe de projet : Groupe N°8
	Date : 13/10/2022

	Parsing de la ligne de commandes utilisateur.

*/

#ifndef _PARSER_H
#define _PARSER_H

#define MAX_LINE_SIZE   1024
#define MAX_CMD_SIZE    256
#include <stddef.h>
#include <stdio.h>
#include <string.h>


int trim(char* str);
int clean(char* str);
int separate_s(char* str, char* s, size_t max);
int substenv(char* str, size_t max);
int strcut(char* str, char sep, char** tokens, size_t max);

// Optionnel
//int strcut_s(char* str, char sep, char** tokens, size_t max);
#endif
