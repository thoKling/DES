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
  bloc32 bloc_temp;
  for(int i = 0; i < 16; i++) {
    // On sauvegarde le bloc de gauche
    bloc_temp.i32 = bloc_init.i32[0];
    // Li+1 = Ri
    bloc_init.i32[0] = bloc_init.i32[1];
    // Ri+1 = Li (+) f(i)
    bloc_init.i32[1] = bloc_temp.i32 ^ f(i);
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
  for(int i = 15; i > -1; i--) {
    // On sauvegarde le bloc de droite
    bloc_temp.i32 = bloc_init.i32[1];
    // Ri-1 = Li
    bloc_init.i32[1] = bloc_init.i32[0];
    // Li-1 = Ri (+) f(i-1)
    bloc_init.i32[0] = bloc_temp.i32 ^ f(i);
  }
  return bloc_init;
}
