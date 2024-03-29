#ifndef BIGNUMBERS_H
#define BIGNUMBERS_H
#define True 1
#define False 0
#include <string.h>
#include <stdio.h>
#include <malloc.h>
#include <ctype.h>
#include <stdarg.h>
#include <math.h>
#define MAXLENGTH 19
typedef struct bigNumberDigits *bigNumberDigitsPtr;
typedef struct bigNumber *bigNumberPtr;
typedef struct bigNumberDigits {
    bigNumberDigitsPtr previousChunk;
    unsigned char chunkOfNumber0 : 4;
    unsigned char chunkOfNumber1 : 4;
    bigNumberDigitsPtr nextChunk;
} bigNumberDigits;
typedef struct bigNumber {
    unsigned char isNegative;
    unsigned long long numberLength;
    bigNumberDigits *digits;
} bigNumber;
typedef const char * const stringConstant;
typedef char * String;

void printBigNumber(bigNumberPtr bigNumber);
void freeBigNumber(bigNumberPtr bigNumber);

void printBinaryNumber(long long number);

bigNumberPtr storeBigNumber(stringConstant);

bigNumberPtr addBigNumbers(unsigned long long count, ...);

bigNumberPtr subtractBigNumbers(unsigned long long count, ...);

bigNumberPtr getMaxBigNumber(unsigned long long count, ...);

bigNumberPtr getMinBigNumber(unsigned long long count, ...);

bigNumberPtr firstComplementBigNumber(bigNumberPtr bigNumber);

bigNumberPtr secondComplementBigNumber(bigNumberPtr bigNumber);

#endif
