#include <dirent.h> 
#include <errno.h> //Used to check states on oppening files
#include <string.h> //Used to compare/concatenate chars*
#include <ctype.h> //Used to check for ascii chars
#include <sys/stat.h>

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

#include <time.h>
#include <inttypes.h>

#include "DES.h"
#include "structure.h"

int decrypt(char* fileName, bloc64* key){
	int isDecrypted = 1;
	/*
	 * -3 : erreur de creation du fichier clair
	 * -2 : erreur de lecture
	 * -1 : erreur lors du decryptage
	 * 1 : fichier decrypter
	 */
	char* clair = "./clairs/";
	char* chiffres = "./chiffres/";

	char* filePath = NULL;	
	int fileNameLenght = strlen(fileName);
	filePath = malloc(7+fileNameLenght+1);
	strcpy(filePath, chiffres);
	strcat(filePath, fileName);

	FILE* file = fopen(filePath, "r");

	char* decryptedPath = NULL;	
	decryptedPath = malloc(7+fileNameLenght+1);
	strcpy(decryptedPath, clair);
	strcat(decryptedPath, fileName);

	FILE* decrypted = fopen(decryptedPath, "w");

	if(file == NULL){
		isDecrypted = -1;
	}
	else if(decrypted == NULL){
		isDecrypted = -3;
	}	
	else{
		fseek(file, 0, SEEK_END);

		long int fileSize = ftell(file);

		fseek(file, 0, SEEK_SET);

		long int nb64BitsBlocks = fileSize / 8; //ftell renvoie le nombre D'OCTETS

		long int lastBitsBlock = fileSize % 8; //On recupere le nombre d'octets restants
		
		bloc64 currentBlock; 
		currentBlock.i64 = 0;
		
		for(long int i = 0; i < nb64BitsBlocks; i++){
			fread(&currentBlock.i64, sizeof(uint64_t), 1, file);
			dechiffrement(&currentBlock, key);
			fwrite(&currentBlock.i64, sizeof(uint64_t), 1, decrypted);
		}
		if(lastBitsBlock > 0){
			bloc64 lastBlock;
			lastBlock.i64 = 0;
			long int nbI8 = 8;
			for(long int i = 0; i < lastBitsBlock; i++){ //Lecture des derniers octets
				fread(&lastBlock.i8[i], sizeof(uint8_t), 1, file);
			}

			for(long int i = lastBitsBlock; i < nbI8; i++){ //Completion du bloc64 avec des 0
				lastBlock.i8[i] = 0;
			}
			dechiffrement(&lastBlock, key);
			
			fwrite(&lastBlock.i64, sizeof(uint64_t), 1, decrypted);
		}
	}
	fclose(file);
	fclose(decrypted);
	return isDecrypted;	
}
int crypt(char* fileName, bloc64* key){
	int isCrypted = 1;
	/*
	 * -3 : erreur de creation du fichier crypte
	 * -2 : erreur de lecture
	 * -1 : erreur lors du cryptage
	 * 1 : fichier crypter
	 */
	char* clair = "./clairs/";

	char* filePath = NULL;	
	int fileNameLenght = strlen(fileName);
	filePath = malloc(7+fileNameLenght+1);
	strcpy(filePath, clair);
	strcat(filePath, fileName);

	FILE* file = fopen(filePath, "r");

	
	char* chiffres = "./chiffres/";

	char* cryptedPath = NULL;	
	cryptedPath = malloc(7+fileNameLenght+1);
	strcpy(cryptedPath, chiffres);
	strcat(cryptedPath, fileName);

	FILE* crypted = fopen(cryptedPath, "w");

	if(file == NULL){
		isCrypted = -1;
	}
	else if(crypted == NULL){
		isCrypted = -3;
	}	
	else{
		fseek(file, 0, SEEK_END);

		long int fileSize = ftell(file);

		fseek(file, 0, SEEK_SET);

		long int nb64BitsBlocks = fileSize / 8; //ftell renvoie le nombre D'OCTETS

		long int lastBitsBlock = fileSize % 8; //On recupere le nombre d'octets restants
		
		bloc64 currentBlock; 
		currentBlock.i64 = 0;
		
		for(long int i = 0; i < nb64BitsBlocks; i++){
			fread(&currentBlock.i64, sizeof(uint64_t), 1, file);
			chiffrement(&currentBlock, key);
			fwrite(&currentBlock.i64, sizeof(uint64_t), 1, crypted);
		}
		if(lastBitsBlock > 0){
			bloc64 lastBlock;
			lastBlock.i64 = 0;
			long int nbI8 = 8;
			for(long int i = 0; i < lastBitsBlock; i++){ //Lecture des derniers octets
				fread(&lastBlock.i8[i], sizeof(uint8_t), 1, file);
			}

			for(long int i = lastBitsBlock; i < nbI8; i++){ //Completion du bloc64 avec des 0
				lastBlock.i8[i] = 0;
			}
			chiffrement(&lastBlock, key);
			
			fwrite(&lastBlock.i64, sizeof(uint64_t), 1, crypted);
		}
	}
	fclose(file);
	fclose(crypted);
	return isCrypted;	
}

int checkFileAscii(char* fileName, char* repository){
	int isAscii = 1;
	/*
	 * -2 : erreur de lecture
	 * -1 : fichier contenant des caracters non ascii
	 * 1 : fichier au format ascii ascii
	 */
	char* filePath = NULL;	
	int fileNameLenght = strlen(fileName);
	filePath = malloc(7+fileNameLenght+1);
	strcpy(filePath, repository);
	strcat(filePath, fileName);

	FILE* file = fopen(filePath, "r");
	if(file == NULL){
		isAscii = -1;
	}	
	else{
		fseek(file, 0, SEEK_END);

		long int fileSize = ftell(file);

		fseek(file, 0, SEEK_SET);
	
		char* localChar = NULL;
		localChar = malloc(sizeof(char));
		
		for(long int i = 0; i < fileSize; i++){
			fread(localChar, sizeof(char), sizeof(char), file);
			printf("%c", localChar[0]);
			if(isascii(localChar[0]) == 0){
				isAscii = -1;
			}
		}
		printf("\n");
	}
	fclose(file);
	return isAscii;	
}

int saveKey(char* fileName, char* key){
	int keyValue;

	 /*
	 * -1 : erreur de sauvegarde
	 * 1 : Cle sauvee
	 */
	
	char* filePath = NULL;
	int fileNameLenght = strlen(fileName);
	filePath = malloc(7+fileNameLenght+1);
	strcpy(filePath, "./cles/");
	strcat(filePath, fileName);

	FILE* file = fopen(filePath, "w");
	if(file == NULL){
		keyValue = -1;
	}	
	else{
		fwrite(key, sizeof(char), 8, file);
		keyValue = 1;
	}

	return keyValue;		
}


int readKeyFromFile(char* fileName, char* key){
	int keyValue;
	/*
	 * -1 : erreur de lecture
	 * 1 : Cle lue 
	 */
	
	char* filePath = NULL;
	int fileNameLenght = strlen(fileName);
	filePath = malloc(7+fileNameLenght+1);
	strcpy(filePath, "./cles/");
	strcat(filePath, fileName);

	FILE* file = fopen(filePath, "r");
	if(file == NULL){
		keyValue = -1;
	}	
	else{
		fseek(file, 0, SEEK_END);

		fseek(file, 0, SEEK_SET);
	
		char* localKey = NULL;
		localKey = malloc(8+1);

		fread(localKey, sizeof(char), sizeof(char)*8, file);
		printf("Cle : %s\n", localKey);

		memcpy(key, &localKey[0], 8);
		key[8] = '\0';

		keyValue = 1;
	}
	fclose(file);
	return keyValue;	
}

int checkKey(char* key){
	int keyValue;
	/* 
	 * -2 : Cle invalide (Caracteres non ascii)
	 * -1 : Cle invalide (taille)
	 * 1 : Cle valide
	 */
	size_t keySize = strlen(key);
	if(keySize == 8){
		for(size_t i = 0; i < keySize; i += sizeof(char)){
			if(isascii(key[i]) == 0){
				keyValue = -2;
			}
		}
		if(keyValue != -2){
			keyValue = 1;
		}
	}
	else{
		keyValue = -1;
		printf("La cle doit etre sur 8 caracteres ascii, taille actuelle : %zd\n", keySize);
	}
	return keyValue;
}

int checkKeyFile(char* fileName){
	int state = 0;
	/*
	 * 1 : cle accessible 
	 * -1 : N'importe quel autre cas 
	 */
	char* dirCles = "cles/";

	int keyInCles = isFileInDirectory(fileName, dirCles);

	if(keyInCles == -1){ //cle inaccessible(droits)
		state = -1;
		printf("La cle est inaccessible dans le dossier cles(verifier les droits)\n");
	}
	else if(keyInCles == -2){ //Cle inaccessible(autre)
		state = -1;
		printf("La cle est inaccessible dans le dossier cles\n");
	}
	else if(keyInCles == -3){ //Cle introuvable
		state = -1;
		printf("La cle est introuvable dans le dossier cles\n");
	}
	else if(keyInCles == 1){ //Cle accessible
		state = 1;
		printf("La cle est accessible dans le dossier cles\n");
	}
	return state;
}	

int checkMode(char* mode){
	int modeValue;
	/*
	 * -1 : ne correspond pas a un mode
	 * 1 : cryptage 
	 * 2 : decryptage
	 */
	if(strncmp("cf", mode, sizeof("cf")) == 0){
		modeValue = 1; 
		printf("Mode defini pour le cryptage\n");
	}
	else if(strncmp("df", mode, sizeof("df")) == 0){
		modeValue = 2;
		printf("Mode defini pour le decryptage\n");
	}
	else{
		modeValue = -1;
		printf("Mode inconnu, impossible de continuer\n");
	}
	return modeValue;
}

int checkFile(char* fileName, int progMode){
	int state = 0;
	/*
	 * 1 : fichier dans le bon dossier avec le bon mode
	 * -1 : N'importe quel autre cas 
	 */
	char* dirClairs = "clairs/";
	char* dirChiffres = "chiffres/\0";

	int fileInClairs = isFileInDirectory(fileName, dirClairs);
	int fileInChiffres = isFileInDirectory(fileName, dirChiffres);

	if(fileInClairs == -1 || fileInChiffres == -1){ //Fichiers inaccessibles(droits)
		state = -1;
		printf("%s est inaccessible dans les dossiers clairs et chiffres(verifier les droits)\n", fileName);
	}
	else if(fileInClairs == -2 || fileInChiffres == -2){ //Fichiers inaccessibles(autre)
		state = -1;
		printf("%s est inaccessible dans les dossiers clairs et chiffres\n", fileName);
	}
	else if(fileInClairs == -3 && fileInChiffres == -3){ //Fichiers introuvables
		state = -1;
		printf("%s est introuvable dans les dossiers clairs et chiffres\n", fileName);
	}
	else if(fileInClairs == -3 && fileInChiffres == -1){ //Fichier existe dans mauvais dossier et inaccessibles(droits)
		state = -1;
		printf("%s trouve dans le mauvais dossier et est inaccessible(verifier les droits)\n", fileName);
	}
	else if(fileInClairs == -3 && fileInChiffres == -2){ //Fichier existe dans mauvais dossier et inaccessibles(autres)
		state = -1;
		printf("%s trouve dans le mauvais dossier et est inaccessible\n", fileName);
	}

	if(progMode == 1){ //Cryptage
		if(fileInClairs == 1 && fileInChiffres == -3){ //Fichier dans le bon dossier et inexistant dans les autres dossiers
			state = 1;
			printf("%s trouve dans le bon dossier et accessible pour le cryptage\n", fileName);
		}
		else if(fileInClairs == -3 && fileInChiffres == 1){ //Fichier dans le mauvais dossier et inexistant dans les autres
			state = -1;
			printf("%s trouve dans chiffres, pour le cryptage le fichier doit etre dans clairs\n", fileName);
		}
		else if(fileInClairs == 1 && fileInChiffres == 1){ //Fichier present dans les deux dossiers 
			state = -1;
		printf("%s trouve dans le mauvais dossier et est inaccessible(verifier les droits)\n", fileName);
		}
	}
	else if(progMode == 2){ //Decryptage
		if(fileInClairs == 1 && fileInChiffres == -3){ //Fichier dans le mauvais dossier et inexistant dans les autres dossiers
			state = -1;
			printf("%s trouve dans clairs, pour le decryptage le fichier doit etre dans chiffres\n", fileName);
		}
		else if(fileInClairs == -3 && fileInChiffres == 1){ //Fichier dans le bon dossier et inexistant dans les autres
			state = 1;
			printf("%s trouve dans le bon dossier et accessible pour le decryptage\n", fileName);
		}
		else if(fileInClairs == 1 && fileInChiffres == 1){ //Fichier present dans les deux dossiers 
			state = -1;
			printf("%s trouve dans les deux dossier, le fichier a-t-il deja ete crypter? Deplacer le fichier du dossier chiffres\n", fileName);
		}
	}
	return state;
}	

int isFileInDirectory(char* fileName, char* dirPath){
	int fileState = 0;
	/*
	 * 1 : Fichier trouve 
	 * -1 : Fichier existe mais erreur de droits
	 * -2 : Erreur autre
	 * -3 : fichier inexistant
	 */

	char* filePath = NULL;
	int fileNameLenght = strlen(fileName);
	int dirPathLenght = strlen(dirPath);
	filePath = malloc(2+dirPathLenght+fileNameLenght+1);
	strcpy(filePath, "./");
	strcat(filePath, dirPath);
	strcat(filePath, fileName);

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



int directoryExists(char* directoryName){
	int dirState;
	/*
	 * 0 : Dossier existe et est accessible
	 * -1 : Erreur de droits
	 * -2 : Erreur autre
	 */

	char* directoryPath = NULL;
	int directoryNameLenght = strlen(directoryName);
	directoryPath = malloc(2+directoryNameLenght+1);
	strcpy(directoryPath, "./");
	strcat(directoryPath, directoryName);

	DIR* dir = opendir(directoryPath);
	if(dir){	//dossier existe
		dirState = 0;
		printf("%s existe et est accessible\n", directoryPath);	
		closedir(dir);
	}
	else if (ENOENT == errno){	//dossier n'existe pas
		mkdir(directoryPath, 0700);
		printf("%s n'existe pas et a ete cree\n", directoryPath);
		dirState = directoryExists(directoryName);
	}
	else if(EACCES == errno){	//problèmes de droits
		dirState = -1;
		printf("%s existe mais n'est pas accessible (verifier les droits)\n", directoryPath);
	}
	else{	//Erreur d'ouverture pour raison X
		dirState = -2;
		printf("%s est inaccessible\n", directoryPath);
	}
	free(directoryPath);
	return dirState;
}

int checkStructure(){
	int dirResult[3];
	int returnValue = 1;
	/*
	 * 1 : Structure ok 
	 * -1 : Probleme dans la structure
	 */

	char dirClairs[7] = "clairs\0";
	char dirChiffres[9] = "chiffres\0";
	char dirCles[5] = "cles\0";
	
	dirResult[0] = directoryExists(dirClairs);
	dirResult[1] = directoryExists(dirChiffres);
	dirResult[2] = directoryExists(dirCles);

	for(int i = 0; i < 3; i++){
		if(dirResult[i] != 0){
			returnValue = -1;
		}
	}
	return returnValue;
}
