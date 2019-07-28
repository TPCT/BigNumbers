#ifndef BIGNUMBERS_H
#define BIGNUMBERS_H
#include <string.h>
#include <stdio.h>
#include <malloc.h>
#include <ctype.h>
#include <stdarg.h>
#define MAXLENGTH 19
typedef struct specialNumber * specialNumberPtr;
typedef struct specialNumber {
    long long chunkOfNumber;
    specialNumberPtr nextChunk;
} specialNumber;
typedef const char * const stringConstant;
typedef char * String;

void printSpecialNumber(specialNumberPtr specialNumber);
void freeSpecialNumber(specialNumberPtr specialNumber);
specialNumberPtr storeSpecialNumber(stringConstant);
specialNumberPtr addSpecialNumbers(specialNumberPtr firstNum, specialNumberPtr secondNumber, ...);

#endif
