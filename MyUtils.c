#include "MyUtils.h"

#include "DES.h"

void printbits_32(uint32_t v)
{
	bloc32* b = (bloc32*)(&v);
	for (int i = 3; i > -1; --i)
	{
		printbits_8(b->i8[i]);
		printf(" ");
	}
}
void printbits_64(uint64_t v)
{
	bloc64* b = (bloc64*)(&v);
	for (int i = 7; i > -1; --i)
	{
		printbits_8(b->i8[i]);
		printf(" ");
	}
}
void setbitvalue(uint8_t* v, uint8_t idx, uint8_t value)
{
	if (value == 0)
		*v = *v & ~(1 << idx);
	else
		*v = *v | (1 << idx);
}
void testSet(uint64_t* v, uint8_t idx, uint8_t value)
{
	if (value == 0)
		*v = *v & ~(1 << idx);
	else
		*v = *v | (1 << idx);
}
uint8_t getbitvalue(uint64_t v, uint8_t idx)
{
	return (v >> idx) & 1;
}

void printbits_8(uint8_t v)
{
	for (uint8_t i = 8; i > 0; --i)
		printf("%u", getbitvalue(v, i-1));
}