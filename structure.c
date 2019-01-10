#include <dirent.h> 
#include <errno.h> //Used to check states on oppening files
#include <string.h> //Used to compare/concatenate chars*
#include <ctype.h> //Used to check for ascii chars
#include <sys/stat.h>

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

#include "DES.h"
#include "structure.h"
#include "MyUtils.h"

// Fonction principale de décryptage d'un fichier
void decrypt(char* fileName, bloc64* key){
	// On créer les chemins vers les fichiers
	int fileNameLength = strlen(fileName);
	
	char* filePath = malloc(7+fileNameLength+1);
	strcpy(filePath, "./chiffres/");
	strcat(filePath, fileName);
	FILE* file = fopen(filePath, "rb");
	free(filePath);

	char* decryptedPath = malloc(7+fileNameLength+1);
	strcpy(decryptedPath, "./clairs/");
	strcat(decryptedPath, fileName);
	FILE* decrypted = fopen(decryptedPath, "wb");
	free(decryptedPath);

	if(file == NULL){
		printf("Erreur lors de la lecture du fichier : ./chiffres/%s\n", fileName);
		exit(1);
	}
	else if(decrypted == NULL){
		printf("Erreur lors de la creation de ./clairs/%s\n", fileName);
		exit(1);
	}	

	// On décrypte
	fseek(file, 0, SEEK_END);
	long int fileSize = ftell(file);
	fseek(file, 0, SEEK_SET);
	
	bloc64 currentBlock; 
	currentBlock.i64 = 0u;
	for(int i = 0; i*8 < fileSize; i++) {
		// On inverse pcq dans l'algo les i8 sont inversés
		for(uint8_t j = 0; j < 8; ++j) {
			fread(&currentBlock.i8[7-j], sizeof(char), 1, file);
		}
		dechiffrement(&currentBlock, key);
		// On re-inverse pour écrire correctement dans le fichier
        for (uint8_t j = 0; j < 8; ++j)
        {
            printf("%c", currentBlock.i8[7-j]);
            fwrite(&currentBlock.i8[7-j], sizeof(char), 1, decrypted);
        }
	}
	// On print un : pour voir les espace à la fin 
	printf(":\n");

	fclose(file);
	fclose(decrypted);
}
// Fonction principale de décryptage
void crypt(char* fileName, bloc64* key){
	// On créer les chemins vers les fichiers
	int fileNameLength = strlen(fileName);

	char* filePath = malloc(7+fileNameLength+1);
	strcpy(filePath, "./clairs/");
	strcat(filePath, fileName);
	FILE* file = fopen(filePath, "rb");
	free(filePath);

	char* cryptedPath = malloc(7+fileNameLength+1);
	strcpy(cryptedPath, "./chiffres/");
	strcat(cryptedPath, fileName);
	FILE* crypted = fopen(cryptedPath, "wb");
	free(cryptedPath);

	if(file == NULL){
		printf("Erreur lors de la lecture du fichier : ./clairs/%s\n", fileName);
		exit(1);
	}
	else if(crypted == NULL){
		printf("Erreur lors de la creation du fichier crypté");
		exit(1);
	}	

	// On crypte
	fseek(file, 0, SEEK_END);
	long int fileSize = ftell(file);
	fseek(file, 0, SEEK_SET);

	bloc64 currentBlock; 
	currentBlock.i64 = 0;
	for(long int i = 0; i < fileSize / 8; i++) {
		for(uint8_t j = 0; j < 8; ++j) {
			fread(&currentBlock.i8[7-j], sizeof(char), 1, file);
		}
		chiffrement(&currentBlock, key);
        for (uint8_t j = 0; j < 8; ++j)
        {
            fwrite(&currentBlock.i8[7-j], sizeof(char), 1, crypted);
        }
	}
	// On gère le dernier bloc si il est incomplet
	long int lastBitsBlock = fileSize % 8; //On recupere le nombre d'octets restants
	if(lastBitsBlock > 0){
		bloc64 lastBlock;
		lastBlock.i64 = 0;
		for(uint8_t i = 0; i < 8; i++){ //Lecture des derniers octets
			if(i < lastBitsBlock)
				fread(&lastBlock.i8[7-i], sizeof(uint8_t), 1, file);
			else
				lastBlock.i8[7-i] = 0;
		}
		chiffrement(&lastBlock, key);
		
		fwrite(&lastBlock.i64, sizeof(uint64_t), 1, crypted);
	}

	fclose(file);
	fclose(crypted);
}

// Lit la clef depuis un fichier
void readKeyFromFile(char* fileName, bloc64* key){
	// On vérifie le fichier
	checkKeyFile(fileName);

	// On construit le chemin vers le fichier
	int fileNameLength = strlen(fileName);
	char* filePath = malloc((7+fileNameLength+1)*sizeof(char));
	strcpy(filePath, "./cles/");
	strcat(filePath, fileName);

	FILE* file = fopen(filePath, "r");
	if(file == NULL){
		printf("Erreur lors de la lecture de la cle depuis le fichier\n");
		exit(1);
	}	

	// On lit la clef
	char* stringKey = calloc(17, sizeof(char));
	for (int i = 0; i < 16; ++i)
	{
		fread(&stringKey[i], sizeof(char), 1, file);
	}
	fclose(file);
	stringKey[17] = '\0';

	printf("Cle: %s lue depuis le fichier\n", stringKey);

	// On verifie la clef
	checkKey(stringKey);

	// Conversion de la clef en nombre
	convertKeyASCII(stringKey, key);
}

// Conversion de la clef (ASCII) en nombre
void convertKeyASCII(char* stringKey, bloc64* key) {
	char* temp = calloc(3, sizeof(char));
	for (int i = 0; i < 8; ++i)
	{
		memcpy(temp, &stringKey[i*2], 2);
		key->i8[7-i] = strtol(temp, NULL, 16);
	}
}

// On verifie que la clef fait bien la bonne taille
void checkKey(char* key){
	if(strlen(key) != 16) {
		printf("La cle doit etre sur 16 caracteres ascii, taille actuelle : %d\n", strlen(key));
		exit(1);
	}
}

// Verifie le fichier contenant la clef
void checkKeyFile(char* fileName){
	switch(isFileInDirectory(fileName, "cles/")) {
		case 1:
			printf("La cle est accessible dans le dossier cles\n");
			break;
		case -1:
			printf("La cle est inaccessible dans le dossier cles (verifier les droits)\n");
			exit(1);
			break;
		case -2:
			printf("La cle est inaccessible dans le dossier cles\n");
			exit(1);
			break;
		case -3:
			printf("La cle est introuvable dans le dossier cles\n");
			exit(1);
			break;
	}
}	

// Vérifie et renvoit le mode séléctionné
uint8_t checkMode(char* mode){
	if(strncmp("cf", mode, sizeof("cf")) == 0){
		printf("Mode defini pour le cryptage\n");
		return 1;
	}
	else if(strncmp("df", mode, sizeof("df")) == 0){
		printf("Mode defini pour le decryptage\n");
		return 2;
	}
	printf("Mode inconnu, impossible de continuer\n");
	exit(1);
}

// Verifie la presence et l'accessibilite du fichier dans les dossiers
void checkFile(char* fileName, int progMode){
	int fileInClairs = isFileInDirectory(fileName, "clairs/");
	int fileInChiffres = isFileInDirectory(fileName, "chiffres/\0");

	if(fileInClairs == -1 || fileInChiffres == -1){ //Fichiers inaccessibles(droits)
		printf("%s est inaccessible dans les dossiers clairs et chiffres(verifier les droits)\n", fileName);
		exit(1);
	}
	else if(fileInClairs == -2 || fileInChiffres == -2){ //Fichiers inaccessibles(autre)
		printf("%s est inaccessible dans les dossiers clairs et chiffres\n", fileName);
		exit(1);
	}
	else if(fileInClairs == -3 && fileInChiffres == -3){ //Fichiers introuvables
		printf("%s est introuvable dans les dossiers clairs et chiffres\n", fileName);
		exit(1);
	}
	else if(fileInClairs == -3 && fileInChiffres == -1){ //Fichier existe dans mauvais dossier et inaccessibles(droits)
		printf("%s trouve dans le mauvais dossier et est inaccessible(verifier les droits)\n", fileName);
		exit(1);
	}
	else if(fileInClairs == -3 && fileInChiffres == -2){ //Fichier existe dans mauvais dossier et inaccessibles(autres)
		printf("%s trouve dans le mauvais dossier et est inaccessible\n", fileName);
		exit(1);
	}

	if(progMode == 1){ //Cryptage
		if(fileInClairs == 1 && fileInChiffres == -3){ //Fichier dans le bon dossier et inexistant dans les autres dossiers
			printf("%s trouve dans le bon dossier et accessible pour le cryptage\n", fileName);
		}
		else if(fileInClairs == -3 && fileInChiffres == 1){ //Fichier dans le mauvais dossier et inexistant dans les autres
			printf("%s trouve dans chiffres, pour le cryptage le fichier doit etre dans clairs\n", fileName);
			exit(1);
		}
		else if(fileInClairs == 1 && fileInChiffres == 1){ //Fichier present dans les deux dossiers 
			printf("%s trouve dans le mauvais dossier et est inaccessible(verifier les droits)\n", fileName);
			exit(1);
		}
	}
	else if(progMode == 2){ //Decryptage
		if(fileInClairs == 1 && fileInChiffres == -3){ //Fichier dans le mauvais dossier et inexistant dans les autres dossiers
			printf("%s trouve dans clairs, pour le decryptage le fichier doit etre dans chiffres\n", fileName);
			exit(1);
		}
		else if(fileInClairs == -3 && fileInChiffres == 1){ //Fichier dans le bon dossier et inexistant dans les autres
			printf("%s trouve dans le bon dossier et accessible pour le decryptage\n", fileName);
		}
		else if(fileInClairs == 1 && fileInChiffres == 1){ //Fichier present dans les deux dossiers 
			printf("%s trouve dans les deux dossier, le fichier a-t-il deja ete crypter? Deplacer le fichier du dossier chiffres\n", fileName);
			exit(1);
		}
	}
}	

// Verifie si le fichier est dans le dossier
int8_t isFileInDirectory(char* fileName, char* dirPath){
	// On renvoit 1 si tout va bien sinon erreur
	uint8_t fileState = 0;

	// On créer le chemin vers le fichier
	char* filePath = malloc(2 + strlen(dirPath) + strlen(fileName) + 1);
	strcpy(filePath, "./");
	strcat(filePath, dirPath);
	strcat(filePath, fileName);

	// On ouvre le fichier et on voit
	FILE* file = fopen(filePath, "r");
	if(file){	//fichier existe
		fileState = 1;
		printf("%s existe et est accessible\n", filePath);
		fclose(file);	
	}
	else if (ENOENT == errno){	//fichier n'existe pas
		fileState = -3;
		printf("Fichier introuvable dans : %s\n", filePath);
	}
	else if(EACCES == errno){	//problèmes de droits
		fileState = -1;
		printf("%s existe mais n'est pas accessible (verifier les droits)\n", filePath);
	}
	else{	//Erreur d'ouverture pour raison X
		fileState = -2;
		printf("%s est inaccessible\n", filePath);
	}

	free(filePath);
	return fileState;
}


// Verifie que le dossier existe
uint8_t directoryExists(char* directoryName){
	// On créer le chemin vers le dossier
	int directoryNameLenght = strlen(directoryName);
	char* directoryPath = malloc(2+directoryNameLenght+1);
	strcpy(directoryPath, "./");
	strcat(directoryPath, directoryName);

	// On essaye de l'ouvrir
	DIR* dir = opendir(directoryPath);	

	if(dir) {	//dossier existe
		printf("%s existe et est accessible\n", directoryPath);	
		closedir(dir);
		free(directoryPath);
		return 1;
	}
	else {	//Erreur d'ouverture pour raison X
		printf("%s est inaccessible\n", directoryPath);
		free(directoryPath);
		return 0;
	}
}

// Verifie la structure des dossiers 
uint8_t checkStructure() {
	return directoryExists("clairs\0") && directoryExists("chiffres\0") && directoryExists("cles\0");
}
