#ifndef structure_h
#define structure_h

#include "DES.h"

void decrypt(char* fileName, bloc64* key);
void crypt(char* fileName, bloc64* key);
void readKeyFromFile(char* fileName, bloc64* key);

void checkKey(char* key);
void checkKeyFile(char* fileName);
void convertKeyASCII(char* stringKey, bloc64* key);

void checkFile(char* fileName, int progMode);
uint8_t checkMode(char* mode);
uint8_t checkStructure();

int8_t isFileInDirectory(char* fileName, char* dirPath);
uint8_t directoryExists(char* directoryName);

#endif /* structure_h */