#include "DES.h"
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <inttypes.h>
#include "MyUtils.h"

int main(){

    // Truc utile pour tester le temps d'exec
    //clock_t begin = clock();
    //Fonction a tester le temps
    //clock_t stop = clock();
    //double time_spent =(double)(stop-begin)/CLOCKS_PER_SEC*1000;
    //printf("Temps d'execution : %f\n", time_spent);

    bloc64 init;
    init.i64 = 0b0000000100100011010001010110011110001001101010111100110111101111;
    printf("Debut chiffre : %"PRIx64"\n", init.i64);
    chiffrement(&init);
    printf("Debut chiffre : %"PRIx64"\n", init.i64);
    dechiffrement(&init);
    printf("Debut chiffre : %"PRIx64"\n", init.i64);
    //printbits_64(init.i64);
    //printf("\n");
    //chiffrement(&init);
    /*bloc64 test;
    test.i64 = 0b11110000101010101111000010101010;
    //printbits_64(test.i64);
    int E[48] = {
    32, 1, 2, 3, 4, 5,
    4, 5, 6, 7, 8, 9,
    8,  9,  10, 11, 12, 13,
    12, 13, 14, 15, 16, 17,
    16, 17, 18, 19, 20, 21,
    20, 21, 22, 23, 24, 25,
    24, 25, 26, 27, 28, 29,
    28, 29, 30, 31, 32, 1};
int P[32]={
    16,7,20,21,29,12,28,17,
    1,15,23,26,5,18,31,10,
    2,8,24,14,32,27,3,9,
    19,13,30,6,22,11,4,25};
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
    }
};
    printf("E(R) :");
    permutation(48, &test, E);
    printbits_64(test.i64);
    bloc64 testK;
    testK.i64 = 0b0001001100110100010101110111100110011011101111001101111111110001; 
    bloc64 sKey[16];
    createSubKeys(sKey, testK.i64);
    printf("\nK :");
    printbits_64(sKey[0].i64);
    printf("\n");
    printbits_64(sKey[0].i64 ^ test.i64);

    bloc64 temp;
    temp.i64 = sKey[0].i64 ^ test.i64;

    uint8_t select[8];
    uint8_t tempVal = 0;
    printf("\n6 bits gang : \n");
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
    printbits_64(res.i64);
    permutationChoice(32, &res, P);
    printf("\n");
    printbits_32(res.i32[1]);*/
    /*
    uint8_t toto = 0b011011;
    printf("\n");
    printbits_8(toto);
    printf("\n");
    selection(&toto, SBOX[0]);
    printbits_8(toto);
    */
    /*
    printf("Fin chiffre : %"PRIx64"\n", test.i64);
    dechiffrement(&test);
    printf("Fin dechiffre : %"PRIx64"\n", test.i64);  */
    //selection(0b00111111);
/*
    bloc64 test;
    test.i64 = 0b0001001100110100010101110111100110011011101111001101111111110001; 
    bloc64 sKey[16];
    createSubKeys(sKey, test.i64);
    for(int i = 0; i < 16; i++) {
        printf("K%d : ", i+1);
        printbits_64(sKey[i].i64);
        printf("\n");
    }*/
    //substitution(&test);
    /*printbits_64(0xFFFFFFFFFFFFFFFF);
    printf("\n");
    printbits_64(0xFFFFFFFFFFFFFFFF << 1);*/
    return 0;
}
