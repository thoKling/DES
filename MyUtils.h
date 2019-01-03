#ifndef MyUtils_h
#define MyUtils_h

#include <stdio.h>
#include <inttypes.h>
#include <stdint.h>

// Utils
uint8_t getbitvalue(uint64_t v, uint8_t idx);

void setbitvalue(uint8_t* v, uint8_t idx, uint8_t value);

void testSet(uint64_t* v, uint8_t idx, uint8_t value);

void printbits_8(uint8_t v);

void printbits_32(uint32_t v);

void printbits_64(uint64_t v);

#endif /* MyUtils_h */