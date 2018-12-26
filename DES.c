#include "DES.h"

// Version 1
int f(int i) {
  return i*37453123;
}

// Version 1
bloc64 chiffrement(bloc64 bloc_init) {
  // Li = bloc_init.i32[0]
  // Ri = bloc_init.i32[1]
  // On fait 16 tours de chiffrement
  printf("\nLi: ");
  printbits_32(bloc_init.i32[0]);
  printf("\n");
  printf("Ri: ");
  printbits_32(bloc_init.i32[1]);
  printf("\nRoutine : \n");
  bloc32 bloc_temp;
  for(int i = 0; i < 15; i++) {
    // On sauvegarde le bloc de gauche
    bloc_temp.i32 = bloc_init.i32[0];
    // Li+1 = Ri
    bloc_init.i32[0] = bloc_init.i32[1];
    // Ri+1 = Li (+) f(i)
    bloc_init.i32[1] = bloc_temp.i32 ^ f(i+1);
		printf("Li: %x : %d\n", bloc_init.i32[0], i);
    printbits_32(bloc_init.i32[0]);
    printf("\n");
    printf("Ri: %x : %d\n", bloc_init.i32[1], i);
    printbits_32(bloc_init.i32[1]);
    printf("\n");
  }

  // Echange de Li et Ri
  bloc_temp.i32 = bloc_init.i32[0];
  // Li = Ri
  bloc_init.i32[0] = bloc_init.i32[1];
  // Ri = Li
  bloc_init.i32[1] = bloc_temp.i32;

  return bloc_init;
}

// Version 1
bloc64 dechiffrement(bloc64 bloc_init) {
  // Li = bloc_init.i32[0]
  // Ri = bloc_init.i32[1]
  // On fait 16 tours de chiffrement
  bloc32 bloc_temp;
  // Echange de Li et Ri
  bloc_temp.i32 = bloc_init.i32[0];
  // Li = Ri
  bloc_init.i32[0] = bloc_init.i32[1];
  // Ri = Li
  bloc_init.i32[1] = bloc_temp.i32;
  for(int i = 16; i > 0; i--) {
    // On sauvegarde le bloc de droite
    bloc_temp.i32 = bloc_init.i32[1];
    // Ri-1 = Li
    bloc_init.i32[1] = bloc_init.i32[0];
    // Li-1 = Ri (+) f(i-1)
    bloc_init.i32[0] = bloc_temp.i32 ^ f(i);
  }
  return bloc_init;
}

// Permutation sur un bloc de 64bits
bloc64 permutation(bloc64 bloc_init, int perm[64]) {
  bloc64 res;
  res.i64 = bloc_init.i64;
  for(int i = 0; i < 64; i++) {
    printf("%d %d\n", i%8, i);
  }
  return res;
}

// Utils
void setbitvalue(uint8_t* v, uint8_t idx, uint8_t value)
{
	if (value == 0)
		*v = *v & ~(1 << idx);
	else
		*v = *v | (1 << idx);
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

uint8_t getbitvalue(uint8_t v, uint8_t idx)
{
	return (v >> idx) & 1;
}
