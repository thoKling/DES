#include "DES.h"
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <inttypes.h>
#include "MyUtils.h"

int main(){

    // Truc utile pour tester le temps d'exec
    //clock_t begin = clock();
    //Fonction a tester le temps
    //clock_t stop = clock();
    //double time_spent =(double)(stop-begin)/CLOCKS_PER_SEC*1000;
    //printf("Temps d'execution : %f\n", time_spent);

    FILE * pFile;
    long lSize;
    char * buffer;
    size_t result;

    pFile = fopen ( "test.txt" , "rb" );
    if (pFile==NULL) {fputs ("File error",stderr); exit (1);}

    // obtain file size:
    fseek (pFile , 0 , SEEK_END);
    lSize = ftell (pFile);
    rewind (pFile);

    // allocate memory to contain the whole file:
    buffer = (char*) malloc (sizeof(char)*lSize);
    if (buffer == NULL) {fputs ("Memory error",stderr); exit (2);}

    // copy the file into the buffer:
    result = fread (buffer,1,lSize,pFile);
    if (result != lSize) {fputs ("Reading error",stderr); exit (3);}

    /* the whole file is now loaded in the memory buffer. */

    // terminate
    fclose (pFile);
    //printf("%s", buffer);


    //init.i64 = 0b0000000100100011010001010110011110001001101010111100110111101111;
    //printf("Debut chiffre : %"PRIx64"\n", init.i64);
    //chiffrement(&init);
    for(int j = 0; (j*8) < lSize; j++) {
        bloc64 init;
        for (int i = 0; i < 8; ++i)
        {
            init.i8[7-i] = (uint8_t)buffer[(j*8)+i];
        }
        //printf("Debut chiffre : %"PRIx64"\n", init.i64);
        dechiffrement(&init);

        //char toto = init.i8[0];
        //printbits_8(init.i8[0]);
        for (int i = 0; i < 8; ++i)
        {
            printf("%c", init.i8[7-i]);
        }
    }   
    free (buffer);
    //printf("Debut chiffre : %"PRIx64"\n", init.i64);
    return 0;
}
