#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

#include <time.h>
#include <inttypes.h>
#include <string.h>

#include "MyUtils.h"
#include "DES.h"
#include "structure.h"

int main(int argc, char** argv){
	int structureOk = checkStructure();
	if(structureOk){
		printf("Structure du programme fonctionnelle\n");
	}
	else{
		printf("Probleme lors de la verification de la structure du programme, fermeture\n"); 
		exit(0);
	}
	if(argc <= 1){
		printf("Pas d'arguments donnes, utilisation : des fichier (cle) mode\n");
		printf("Avec fichier : nom fichier a crypter/decrypter dans le dossier clairs ou chiffres\n");
	       	printf("OPTIONNEL : Avec cle : la cle de cryptage dans le dossier cles\n");
		printf("Avec mode : le mode d'utilisation du programme cf pour crypter, df pour decrypter\n");
		printf("Fermeture du programme\n");
	}
	else if (argc < 3){
		printf("Pas assez d'arguments donnes, utilisation : des fichier (cle) mode\n");
		printf("Avec fichier : nom fichier a crypter/decrypter dans le dossier clairs ou chiffres, nom de la cle dans le dossier chiffres\n");
	       	printf("OPTIONNEL : Avec cle : la cle de cryptage\n");
		printf("Avec mode : le mode d'utilisation du programme cf pour crypter, df pour decrypter\n");
		printf("Fermeture du programme\n");
	}
	else if(argc == 3){
		printf("nombre d'arguments correct, cle non specifie\n");
		int mode = checkMode(argv[2]);
		if(mode == 1 || mode == 2){
			int fileState = checkFile(argv[1], mode);
			if(fileState == 1){
				int keyFileState = checkKeyFile(argv[1]);
				if(keyFileState == 1){
					char* stringKey = malloc(sizeof(char)*8+1);
					int keyReadState = readKeyFromFile(argv[1], stringKey);
					if(keyReadState == 1){
						printf("Cle: %s lue depuis le fichier\n", stringKey);
						int keyState = checkKey(stringKey);
						if(keyState == 1){
							bloc64 key;
								memcpy(&key.i64, &stringKey[0], 8);
							if(mode == 1){ //Chiffrement
								char* repository = "./clairs/";
								int fileIsAscii = checkFileAscii(argv[1], repository);
								if(fileIsAscii == 1){
									printf("Le fichier %s%s est conforme au format ascii\n", repository, argv[1]);
									int isCrypted = crypt(argv[1], &key);
									if(isCrypted == 1){
										printf("Le fichier a ete crypte avec succes!\n");

									}
									else if(isCrypted == -2){
										printf("Erreur lors de la lecture du fichier : %s%s\n", repository, argv[1]);
									}
									else if (isCrypted == -1){
										printf("Erreur lors du cryptage\n");
									}
									else if (isCrypted == -1){
										printf("Erreur lors de la creation de ./chiffres/%s\n", argv[1]);
									}
								}
								else if(fileIsAscii == -2){
									printf("Erreur lors de la lecture de %s%s\n", repository, argv[1]);
								}
								else if(fileIsAscii == -1){
									printf("Le fichier: %s  contient des caracteres non ascii\n", argv[1]);
								}
							}
							else if(mode == 2){ //Dechiffrement
								int isDecrypted = decrypt(argv[1], &key);
								if(isDecrypted == 1){
									printf("Le fichier a ete decrypte avec succes!\n");

								}
								else if(isDecrypted == -2){
									printf("Erreur lors de la lecture du fichier : ./chiffres/%s\n", argv[1]);
								}
								else if (isDecrypted == -1){
									printf("Erreur lors du decryptage\n");
								}
								else if (isDecrypted == -1){
									printf("Erreur lors de la creation de ./clairs/%s\n", argv[1]);
								}
							}

						}
						else if(keyState == -1){
							printf("Taille de la cle invalide\n");
						}
						else if(keyState == -2){
							printf("Caracteres non ascii detectes dans la cle\n");
						}
					}
					else{
						printf("Erreur lors de la lecture de la cle depuis le fichier\n");
					}
				}
				else{
					printf("Probleme lors de l'acces au fichier contenant la cle\n");
				}
			}
			else{
				printf("Probleme lors de la verification du fichier\n");	
			}	
		}
		else{
			printf("Mode d'utilisation du programme incorrecte\n");
			printf("Mode : le mode d'utilisation du programme cf pour crypter, df pour decrypter\n");
		}
	}
	else if(argc == 4){
		printf("nombre d'arguments correct, cle specifie\n");
		int mode = checkMode(argv[3]);
		if(mode == 1 || mode == 2){
			int fileState = checkFile(argv[1], mode);
			if(fileState == 1){
				int keyState = checkKey(argv[2]);
				if(keyState == 1){
					printf("Cle valide\n");
					char* stringKey = NULL;	
					stringKey = malloc(8+1);
					strcpy(stringKey, argv[2]);
					printf("%s\n", stringKey);
					bloc64 key;
				       	memcpy(&key.i64, &stringKey[0], 8);
					if(mode == 1){ //Chiffrement
						char* repository = "./clairs/";
						int fileIsAscii = checkFileAscii(argv[1], repository);
						if(fileIsAscii == 1){
							printf("Le fichier %s%s est conforme au format ascii\n", repository, argv[1]);
							int isCrypted = crypt(argv[1], &key);
							if(isCrypted == 1){
								printf("Le fichier a ete crypte avec succes!\n");
								int keySaved = saveKey(argv[1], stringKey);
								if(keySaved == 1){
									printf("Cle: '%s' sauvegardee sous ./cles/%s\n", stringKey, argv[1]);
								}
								else{
									printf("Erreur lors de la sauvegarde de la cle : '%s'\n", stringKey);
								}
							}
							else if(isCrypted == -2){
								printf("Erreur lors de la lecture du fichier : %s%s\n", repository, argv[1]);
							}
							else if (isCrypted == -1){
								printf("Erreur lors du cryptage\n");
							}
							else if (isCrypted == -1){
								printf("Erreur lors de la creation de ./chiffres/%s\n", argv[1]);
							}
						}
						else if(fileIsAscii == -2){
							printf("Erreur lors de la lecture de %s%s\n", repository, argv[1]);
						}
						else if(fileIsAscii == -1){
							printf("Le fichier: %s  contient des caracteres non ascii\n", argv[1]);
						}
					}
					else if(mode == 2){ //Dechiffrement
						int isDecrypted = decrypt(argv[1], &key);
						if(isDecrypted == 1){
							printf("Le fichier a ete decrypte avec succes!\n");
							int keySaved = saveKey(argv[1], stringKey);
							if(keySaved == 1){
								printf("Cle: '%s' sauvegardee sous ./cles/%s\n", stringKey, argv[1]);
							}
							else{
								printf("Erreur lors de la sauvegarde de la cle : '%s'\n", stringKey);
							}
						}
						else if(isDecrypted == -2){
							printf("Erreur lors de la lecture du fichier : ./chiffres/%s\n", argv[1]);
						}
						else if (isDecrypted == -1){
							printf("Erreur lors du decryptage\n");
						}
						else if (isDecrypted == -1){
							printf("Erreur lors de la creation de ./clairs/%s\n", argv[1]);
						}
					}

				}
				else if(keyState == -1){
					printf("Taille de la cle invalide\n");
				}
				else if(keyState == -2){
					printf("Caracteres non ascii detectes dans la cle\n");
				}
			}
			else{
				printf("Probleme lors de la verification du fichier\n");	
			}		
		}
		else{
			printf("Mode d'utilisation du programme incorrecte\n");
			printf("Mode : le mode d'utilisation du programme cf pour crypter, df pour decrypter\n");
		}
	}
	else if(argc > 4){	
		printf("Trops d'arguments donnes, utilisation : des fichier (cle) mode\n");
		printf("Avec fichier : nom fichier a crypter/decrypter dans le dossier clairs ou chiffres\n");
	       	printf("OPTIONNEL : Avec cle : la cle de cryptage\n");
		printf("Avec mode : le mode d'utilisation du programme cf pour crypter, df pour decrypter\n");
		printf("Fermeture du programme\n");
	}
}
