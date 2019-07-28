#include "Bignumbers.h"

specialNumberPtr storeSpecialNumber(stringConstant number){
    if (!number || !*number)
        return NULL;
    String tempNumberString = strdup(number);
    String tempNumberStringPtr = tempNumberString;
    unsigned long long numberLength = 0;
    unsigned char settings = 0, currentChar = 0;
    for(;(currentChar = *tempNumberStringPtr);) {
        if (!isdigit(currentChar)) {
            if (!(settings & (unsigned) 0x1)) {
                if (currentChar == '-') {
                    if (settings & (unsigned) 0x2) {
                        settings &= (unsigned) 0xFD;
                    } else {
                        settings |= (unsigned) 0x2;
                    }
                    tempNumberStringPtr++;
                } else if (currentChar == '+') {
                    tempNumberStringPtr++;
                } else {
                    free(tempNumberString);
                    return NULL;
                }
            }else{
                free(tempNumberString);
                return NULL;
            }
        }
        else {
            settings |= (unsigned) 0x1;
            break;
        }
    }
    String tempNumberChunk = NULL;
    specialNumberPtr specialNumber = NULL;
    specialNumberPtr Head = NULL;
    long long tempNumber;
    do{
        tempNumber = 0;
        numberLength = strlen(tempNumberStringPtr);
        if (numberLength > 0 && numberLength <= MAXLENGTH){
            tempNumberChunk = (String) calloc(numberLength+1, sizeof(char));
            strncpy(tempNumberChunk, tempNumberStringPtr, numberLength);
            tempNumberStringPtr = NULL;
        }else if (numberLength > MAXLENGTH){
            tempNumberChunk = (String) calloc(MAXLENGTH , sizeof(char));
            strncpy(tempNumberChunk, tempNumberStringPtr, MAXLENGTH);
            tempNumberStringPtr += MAXLENGTH;
            numberLength = MAXLENGTH;
        }else{
            free(tempNumberChunk);
            free(tempNumberString);
            return specialNumber;
        }
        for(unsigned char i = 0; i < (unsigned char)numberLength && (currentChar = *(tempNumberChunk + i)); i++) {
            if (!isdigit(currentChar)) {
                free(tempNumberChunk);
                free(tempNumberString);
                freeSpecialNumber(Head);
                return NULL;
            }
            else {
                tempNumber *= 10;
                tempNumber += currentChar - '0';
            }
        }
        if (!Head) {
            if (settings & (unsigned)0x2)
                tempNumber *= -1;
            Head = (specialNumberPtr) malloc(sizeof(specialNumber));
            specialNumber = Head;
        }
        specialNumber->chunkOfNumber = tempNumber;
        if (tempNumberStringPtr) {
            specialNumber->nextChunk = (specialNumberPtr) malloc(sizeof(specialNumber));
            specialNumber = specialNumber->nextChunk;
        }
        free(tempNumberChunk);
    }while(tempNumberStringPtr);
    free(tempNumberString);
    return Head;
}
specialNumberPtr addSpecialNumbers(specialNumberPtr firstNum, specialNumberPtr secondNumber, ...){

}
void printSpecialNumber(specialNumberPtr specialNumber){
    while(specialNumber){
        printf("%lld", specialNumber->chunkOfNumber);
        specialNumber = specialNumber->nextChunk;
    }
    printf("\n");
}

void freeSpecialNumber(specialNumberPtr specialNumber){
    while(specialNumber){
        specialNumberPtr nextSpecialNumber = specialNumber->nextChunk;
        free(specialNumber);
        specialNumber = nextSpecialNumber;
    }
}
                                                                                                
