#include "Bignumbers.h"

bigNumberPtr storeBigNumber(stringConstant number) {
    if (!number || !*number)
        return NULL;
    String tempNumberString = strdup(number);
    String tempNumberStringPtr = tempNumberString;
    unsigned long long numberLength = strlen(tempNumberStringPtr);
    unsigned long long numberCounter = 0;
    unsigned char settings = 0, currentChar = 0;
    for (; (currentChar = *tempNumberStringPtr);) {
        if (!isdigit(currentChar)) {
            if (!(settings & 0x1)) {
                if (currentChar == '-') {
                    if (settings & 0x2) {
                        settings &= 0xFD;
                    } else {
                        settings |= 0x2;
                    }
                    tempNumberStringPtr++;
                    numberLength--;
                } else if (currentChar == '+' || currentChar == ' ') {
                    tempNumberStringPtr++;
                    numberLength--;
                } else {
                    free(tempNumberString);
                    return NULL;
                }
            } else {
                free(tempNumberString);
                return NULL;
            }
        } else {
            if (currentChar == '0' && numberLength == 1) {
                settings |= 0x1;
                break;
            }
            if (currentChar != '0') {
                settings |= 0x1;
                break;
            }
            numberLength--;
        }
    }
    if (!numberLength)
        return NULL;
    bigNumberDigitsPtr Tail = (bigNumberDigitsPtr) calloc(1, sizeof(bigNumber));
    bigNumberPtr internalBigNumber = (bigNumberPtr) malloc(sizeof(bigNumber));
    bigNumberDigitsPtr nextChunk = NULL;
    internalBigNumber->isNegative = 0;
    internalBigNumber->numberLength = numberLength;
    bigNumberDigitsPtr tempNumber = Tail;
    do {
        currentChar = *(tempNumberStringPtr + numberLength - numberCounter - 1);
        if (!isdigit(currentChar)) {
            freeBigNumber(internalBigNumber);
            free(tempNumberString);
            return NULL;
        } else {
            if (settings & 0x4) {
                tempNumber->chunkOfNumber0 = (unsigned char) (currentChar - '0');
                if ((numberCounter + 1) < numberLength) {
                    nextChunk = tempNumber;
                    tempNumber->previousChunk = (bigNumberDigitsPtr) calloc(1, sizeof(bigNumberDigits));
                    tempNumber = tempNumber->previousChunk;
                    tempNumber->nextChunk = nextChunk;
                }
                settings &= 0xFB;
            } else {
                tempNumber->chunkOfNumber1 = (char) (currentChar - '0');
                settings |= 0x4;
            }
        }
        numberCounter++;
    } while (numberCounter < numberLength && currentChar);
    free(tempNumberString);
    Tail->nextChunk = tempNumber;
    tempNumber->previousChunk = Tail;
    internalBigNumber->digits = tempNumber;
    if (settings & 0x2)
        internalBigNumber->isNegative = 1;
    return internalBigNumber;
}

static bigNumberPtr _addBigNumbers(bigNumberPtr firstNumber, bigNumberPtr secondNumber) {
    if (!firstNumber || !firstNumber->digits || !secondNumber || !secondNumber->digits) return NULL;
    bigNumberPtr AdditionNumber = (bigNumberPtr) calloc(1, sizeof(bigNumber));
    bigNumberDigitsPtr Tail = (bigNumberDigitsPtr) calloc(1, sizeof(bigNumberDigits));
    bigNumberDigitsPtr additionDigits = Tail;
    bigNumberDigitsPtr firstNumberDigits = firstNumber->digits->previousChunk;
    bigNumberDigitsPtr secondNumberDigits = secondNumber->digits->previousChunk;
    bigNumberDigitsPtr nextChunk = NULL;
    bigNumberDigitsPtr dominant = NULL;
    unsigned char opResult = 0;
    unsigned long long counter = 0, minLength = 0, maxLength = 0;
    opResult =
            0x40 | ((AdditionNumber->isNegative = ((firstNumber->isNegative & 1) && (secondNumber->isNegative & 1))) ||
                    ((!(firstNumber->isNegative & 1) && !(secondNumber->isNegative & 1))));
    AdditionNumber->numberLength = (firstNumber->numberLength > secondNumber->numberLength)
                                   ? (dominant = firstNumber->digits->previousChunk,
                                      maxLength = firstNumber->numberLength,
                                      minLength = secondNumber->numberLength,
                    firstNumber->numberLength)
                                   : (dominant = secondNumber->digits->previousChunk,
                                      maxLength = secondNumber->numberLength,
                                      minLength = firstNumber->numberLength,
                    secondNumber->numberLength);
    while (counter < maxLength || (opResult & 0x80)) {
        if (opResult & 0x40) {
            if (opResult & 0x20) {
                if (counter < minLength) {
                    opResult = 0x40 | (firstNumberDigits->chunkOfNumber0 +
                                       secondNumberDigits->chunkOfNumber0 + ((opResult & 0x80) >> 7));
                    if ((opResult & 0x1F) > 9)
                        opResult = 0x80 | 0x40 | ((opResult & 0x1F) - 10);
                    firstNumberDigits = firstNumberDigits->previousChunk;
                    secondNumberDigits = secondNumberDigits->previousChunk;
                } else {
                    opResult = 0x40 | (dominant->chunkOfNumber0 + ((opResult & 0x80) >> 7));
                    if ((opResult & 0x1F) > 9)
                        opResult = 0x80 | 0x40 | ((opResult & 0x1F) - 10);
                }
                dominant = dominant->previousChunk;
                additionDigits->chunkOfNumber0 = opResult & 0x1F;
                nextChunk = additionDigits;
                if ((counter + 1) < maxLength || (opResult & 0x80)) {
                    additionDigits->previousChunk = (bigNumberDigitsPtr) calloc(1, sizeof(bigNumberDigits));
                    additionDigits = additionDigits->previousChunk;
                    additionDigits->nextChunk = nextChunk;
                }
            } else {
                if (counter < minLength) {
                    opResult = 0x40 | 0x20 | (firstNumberDigits->chunkOfNumber1 +
                                              secondNumberDigits->chunkOfNumber1 + ((opResult & 0x80) >> 7));
                    if ((opResult & 0x1F) > 9)
                        opResult = 0x80 | 0x20 | (opResult & 0x40) | ((opResult & 0x1F) - 10);
                    additionDigits->chunkOfNumber1 = opResult & 0x1F;
                } else if (counter < maxLength) {
                    opResult = 0x40 | 0x20 | (dominant->chunkOfNumber1 + ((opResult & 0x80) >> 7));
                    if ((opResult & 0x1F) > 9)
                        opResult = 0x80 | 0x20 | (opResult & 0x40) | ((opResult & 0x1F) - 10);
                    additionDigits->chunkOfNumber1 = opResult & 0x1F;
                } else {
                    opResult = 0x40 | 0x20 | ((opResult & 0x80) >> 7);
                    if ((opResult & 0x1F) > 9)
                        opResult = 0x80 | 0x20 | (opResult & 0x40) | ((opResult & 0x1F) - 10);
                    additionDigits->chunkOfNumber1 = opResult & 0x1F;
                    AdditionNumber->numberLength++;
                }
            }
        } else {

        }
        counter++;
    }
    Tail->nextChunk = additionDigits;
    additionDigits->previousChunk = Tail;
    AdditionNumber->digits = additionDigits;
    return AdditionNumber;
}

bigNumberPtr addBigNumbers(unsigned long long count, ...) {
    if (count < 2)
        return NULL;
    va_list addList;
    bigNumberPtr firstNumber = NULL;
    bigNumberPtr secondNumber = NULL;
    bigNumberPtr additionNumber = NULL;
    va_start(addList, count);
    for (int i = 0; i < count;) {
        if (!i) {
            firstNumber = va_arg(addList, bigNumberPtr);
            secondNumber = va_arg(addList, bigNumberPtr);
            i += 2;
            additionNumber = _addBigNumbers(firstNumber, secondNumber);
        } else {
            firstNumber = additionNumber;
            secondNumber = va_arg(addList, bigNumberPtr);
            additionNumber = _addBigNumbers(firstNumber, secondNumber);
            freeBigNumber(firstNumber);
            i++;
        }
    }
    return additionNumber;
}

bigNumberPtr subtractbigNumbers();

void printBigNumber(bigNumberPtr bigNumber) {
    if (!bigNumber)
        return;
    bigNumberDigitsPtr tempPtr = bigNumber->digits;
    if (bigNumber->isNegative)
        printf("-");
    unsigned long long counter = 0;
    char started = 0;
    while (counter < bigNumber->numberLength) {
        if (!started && tempPtr->chunkOfNumber0 != 0)
            started = 1;
        if (started)
            printf("%d", tempPtr->chunkOfNumber0);
        if (!started && tempPtr->chunkOfNumber1 != 0)
            started = 1;
        if (started)
            printf("%d", tempPtr->chunkOfNumber1);
        counter += 2;
        tempPtr = tempPtr->nextChunk;
    }
    printf("\n");
}

void freeBigNumber(bigNumberPtr internalBigNumber) {
    if (!internalBigNumber)
        return;
    bigNumberDigitsPtr tempPtr = internalBigNumber->digits;
    bigNumberDigitsPtr nextBigNumber = NULL;
    unsigned long long counter = 0;
    while (counter < internalBigNumber->numberLength) {
        nextBigNumber = tempPtr->nextChunk;
        free(tempPtr);
        counter += 2;
        tempPtr = nextBigNumber;
    }
    free(internalBigNumber);
}

void printBinaryNumber(long long number) {
    String binaryData = (String) calloc(65, sizeof(char));
    char wordCounter = 63;
    while (wordCounter >= 0) {
        binaryData[wordCounter--] = ((number % 2) + '0');
        number /= 2;
    }
    printf("%s\n", binaryData);
    free(binaryData);
}
