/*
	Projet minishell - Licence 3 Info - PSI 2022

	Nom : DEHORS & LORMIER
	Prénom : Pauline & Nicolas
	Num. étudiant : 22000055 & 21502501
	Groupe de projet : Groupe N°8
	Date : 13/10/2022

  Parsing de la ligne de commandes utilisateur (implémentation).
 
 */

#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>	

int trim(char* str) {
	int debut=0;
	int i=0;
	int k = strlen(str)-1;
	char* copie_str = strdup(str); 	// copie la str dans copie_str
	memset(str,0,strlen(str)); 		// vide la chaine str 

	
	/*Dans cette première boucle while on va tester les premiers caractères de notre chaîne 'copie_str' et ainsi trouver l'indice de 
	premier caractère qui n'est pas un esapce => dans le but de supprimer les espaces en début de chaine*/
	while(copie_str[i] == ' '){		// tant que le caractère de ma chaine copie_str est un espace
		i++;						// incrémentation de i
		debut++;					// incrémentation de debut
	}
	
	/*Dans cette seconde boucle while on va tester les derniers caractères de notre chaîne 'copie_str' et ainsi trouver l'indice du dernier
	caractère qui n'est pas un espace => dans le but de supprimer les esapces en fin de chaîne*/
	while(copie_str[k] == ' '){		// tant que le caractère de ma chaine copie_str est un espace
		k--;						// décrémentation de k
	}
	
	/*Dans cette boucle for on va réécrire la chaîne sans les espaces de début et de fin dans la chaîne initiale 'str'*/
	for(int j=debut; j<=k;j++){			// début = l'indice du premier caractère (qui n'est pas un espace) récupérer dans la première boucle while
										// k = l'indice du dernier caractère (qui n'est pas un espace) récupérer dans la seconde boucle while
		str[j-debut]=copie_str[j];		// écriture dans la chaîne 'str' à partir de le copie 
	}
	
	return 0; 
}

int clean(char* str) {
	int Size_str=strlen(str);	
	//char new[Size_str];
	char* copie_str = strdup(str); 	// copie la str dans copie_str
	memset(str,0,strlen(str)); 		// vide la chaine str 
	int i=0;
	int cpt=0;
	
	/*Cette boucle for va permettre de remplacer plusieurs espaces consécutif en un seul*/
	for(int i=0;i<Size_str;i++){								// on parcours la chaine copié 'copie_str'
		if((copie_str[i] == ' ')&&(copie_str[i+1] == ' ')){		// si le caractère de la chaine est un espace à l'indice 'i' mais aussi à l'indice 'i+1'
			str[cpt]=' ';										// on écrit dans la chaîne 'str' un seul espace
		}	
		else{
			str[cpt]=copie_str[i];								// sinon on écrit dans la chaine 'str' le caractère de copie à l'indice 'i'
			cpt++;												// on incrémente 'cpt' 
		}
	}
	
	return 0;
}

int separate_s(char* str, char* s, size_t max) {
	int Size_str=strlen(str);
	int Size_s=strlen(s);
	char* copie_str = strdup(str); // copie la str dans copie_str
	memset(str,0,strlen(str)); 		// vide la chaine str 
	int cpt=0;
	int i=0;
	
	// Si la taille de la chaine est supérieur à la capacité max
	if(Size_str > max){
		copie_str[max]='\0';	// la copie de la chaine 'str' prend à l'indice 'max' '\0' pour signaler la fin de la chaîne
	}
		/*Cette boucle while va parcourir la chaine 'copie_str' tant que la chaîne n'est pas fini et que l'indice i ne dépasse pas la 
		capacité maximale */
		while((i<max)&&(copie_str[i] != '\0')){
			if(   (copie_str[i] == copie_str[i+1])   &&  ((copie_str[i] =='&') || (copie_str[i] == '|'))  ){ // test s'il y a deux fois le même caractères => && ou ||
				str[cpt]=' ';				// on marque un esapce dans 'str'
				str[cpt+1]=copie_str[i];	// on y écrit le caractères doublé
				str[cpt+2]=copie_str[i];
				str[cpt+3]=' ';				// on marque un autre espace 
				cpt=cpt+4;					// on fait avancer le compteur du nombre de case qu'on a écrit pour 'str'
				i=i+2;						// on saute les doubles caractères en incrémenter de 2 l'indice 'i' pour 'copie_str'
			}
			else{
				for(int j=0;j<Size_s;j++){					//cette boucle va parcourir 's' qui contient tous les caractères à trouver
						if(copie_str[i] == s[j] ){			// test si le caractère de 'copie_str' correspond au caractères de 's'
							if(i==0){						// si oui : test si c'est le premier indice 
								str[cpt]=copie_str[i];					// si oui : marque le caractère spécial
								str[cpt+1]=' ';							// marque un espace
								cpt=cpt+2;								// incrémente le compteur du nombre de case qu'on a écrit dans 'str'
								i++;									// on avance dans la chaîne 'copie_str'
							}
							else if(i == Size_str-1){					// sinon : test si c'est le dernier caractère
								str[cpt]=' ';							// marque un esapce dans 'str'
								str[cpt+1]=copie_str[i];				// écrit le caractère de 'copie_str' dans 'str'
								cpt=cpt+2;								// incrémente le compteur du nombre de case qu'on a écrit dans 'str'
								i++;									// on avance dans la chaîne 'copie_str'
							}
							else{										// sinon : (pour les caractères qui se trouve dans la chaine même
								str[cpt]=' ';							// marque un esapce dans 'str'
								str[cpt+1]=copie_str[i];				// écrit le caractère de 'copie_str' dans 'str'
								str[cpt+2]=' ';							// marque un esapce dans 'str'
								cpt=cpt+3;								// incrémente le compteur du nombre de case qu'on a écrit dans 'str'
								i++;									// on avance dans la chaîne 'copie_str'
							}
						}
				}
			}
			
			str[cpt]=copie_str[i];							// on écrit le caractère dans la chaîne 'str'
			cpt++;											// incrémente cpt
			i++;											// incrémente i
		}
	
	return 0;
}

int substenv(char* str, size_t max) {
	int cpt=0;
	char var[5];
	char* copie_str = strdup(str); // copie la str dans copie_str
	char* d=" ";
	int Size_copie_str = strlen(copie_str);
	char* tmp;
	
	for(int i=0; i<Size_copie_str-1;i++){		// Cette boucle va permettre de parcourir la chaine 'copie_str'
		if(copie_str[i] == '$'){				// test si le caractère est un '$'
			while((copie_str[i+1] != ' ')&&(copie_str[i+1] != '\0')){		// si oui : boucle while => tant que le prochain caractère n'est ni un esapce ni la
																			//fin de chaine
				var[cpt]=copie_str[i+1];									// on écrit dans la chaîne 'var' les caractères de 'copie_str'
				cpt++;														// incrémente cpt	
				i++;			 											// incrémente i
			}
			cpt=0;															// remet cpt à 0 pour capturer une autre variale d'environnement
			const char * path = getenv( var );								// déclaration de la variable d'environnement grâce à la fonction 'getenv' ainsi	
			printf( "%s\n",path);														// qu'à la variable précédement découpée
		}	
	}
	return 0;
}

int strcut(char* str, char sep, char** tokens, size_t max) {
	int i=0;
	int cpt=0;
	char* copie_str = strdup(str); 		// copie la str dans copie_str 
	memset(str,0,strlen(str)); 			// vide la chaine str 
	char *d=&sep; 
	
	do {
	    str = strsep(&copie_str, d);	// str prend le morceau coupé par des espaces
		tokens[i]=str;					// sauvegarde de ce morceau dans un tableau de chaîne
		i++;							// incrémentation de i
	}while(str != NULL);				//tant que la chaîne 'str' n'est pas vide
	
	tokens[max]=NULL;			
	
	return 0;
}

// Optionnel
/*int strcut_s(char* str, char sep, char** tokens, size_t max) {

}*/
