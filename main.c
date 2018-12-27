#include "DES.h"
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <inttypes.h>

int main(){

    // Truc utile pour tester le temps d'exec
    //clock_t begin = clock();
    //Fonction a tester le temps
    //clock_t stop = clock();
    //double time_spent =(double)(stop-begin)/CLOCKS_PER_SEC*1000;
    //printf("Temps d'execution : %f\n", time_spent);

    bloc64 test;
    test.i64 = 0x0123456789abcdef;
    printf("Init : %"PRIx64"\n", test.i64);
    chiffrement(&test);
    printf("Fin chiffre : %"PRIx64"\n", test.i64);
    dechiffrement(&test);
    printf("Fin dechiffre : %"PRIx64"\n", test.i64);  
    return 0;
}
