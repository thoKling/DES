#include <stdio.h>

#include "structure.h"
#include "DES.h"

int main(int argc, char** argv){

	if(argc != 3 && argc != 4) {
		printf("Usage : main.exe filName {cf|df} [key]");
		return 1;
	}

	if(checkStructure()){
		printf("Structure du programme fonctionnelle\n");
	}
	else{
		printf("Probleme lors de la verification de la structure du programme\n"); 
		return 1;
	}

	// Vérification de la structure des fichiers
	int mode = checkMode(argv[2]);
	checkFile(argv[1], mode);

	// On récupere la clef
	bloc64 key;
	
	// Clef dans le fichier
	if(argc == 3) {
		readKeyFromFile(argv[1], &key);
	}
	// Clef donne en ligne de commande
	else if(argc == 4) {
		checkKey(argv[3]);
		convertKeyASCII(argv[3], &key);
	}

	//Chiffrement
	if(mode == 1){ 
		crypt(argv[1], &key);
		printf("Le fichier a ete crypte avec succes!\n");
	}
	//Dechiffrement
	else if(mode == 2){ 
		decrypt(argv[1], &key);
		printf("Le fichier a ete decrypte avec succes!\n");
	}

	return 0;
}
