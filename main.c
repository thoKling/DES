#include "DES.h"
#include <time.h>



int main(){

    // Truc utile pour tester le temps d'exec
    //clock_t begin = clock();
    //Fonction a tester le temps
    //clock_t stop = clock();
    //double time_spent =(double)(stop-begin)/CLOCKS_PER_SEC*1000;
    //printf("Temps d'execution : %f\n", time_spent);


    bloc64 test;
    test.i64 = 0x0123456789abcdef;
    printf("Avant chiffrement et dechiffrement\n");
		printf("Valeur en héxa : %lx\n", test.i64);
    printbits_32(test.i32[0]);
		printbits_32(test.i32[1]);
    test = chiffrement(test);
		printf("\nChiffré\n");
		printf("Valeur en héxa : %lx\n", test.i64);
		printbits_32(test.i32[0]);
		printbits_32(test.i32[1]);
		test.i64 = 0xb94bd5cf02dcad77;
    test = dechiffrement(test);
    printf("\nApres\n");
    printbits_32(test.i32[0]);
    printbits_32(test.i32[1]);
		printf("\n%lx\n", test.i64);

    return 0;
}
