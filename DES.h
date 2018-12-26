#ifndef des_h
#define des_h

#include <stdio.h>
#include <stdint.h>

typedef union {
  uint32_t i32;
  uint8_t i8[4];
}
bloc32;

typedef
union
{
  uint64_t i64;
  uint32_t i32[2];
  uint16_t i16[4];
  uint8_t i8[8];
}
bloc64;

//bloc64 b;
//b.i64 = 1;

int f(int i);
bloc64 chiffrement(bloc64 bloc_init);
bloc64 dechiffrement(bloc64 bloc_init);
bloc64 permutation(bloc64 bloc_init, int perm[64]);

// Utils
void setbitvalue(uint8_t* v, uint8_t idx, uint8_t value);
void printbits_8(uint8_t v);
void printbits_32(uint32_t v);
uint8_t getbitvalue(uint8_t v, uint8_t idx);

#endif /* coloriage_h */
