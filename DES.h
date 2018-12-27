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
void permutation(int taille, bloc64* bloc_init, int perm[64]);

#endif /* DES_h */
