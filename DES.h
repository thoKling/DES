#ifndef des_h
#define des_h

#include <stdint.h>

typedef union {
  uint32_t i32;
  uint8_t i8[4];
}
bloc32;

typedef union
{
  uint64_t i64;
  uint32_t i32[2];
  uint16_t i16[4];
  uint8_t i8[8];
}
bloc64;

int f(int i);
void chiffrement(bloc64* bloc_init);
void dechiffrement(bloc64* bloc_init);
void permutation(int taille, bloc64* bloc_init, int* perm);
void permutationChoice(int taille, bloc64* bloc_init, int* PC);
void cycledShift(bloc64* bloc_init, int shift);
void createSubKeys(bloc64 subKeys[16], uint64_t initKey);
void selection(uint8_t* v, int SBOX[4][16]);
void substitution(bloc64* v);

#endif /* DES_h */
