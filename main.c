#include "DES.h"
#include <time.h>

uint8_t getbitvalue(uint8_t v, uint8_t idx)
{
	return (v >> idx) & 1;
}


void printbits_8(uint8_t v)
{
	for (uint8_t i = 8; i > 0; --i)
		printf("%u", getbitvalue(v, i-1));
}

void printbits_32(uint32_t v)
{
	// uint8_t* o1 = (uint8_t*)(&v);
	// uint8_t* o2 = o1 + 1;
	// uint8_t* o3 = o1 + 2;
	// uint8_t* o4 = o1 + 3;
	// printbits_8(*o1);
	// printf(" ");
	// printbits_8(*o2);
	// printf(" ");
	// printbits_8(*o3);
	// printf(" ");
	// printbits_8(*o4);

	bloc32* b = (bloc32*)(&v);
	for (uint8_t i = 0; i < 4; ++i)
	{
		printbits_8(b->i8[i]);
		printf(" ");
	}
}

int main(){

    // Truc utile pour tester le temps d'exec
    //clock_t begin = clock();
    //Fonction a tester le temps
    //clock_t stop = clock();
    //double time_spent =(double)(stop-begin)/CLOCKS_PER_SEC*1000;
    //printf("Temps d'execution : %f\n", time_spent);

    bloc64 test;
    test.i64 = 456;
    printf("Avant chiffrement et dechiffrement\n");
    printbits_32(test.i32[0]);
    printbits_32(test.i32[1]);
    test = chiffrement(test);
		printf("\nChiffré\n");
		printbits_32(test.i32[0]);
		printbits_32(test.i32[1]);
    test = dechiffrement(test);
    printf("\nApres\n");
    printbits_32(test.i32[0]);
    printbits_32(test.i32[1]);
		printf("\n");
    return 0;
}
