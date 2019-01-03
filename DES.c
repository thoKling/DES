#include "DES.h"

#include "MyUtils.h"
#include <math.h>

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

int E[48] = {
    32, 1, 2, 3, 4, 5,
    4, 5, 6, 7, 8, 9,
    8,  9,  10, 11, 12, 13,
    12, 13, 14, 15, 16, 17,
    16, 17, 18, 19, 20, 21,
    20, 21, 22, 23, 24, 25,
    24, 25, 26, 27, 28, 29,
    28, 29, 30, 31, 32, 1};

int shiftage[16] = {
    1,1,2,2,
    2,2,2,2,
    1,2,2,2,
    2,2,2,1
};

int PC1[64] = {
    57,49,41,33,25,17,9,
    1,58,50,42,34,26,18,
    10,2,59,51,43,35,27,
    19,11,3,60,52,44,36,
    63,55,47,39,31,23,15,
    7,62,54,46,38,30,22,
    14,6,61,53,45,37,29,
    21,13,5,28,20,12,4};
int PC2[48]={
    14,17,11,24,1,5,
    3,28,15,6,21,10,
    23,19,12,4,26,8,
    16,7,27,20,13,2,
    41,52,31,37,47,55,
    30,40,51,45,33,48,
    44,49,39,56,34,53,
    46,42,50,36,29,32};


int SBOX[8][4][16] = {
    {
        {14,  4, 13,  1,  2, 15, 11,  8,  3, 10,  6, 12,  5,  9,  0,  7},
        { 0, 15,  7,  4, 14,  2, 13,  1, 10,  6, 12, 11,  9,  5,  3,  8},
        { 4,  1, 14,  8, 13,  6,  2, 11, 15, 12,  9,  7,  3, 10,  5,  0},
        {15, 12,  8,  2,  4,  9,  1,  7,  5, 11,  3, 14, 10,  0,  6, 13},
    },
    
    {
        {15,  1,  8, 14,  6, 11,  3,  4,  9,  7,  2, 13, 12,  0,  5, 10},
        { 3, 13,  4,  7, 15,  2,  8, 14, 12,  0,  1, 10,  6,  9, 11,  5},
        { 0, 14,  7, 11, 10,  4, 13,  1,  5,  8, 12,  6,  9,  3,  2, 15},
        {13,  8, 10,  1,  3, 15,  4,  2, 11,  6,  7, 12,  0,  5, 14,  9},
    },
    
    {
        {10,  0,  9, 14,  6,  3, 15,  5,  1, 13, 12,  7, 11,  4,  2,  8},
        {13,  7,  0,  9,  3,  4,  6, 10,  2,  8,  5, 14, 12, 11, 15,  1},
        {13,  6,  4,  9,  8, 15,  3,  0, 11,  1,  2, 12,  5, 10, 14,  7},
        { 1, 10, 13,  0,  6,  9,  8,  7,  4, 15, 14,  3, 11,  5,  2, 12},
    },
    
    {
        { 7, 13, 14,  3,  0,  6,  9, 10,  1,  2,  8,  5, 11, 12,  4, 15},
        {13,  8, 11,  5,  6, 15,  0,  3,  4,  7,  2, 12,  1, 10, 14,  9},
        {10,  6,  9,  0, 12, 11,  7, 13, 15,  1,  3, 14,  5,  2,  8,  4},
        { 3, 15,  0,  6, 10,  1, 13,  8,  9,  4,  5, 11, 12,  7,  2, 14},
    },
    
    {
        { 2, 12,  4,  1,  7, 10, 11,  6,  8,  5,  3, 15, 13,  0, 14,  9},
        {14, 11,  2, 12,  4,  7, 13,  1,  5,  0, 15, 10,  3,  9,  8,  6},
        { 4,  2,  1, 11, 10, 13,  7,  8, 15,  9, 12,  5,  6,  3,  0, 14},
        {11,  8, 12,  7,  1, 14,  2, 13,  6, 15,  0,  9, 10,  4,  5,  3},
    },
    
    {
        {12,  1, 10, 15,  9,  2,  6,  8,  0, 13,  3,  4, 14,  7,  5, 11},
        {10, 15,  4,  2,  7, 12,  9,  5,  6,  1, 13, 14,  0, 11,  3,  8},
        { 9, 14, 15,  5,  2,  8, 12,  3,  7,  0,  4, 10,  1, 13, 11,  6},
        { 4,  3,  2, 12,  9,  5, 15, 10, 11, 14,  1,  7,  6,  0,  8, 13},
    },
    
    {
        { 4, 11,  2, 14, 15,  0,  8, 13,  3, 12,  9,  7,  5, 10,  6,  1},
        {13,  0, 11,  7,  4,  9,  1, 10, 14,  3,  5, 12,  2, 15,  8,  6},
        { 1,  4, 11, 13, 12,  3,  7, 14, 10, 15,  6,  8,  0,  5,  9,  2},
        { 6, 11, 13,  8,  1,  4, 10,  7,  9,  5,  0, 15, 14,  2,  3, 12},
    },
    
    {
        {13,  2,  8,  4,  6, 15, 11,  1, 10,  9,  3, 14,  5,  0, 12,  7},
        { 1, 15, 13,  8, 10,  3,  7,  4, 12,  5,  6, 11,  0, 14,  9,  2},
        { 7, 11,  4,  1,  9, 12, 14,  2,  0,  6, 10, 13, 15,  3,  5,  8},
        { 2,  1, 14,  7,  4, 10,  8, 13, 15, 12,  9,  0,  3,  5,  6, 11},
    },
    
};
int P[32]={
    16,7,20,21,29,12,28,17,
    1,15,23,26,5,18,31,10,
    2,8,24,14,32,27,3,9,
    19,13,30,6,22,11,4,25};

// Version 1
int f(int i) {
  return i*37453123;
}

bloc64 sKey[16];

// Version 2
uint64_t f_v2(uint32_t R, uint8_t k) {
    bloc64 initR;
    initR.i64 = R;
    //Expansion de R
    permutation(48, &initR, E);
    //printbits_64(initR.i64);

    //printf("\nK :");
    //printbits_64(sKey[0].i64);
    //printf("\n");
    //printbits_64(sKey[0].i64 ^ initR.i64);

    bloc64 temp;
    temp.i64 = sKey[k].i64 ^ initR.i64;

    uint8_t select[8];
    uint8_t tempVal = 0;
    //printf("\n6 bits gang : \n");
    for (int i = 0; i < 48; ++i)
    {
        tempVal |= getbitvalue(temp.i8[5-(i/8)], 7-(i%8)) << (5-(i%6));
        if((i+1) % 6 == 0) {
            //printf("i : %d", i/6);
            select[i/6] = tempVal;
            //printf("\n");
            selection(&select[i/6], SBOX[i/6]);
            //printf("Res:");
            //printbits_8(select[i/6]);
            tempVal = 0;
        }
    }
    bloc64 res;
    res.i64 = 0u;
    // On reassemble les morceaux de 4bits
    for (int i = 0; i < 4; ++i)
    {
        res.i8[7-(i)] = select[i*2] << 4;
        res.i8[7-(i)] |= select[(2*i)+1];
    }
    //printbits_64(res.i64);
    permutationChoice(32, &res, P);
    //printf("\n");
    //printbits_32(res.i32[1]);
  return res.i32[1];
}

// Application de l'ensemble des SBOX a un bloc de 48bits
void substitution(bloc64* v) {
  printbits_64(v->i64);
  printf("\n");
  for(int i = 0; i < 8; i++) {
    bloc64 temp;
    //temp.i64 = v->i64 >> i*6;
    temp.i64 = v->i64 << 6*i;
    printbits_8(temp.i8[5] << 2);
    printf("\n");
    //selection(&temp.i8[5] << 2, SBOX[i]); // res ?
  }
}

// Application d'une SBOX a un ensemble de 6 bits (4bits en retour)
void selection(uint8_t* v, int uSBOX[4][16]) {
  // On calcule i et j
  uint8_t i = getbitvalue(*v, 5)*2 + getbitvalue(*v, 0);
  uint8_t j = 0;
  for(int k = 0; k < 4; k++) {
    j += getbitvalue(*v, 1+k)*pow(2, k);
  }
  //printf("i : %d j : %d -> %d\n", i, j, uSBOX[i][j]);
  // On retourne la valeur selectionnee
  *v = uSBOX[i][j];
  //printbits_8(*v);
  //printf("\n");
}

// Version 1
void chiffrement(bloc64* bloc_init, bloc64* testK) {
  // Permutation initiale
  permutation(64, bloc_init, PI);
  // Generation des clefs
  createSubKeys(sKey, testK->i64);
  // 16 tours de chiffrements  
  uint32_t temp;
  for(int i = 0; i < 16; i++) {
    // On sauvegarde le bloc de gauche
    temp = bloc_init->i32[Li];
    // Li+1 = Ri
    bloc_init->i32[Li] = bloc_init->i32[Ri];
    // Ri+1 = Li (+) f(i)
    printf("I : %"PRIx64"\n", sKey[i].i64);
    bloc_init->i32[Ri] = temp ^ f_v2(bloc_init->i32[Ri], i);
  }
  // On échange
  temp = bloc_init->i32[Li];
  bloc_init->i32[Li] = bloc_init->i32[Ri];
  bloc_init->i32[Ri] = temp;
  // Permutation inverse
  permutation(64, bloc_init, PI_INV);
}

// Version 1
void dechiffrement(bloc64* bloc_init, bloc64* testK) {
  // Permutation initiale
  permutation(64, bloc_init, PI);
  // Generation des clefs
  createSubKeys(sKey, testK->i64);
  
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
    bloc_init->i32[Li] = temp ^ f_v2(bloc_init->i32[Ri], i);
  }
  // Permutation inverse
  permutation(64, bloc_init, PI_INV);
}

// Permutation sur un bloc de 64bits
void permutation(int taille, bloc64* bloc_init, int* perm) {
  bloc64 res;
  res.i64 = 0u;
  for(int i = 0; i < taille; i++) {
    //printf("Tour %d : valPerm : %d valBit : %d \n", i, perm[i], getbitvalue(bloc_init->i64, perm[i]-1));
    // On cast car getbitvalue() renvoie un uint_8
    // On enleve 1 à la perm car getBit regarde l'index et index commence a 0
    res.i64 |= (long long unsigned int)(getbitvalue(bloc_init->i64, perm[i]-1)) << i; 
  }
  bloc_init->i64 = res.i64;
}
// Permutation avec lecture dans l'autre sens des bits
void permutationChoice(int taille, bloc64* bloc_init, int* PC) {
    bloc64 res;
    res.i64 = 0u;
    for (int i = 0; i < taille; ++i)
    {
        res.i64 |= (long long unsigned int)(getbitvalue(bloc_init->i64, (64-PC[i]))) << (63-i); 
    }
    bloc_init->i64 = res.i64;
}

// Fonction qui permet de shifter a gauche en repassant le premier bit a la fin
void cycledShift(bloc64* bloc_init, int shift) {
    // On shift "normalement"
    bloc_init->i32[0] = bloc_init->i32[0] << shift;
    // On regarde si les bits en positions 28 et 29 sont maintenants à 1
    if(getbitvalue(bloc_init->i32[0], 28) && getbitvalue(bloc_init->i32[0], 29)) {
        setbitvalue(&bloc_init->i8[0], 0, 1);

        setbitvalue(&bloc_init->i8[0], 1, 1);
    }
    else if(getbitvalue(bloc_init->i32[0], 28)) {
        setbitvalue(&bloc_init->i8[0], 0, 1);
    }
    else if(getbitvalue(bloc_init->i32[0], 29)) {
        setbitvalue(&bloc_init->i8[0], 1, 1);
    }
    // Nettoyage des bits en position 28 et 29 
    setbitvalue(&bloc_init->i8[3], 5, 0);
    setbitvalue(&bloc_init->i8[3], 4, 0);
}

void createSubKeys(bloc64 subKeys[16], uint64_t initKey) {
  bloc64 init_key;
  init_key.i64 = initKey;
  // On applique PC1
  permutationChoice(56, &init_key, PC1);

  // On coupe le resultat de la permutation en deux moitiée de 28bits
  bloc64 D0;
  D0.i64 = init_key.i64 << 24;
  D0.i32[0] = D0.i32[1];
  D0.i32[1] = 0u;
  // Nettoyage des bits en position 28 à 31 
  setbitvalue(&D0.i8[3], 7, 0);
  setbitvalue(&D0.i8[3], 6, 0);
  setbitvalue(&D0.i8[3], 5, 0);
  setbitvalue(&D0.i8[3], 4, 0);

  bloc64 C0;
  C0.i64 = init_key.i64 >> 36;

  for (int i = 0; i < 16; ++i)
    {
        // On effectue le shift cyclique vers la gauche
        cycledShift(&C0, shiftage[i]);
        cycledShift(&D0, shiftage[i]);

        // On reforme la clef en reassmemblant Ci et Di
        bloc64 K;
        K.i64 = 0u;
        K.i64 = (uint64_t)(C0.i32[0]) << 36;
        // On met les 4 premiers bits de D dans les 4 derniers de K.i32[1]
        setbitvalue(&K.i8[4], 0, getbitvalue(D0.i8[3], 0));
        setbitvalue(&K.i8[4], 1, getbitvalue(D0.i8[3], 1));
        setbitvalue(&K.i8[4], 2, getbitvalue(D0.i8[3], 2));
        setbitvalue(&K.i8[4], 3, getbitvalue(D0.i8[3], 3));
        K.i32[0] = D0.i32[0] << 8;

        // On applique PC2
        permutationChoice(48, &K, PC2);
        //printbits_64(K.i64);
        //printf("\n");
        // Maraboutage
        subKeys[i].i64 = K.i64 >> 16;
    }

}