#include "DES.h"

#include "MyUtils.h"

// On inverse Li et Ri a cause de l'architecture little Endian
const uint8_t Li = 1;
const uint8_t Ri = 0;

//La permutation initiale et son inverse

int PI[64] = {
    58,50,42,34,26,18,10,2,
    60,52,44,36,28,20,12,4,
    62,54,46,38,30,22,14,6,
    64,56,48,40,32,24,16,8,
    57,49,41,33,25,17,9,1,
    59,51,43,35,27,19,11,3,
    61,53,45,37,29,21,13,5,
    63,55,47,39,31,23,15,7};

int PI_INV[64] = {
    40,8,48,16,56,24,64,32,
    39,7,47,15,55,23,63,31,
    38,6,46,14,54,22,62,30,
    37,5,45,13,53,21,61,29,
    36,4,44,12,52,20,60,28,
    35,3,43,11,51,19,59,27,
    34,2,42,10,50,18,58,26,
    33,1,41,9,49,17,57,25};


// Version 1
int f(int i) {
  return i*37453123;
}

// Version 1
void chiffrement(bloc64* bloc_init) {
  // Permutation initiale
  permutation(64, bloc_init, PI);
  // 16 tours de chiffrements  
  uint32_t temp;
  for(int i = 0; i < 16; i++) {
    // On sauvegarde le bloc de gauche
    temp = bloc_init->i32[Li];
    // Li+1 = Ri
    bloc_init->i32[Li] = bloc_init->i32[Ri];
    // Ri+1 = Li (+) f(i)
    bloc_init->i32[Ri] = temp ^ f(i);
  }
  // On échange
  temp = bloc_init->i32[Li];
  bloc_init->i32[Li] = bloc_init->i32[Ri];
  bloc_init->i32[Ri] = temp;
  // Permutation inverse
  permutation(64, bloc_init, PI_INV);
}

// Version 1
void dechiffrement(bloc64* bloc_init) {
  // Permutation initiale
  permutation(64, bloc_init, PI);

  uint32_t temp;
  // Echange de Li et Ri
  temp = bloc_init->i32[Li];
  bloc_init->i32[Li] = bloc_init->i32[Ri];
  bloc_init->i32[Ri] = temp;
  // 16 tours de dechiffrements
  for(int i = 15; i > -1; i--) {
    // On sauvegarde le bloc de droite
    temp = bloc_init->i32[Ri];
    // Ri-1 = Li
    bloc_init->i32[Ri] = bloc_init->i32[Li];
    // Li-1 = Ri (+) f(i-1)
    bloc_init->i32[Li] = temp ^ f(i);
  }
  // Permutation inverse
  permutation(64, bloc_init, PI_INV);
}

// Permutation sur un bloc de 64bits
void permutation(int taille, bloc64* bloc_init, int* perm) {
  bloc64 res;
  res.i64 = 0u;
  for(int i = 0; i < taille; i++) {
    // On cast car getbitvalue() renvoie un uint_8
    res.i64 |= (long long unsigned int)(getbitvalue(bloc_init->i64, perm[i]-1)) << i;
  }
  bloc_init->i64 = res.i64;
}

