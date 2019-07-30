#include "Bignumbers.h"

static bigNumberPtr _specialGetMaxBigNumber(bigNumberPtr firstNumber, bigNumberPtr secondNumber);

static bigNumberPtr _specialGetMinBigNumber(bigNumberPtr firstNumber, bigNumberPtr secondNumber);

static bigNumberPtr _addBigNumbers(bigNumberPtr firstNumber, bigNumberPtr secondNumber);

static bigNumberPtr _subtractBigNumbers(bigNumberPtr firstNumber, bigNumberPtr secondNumber);

static bigNumberPtr _getMaxBigNumber(bigNumberPtr firstNumber, bigNumberPtr secondNumber);

static bigNumberPtr _getMinBigNumber(bigNumberPtr firstNumber, bigNumberPtr secondNumber);

static bigNumberPtr _firstComplementBigNumber(bigNumberPtr firstNumber);

static bigNumberPtr _secondComplementBigNumber(bigNumberPtr firstNumber);

static void _BigNumberCpy(bigNumberPtr *destNumber, bigNumberPtr srcNumber) {
    if (!destNumber || !srcNumber)
        return;
    if (!*destNumber)
        *destNumber = (bigNumberPtr) calloc(1, sizeof(bigNumber));
    (*destNumber)->numberLength = srcNumber->numberLength;
    (*destNumber)->isNegative = srcNumber->isNegative;
    (*destNumber)->digits = (bigNumberDigitsPtr) calloc(1, sizeof(bigNumberDigits));
    bigNumberDigitsPtr srcNumberDigits = srcNumber->digits->previousChunk;
    bigNumberDigitsPtr destNumberDigits = (*destNumber)->digits;
    bigNumberDigitsPtr nextChunk = NULL;
    unsigned long long maxLength = srcNumber->numberLength;
    if (srcNumber->numberLength % 2 != 0)
        maxLength++;
    for (unsigned long long i = 0; i < maxLength; i += 2) {
        destNumberDigits->chunkOfNumber0 = srcNumberDigits->chunkOfNumber0;
        destNumberDigits->chunkOfNumber1 = srcNumberDigits->chunkOfNumber1;
        srcNumberDigits = srcNumberDigits->previousChunk;
        if ((i + 2) < maxLength) {
            nextChunk = destNumberDigits;
            destNumberDigits->previousChunk = (bigNumberDigitsPtr) calloc(1, sizeof(bigNumberDigits));
            destNumberDigits = destNumberDigits->previousChunk;
            destNumberDigits->nextChunk = nextChunk;
        }
    }
    destNumberDigits->previousChunk = (*destNumber)->digits;
    (*destNumber)->digits->nextChunk = destNumberDigits;
    (*destNumber)->digits = (*destNumber)->digits->nextChunk;
}

static bigNumberPtr _specialGetMaxBigNumber(bigNumberPtr firstNumber, bigNumberPtr secondNumber) {
    if (!firstNumber || !firstNumber->digits) return secondNumber;
    if (!secondNumber || !secondNumber->digits) return firstNumber;
    bigNumberDigitsPtr firstNumberDigits = firstNumber->digits;
    bigNumberDigitsPtr secondNumberDigits = secondNumber->digits;
    bigNumberPtr maxBigNumber = NULL;
    if ((!firstNumber->isNegative &&
         (secondNumber->isNegative || firstNumber->numberLength > secondNumber->numberLength)) ||
        (firstNumber->isNegative && secondNumber->isNegative &&
         (firstNumber->numberLength < secondNumber->numberLength)))
        maxBigNumber = firstNumber;
    else if ((!secondNumber->isNegative &&
              (firstNumber->isNegative || secondNumber->numberLength > firstNumber->numberLength)) || (
                     firstNumber->isNegative && secondNumber->isNegative &&
                     (firstNumber->numberLength > secondNumber->numberLength)))
        maxBigNumber = secondNumber;
    else {
        unsigned long long counter = 0;
        while (counter < firstNumber->numberLength) {
            if (!firstNumber->isNegative && !secondNumber->isNegative) {
                if (firstNumberDigits->chunkOfNumber0 > secondNumberDigits->chunkOfNumber0) {
                    maxBigNumber = firstNumber;
                    break;
                } else if (firstNumberDigits->chunkOfNumber0 < secondNumberDigits->chunkOfNumber0) {
                    maxBigNumber = secondNumber;
                    break;
                } else {
                    if (firstNumberDigits->chunkOfNumber1 > secondNumberDigits->chunkOfNumber1) {
                        maxBigNumber = firstNumber;
                        break;
                    } else if (firstNumberDigits->chunkOfNumber1 < secondNumberDigits->chunkOfNumber1) {
                        maxBigNumber = secondNumber;
                        break;
                    }
                }
            } else {
                if (firstNumberDigits->chunkOfNumber0 > secondNumberDigits->chunkOfNumber0) {
                    maxBigNumber = firstNumber;
                    break;
                } else if (firstNumberDigits->chunkOfNumber0 < secondNumberDigits->chunkOfNumber0) {
                    maxBigNumber = firstNumber;
                    break;
                } else {
                    if (firstNumberDigits->chunkOfNumber1 > secondNumberDigits->chunkOfNumber1) {
                        maxBigNumber = secondNumber;
                        break;
                    } else if (firstNumberDigits->chunkOfNumber1 < secondNumberDigits->chunkOfNumber1) {
                        maxBigNumber = firstNumber;
                        break;
                    }
                }
            }
            firstNumberDigits = firstNumberDigits->nextChunk;
            secondNumberDigits = secondNumberDigits->nextChunk;
            counter += 2;
        }
    }
    if (!maxBigNumber) {
        maxBigNumber = firstNumber;
    }
    return maxBigNumber;

}

static bigNumberPtr _specialGetMinBigNumber(bigNumberPtr firstNumber, bigNumberPtr secondNumber) {
    if (!firstNumber || !firstNumber->digits) return secondNumber;
    if (!secondNumber || !secondNumber->digits) return firstNumber;
    bigNumberDigitsPtr firstNumberDigits = firstNumber->digits;
    bigNumberDigitsPtr secondNumberDigits = secondNumber->digits;
    bigNumberPtr minBigNumber = NULL;
    if ((!firstNumber->isNegative &&
         (secondNumber->isNegative || firstNumber->numberLength > secondNumber->numberLength)) ||
        (firstNumber->isNegative && secondNumber->isNegative &&
         (firstNumber->numberLength < secondNumber->numberLength)))
        minBigNumber = secondNumber;
    else if ((!secondNumber->isNegative &&
              (firstNumber->isNegative || secondNumber->numberLength > firstNumber->numberLength)) || (
                     firstNumber->isNegative && secondNumber->isNegative &&
                     (firstNumber->numberLength > secondNumber->numberLength)))
        minBigNumber = firstNumber;
    else {
        unsigned long long counter = 0;
        while (counter < firstNumber->numberLength) {
            if (!firstNumber->isNegative && !secondNumber->isNegative) {
                if (firstNumberDigits->chunkOfNumber0 > secondNumberDigits->chunkOfNumber0) {
                    minBigNumber = secondNumber;
                    break;
                } else if (firstNumberDigits->chunkOfNumber0 < secondNumberDigits->chunkOfNumber0) {
                    minBigNumber = firstNumber;
                    break;
                } else {
                    if (firstNumberDigits->chunkOfNumber1 > secondNumberDigits->chunkOfNumber1) {
                        minBigNumber = secondNumber;
                        break;
                    } else if (firstNumberDigits->chunkOfNumber1 < secondNumberDigits->chunkOfNumber1) {
                        minBigNumber = firstNumber;
                        break;
                    }
                }
            } else {
                if (firstNumberDigits->chunkOfNumber0 > secondNumberDigits->chunkOfNumber0) {
                    minBigNumber = firstNumber;
                    break;
                } else if (firstNumberDigits->chunkOfNumber0 < secondNumberDigits->chunkOfNumber0) {
                    minBigNumber = secondNumber;
                    break;
                } else {
                    if (firstNumberDigits->chunkOfNumber1 > secondNumberDigits->chunkOfNumber1) {
                        minBigNumber = firstNumber;
                        break;
                    } else if (firstNumberDigits->chunkOfNumber1 < secondNumberDigits->chunkOfNumber1) {
                        minBigNumber = secondNumber;
                        break;
                    }
                }
            }
            firstNumberDigits = firstNumberDigits->nextChunk;
            secondNumberDigits = secondNumberDigits->nextChunk;
            counter += 2;
        }
    }
    if (!minBigNumber) {
        minBigNumber = firstNumber;
    }
    return minBigNumber;

}

static bigNumberPtr _addBigNumbers(bigNumberPtr firstNumber, bigNumberPtr secondNumber) {
    if (!firstNumber || !firstNumber->digits) return secondNumber;
    if (!secondNumber || !secondNumber->digits) return firstNumber;
    bigNumberPtr AdditionNumber;
    if (firstNumber->isNegative != secondNumber->isNegative) {
        AdditionNumber = _subtractBigNumbers(firstNumber, secondNumber);
        return AdditionNumber;
    }
    AdditionNumber = (bigNumberPtr) calloc(1, sizeof(bigNumber));
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
        nextChunk = additionDigits;
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
        counter++;
    }
    Tail->nextChunk = additionDigits;
    additionDigits->previousChunk = Tail;
    AdditionNumber->digits = additionDigits;
    return AdditionNumber;
}

static bigNumberPtr _firstComplementBigNumber(bigNumberPtr Number) {
    if (!Number)
        return Number;
    unsigned long long counter = 0;
    bigNumberPtr complementNumber = (bigNumberPtr) calloc(1, sizeof(bigNumber));
    bigNumberDigitsPtr Tail = (bigNumberDigitsPtr) calloc(1, sizeof(bigNumberDigits));
    bigNumberDigitsPtr tempOpPtr = Tail;
    bigNumberDigitsPtr numberTail = Number->digits->previousChunk;
    bigNumberDigitsPtr nextChunk = NULL;
    complementNumber->numberLength = Number->numberLength;
    complementNumber->isNegative = 0;
    char firstDigit = 1;
    while (counter < Number->numberLength) {
        nextChunk = tempOpPtr;
        if (firstDigit) {
            tempOpPtr->chunkOfNumber1 = 9 - numberTail->chunkOfNumber1;
            firstDigit = 0;
        } else {
            tempOpPtr->chunkOfNumber0 = 9 - numberTail->chunkOfNumber0;
            firstDigit = 1;
            numberTail = numberTail->previousChunk;
            if ((counter + 1) < Number->numberLength) {
                tempOpPtr->previousChunk = (bigNumberDigitsPtr) calloc(1, sizeof(bigNumberDigits));
                tempOpPtr = tempOpPtr->previousChunk;
            }
            tempOpPtr->nextChunk = nextChunk;
        }
        counter++;
    }
    nextChunk->previousChunk = Tail;
    Tail->nextChunk = nextChunk;
    complementNumber->digits = nextChunk;
}

static bigNumberPtr _secondComplementBigNumber(bigNumberPtr Number) {
    if (!Number)
        return Number;
    unsigned long long counter = 0;
    bigNumberPtr complementNumber = (bigNumberPtr) calloc(1, sizeof(bigNumber));
    bigNumberDigitsPtr Tail = (bigNumberDigitsPtr) calloc(1, sizeof(bigNumberDigits));
    bigNumberDigitsPtr tempOpPtr = Tail;
    bigNumberDigitsPtr numberTail = Number->digits->previousChunk;
    bigNumberDigitsPtr nextChunk = NULL;
    complementNumber->numberLength = Number->numberLength;
    complementNumber->isNegative = 0;
    char opData = 0X40;
    while (counter < Number->numberLength) {
        nextChunk = tempOpPtr;
        if (opData & 0x40) {
            if (!(opData & 0x20))
                opData = 0x20 | (9 - numberTail->chunkOfNumber1 + 1);
            else
                opData = 0x20 | (9 - numberTail->chunkOfNumber1 + ((opData & 0x08) >> 7));
            if ((opData & 0x1F) > 9)
                opData = 0x80 | 0x20 | (10 - (opData & 0x1F));
            tempOpPtr->chunkOfNumber1 = opData & 0x1F;
        } else {
            opData = 0x40 | 0x20 | (9 - numberTail->chunkOfNumber1 + ((opData & 0x08) >> 7));
            if ((opData & 0x1F) > 9)
                opData = 0x80 | 0x40 | 0x20 | (10 - (opData & 0x1F));
            tempOpPtr->chunkOfNumber1 = opData & 0x1F;
            if ((counter + 1) < Number->numberLength) {
                tempOpPtr->previousChunk = (bigNumberDigitsPtr) calloc(1, sizeof(bigNumberDigits));
                tempOpPtr = tempOpPtr->previousChunk;
                tempOpPtr->nextChunk = nextChunk;
            }
        }
        counter++;
    }
    nextChunk->previousChunk = Tail;
    Tail->nextChunk = nextChunk;
    complementNumber->digits = nextChunk;
    return complementNumber;
}

static bigNumberPtr _getMaxBigNumber(bigNumberPtr firstNumber, bigNumberPtr secondNumber) {
    if (!firstNumber || !firstNumber->digits) return secondNumber;
    if (!secondNumber || !secondNumber->digits) return firstNumber;
    bigNumberDigitsPtr firstNumberDigits = firstNumber->digits;
    bigNumberDigitsPtr secondNumberDigits = secondNumber->digits;
    bigNumberPtr maxBigNumber = NULL;
    if ((!firstNumber->isNegative &&
         (secondNumber->isNegative || firstNumber->numberLength > secondNumber->numberLength)) ||
        (firstNumber->isNegative && secondNumber->isNegative &&
         (firstNumber->numberLength < secondNumber->numberLength)))
        _BigNumberCpy(&maxBigNumber, firstNumber);
    else if ((!secondNumber->isNegative &&
              (firstNumber->isNegative || secondNumber->numberLength > firstNumber->numberLength)) || (
                     firstNumber->isNegative && secondNumber->isNegative &&
                     (firstNumber->numberLength > secondNumber->numberLength)))
        _BigNumberCpy(&maxBigNumber, secondNumber);
    else {
        unsigned long long counter = 0;
        while (counter < firstNumber->numberLength) {
            if (!firstNumber->isNegative && !secondNumber->isNegative) {
                if (firstNumberDigits->chunkOfNumber0 > secondNumberDigits->chunkOfNumber0) {
                    _BigNumberCpy(&maxBigNumber, firstNumber);
                    break;
                } else if (firstNumberDigits->chunkOfNumber0 < secondNumberDigits->chunkOfNumber0) {
                    _BigNumberCpy(&maxBigNumber, secondNumber);
                    break;
                } else {
                    if (firstNumberDigits->chunkOfNumber1 > secondNumberDigits->chunkOfNumber1) {
                        _BigNumberCpy(&maxBigNumber, firstNumber);
                        break;
                    } else if (firstNumberDigits->chunkOfNumber1 < secondNumberDigits->chunkOfNumber1) {
                        _BigNumberCpy(&maxBigNumber, secondNumber);
                        break;
                    }
                }
            } else {
                if (firstNumberDigits->chunkOfNumber0 > secondNumberDigits->chunkOfNumber0) {
                    _BigNumberCpy(&maxBigNumber, secondNumber);
                    break;
                } else if (firstNumberDigits->chunkOfNumber0 < secondNumberDigits->chunkOfNumber0) {
                    _BigNumberCpy(&maxBigNumber, firstNumber);
                    break;
                } else {
                    if (firstNumberDigits->chunkOfNumber1 > secondNumberDigits->chunkOfNumber1) {
                        _BigNumberCpy(&maxBigNumber, secondNumber);
                        break;
                    } else if (firstNumberDigits->chunkOfNumber1 < secondNumberDigits->chunkOfNumber1) {
                        _BigNumberCpy(&maxBigNumber, firstNumber);
                        break;
                    }
                }
            }
            firstNumberDigits = firstNumberDigits->nextChunk;
            secondNumberDigits = secondNumberDigits->nextChunk;
            counter += 2;
        }
    }
    if (!maxBigNumber) {
        _BigNumberCpy(&maxBigNumber, firstNumber);
    }
    return maxBigNumber;
}

static bigNumberPtr _getMinBigNumber(bigNumberPtr firstNumber, bigNumberPtr secondNumber) {
    if (!firstNumber || !firstNumber->digits) return secondNumber;
    if (!secondNumber || !secondNumber->digits) return firstNumber;
    bigNumberDigitsPtr firstNumberDigits = firstNumber->digits;
    bigNumberDigitsPtr secondNumberDigits = secondNumber->digits;
    bigNumberPtr minBigNumber = NULL;
    if ((!firstNumber->isNegative &&
         (secondNumber->isNegative || firstNumber->numberLength > secondNumber->numberLength)) ||
        (firstNumber->isNegative && secondNumber->isNegative &&
         (firstNumber->numberLength < secondNumber->numberLength)))
        _BigNumberCpy(&minBigNumber, secondNumber);
    else if ((!secondNumber->isNegative &&
              (firstNumber->isNegative || secondNumber->numberLength > firstNumber->numberLength)) || (
                     firstNumber->isNegative && secondNumber->isNegative &&
                     (firstNumber->numberLength > secondNumber->numberLength)))
        _BigNumberCpy(&minBigNumber, firstNumber);
    else {
        unsigned long long counter = 0;
        while (counter < firstNumber->numberLength) {
            if (!firstNumber->isNegative && !secondNumber->isNegative) {
                if (firstNumberDigits->chunkOfNumber0 > secondNumberDigits->chunkOfNumber0) {
                    _BigNumberCpy(&minBigNumber, secondNumber);
                    break;
                } else if (firstNumberDigits->chunkOfNumber0 < secondNumberDigits->chunkOfNumber0) {
                    _BigNumberCpy(&minBigNumber, firstNumber);
                    break;
                } else {
                    if (firstNumberDigits->chunkOfNumber1 > secondNumberDigits->chunkOfNumber1) {
                        _BigNumberCpy(&minBigNumber, secondNumber);
                        break;
                    } else if (firstNumberDigits->chunkOfNumber1 < secondNumberDigits->chunkOfNumber1) {
                        _BigNumberCpy(&minBigNumber, firstNumber);
                        break;
                    }
                }
            } else {
                if (firstNumberDigits->chunkOfNumber0 > secondNumberDigits->chunkOfNumber0) {
                    _BigNumberCpy(&minBigNumber, firstNumber);
                    break;
                } else if (firstNumberDigits->chunkOfNumber0 < secondNumberDigits->chunkOfNumber0) {
                    _BigNumberCpy(&minBigNumber, secondNumber);
                    break;
                } else {
                    if (firstNumberDigits->chunkOfNumber1 > secondNumberDigits->chunkOfNumber1) {
                        _BigNumberCpy(&minBigNumber, firstNumber);
                        break;
                    } else if (firstNumberDigits->chunkOfNumber1 < secondNumberDigits->chunkOfNumber1) {
                        _BigNumberCpy(&minBigNumber, secondNumber);
                        break;
                    }
                }
            }
            firstNumberDigits = firstNumberDigits->nextChunk;
            secondNumberDigits = secondNumberDigits->nextChunk;
            counter += 2;
        }
    }
    if (!minBigNumber) {
        _BigNumberCpy(&minBigNumber, firstNumber);
    }
    return minBigNumber;
}

static bigNumberPtr _subtractBigNumbers(bigNumberPtr firstNumber, bigNumberPtr secondNumber) {
    if (!firstNumber || !firstNumber->digits) return secondNumber;
    if (!secondNumber || !secondNumber->digits) return firstNumber;
    bigNumberPtr tempNumber = NULL;
    if (secondNumber->isNegative && !firstNumber->isNegative) {
        secondNumber->isNegative = False;
        tempNumber = _addBigNumbers(firstNumber, secondNumber);
        secondNumber->isNegative = True;
    } else if (!secondNumber->isNegative && firstNumber->isNegative) {
        secondNumber->isNegative = True;
        tempNumber = _addBigNumbers(firstNumber, secondNumber);
        secondNumber->isNegative = False;

    } else {
        bigNumberPtr greatestNumber = _specialGetMaxBigNumber(firstNumber, secondNumber);
        bigNumberPtr smallestNumber = _specialGetMinBigNumber(firstNumber, secondNumber);
        tempNumber = (bigNumberPtr) calloc(1, sizeof(bigNumber));
        bigNumberDigitsPtr greatestNumberDigits = greatestNumber->digits->previousChunk;
        bigNumberDigitsPtr smallestNumberDigits = smallestNumber->digits->previousChunk;
        bigNumberDigitsPtr Tail = (bigNumberDigitsPtr) calloc(1, sizeof(bigNumberDigits));
        bigNumberDigitsPtr subtractionDigits = Tail;
        bigNumberDigitsPtr nextChunk = NULL;
        if (greatestNumber == secondNumber)
            tempNumber->isNegative = 1;
        tempNumber->numberLength = greatestNumber->numberLength;
        char opResult = 0x40;
        unsigned long long counter = 0;
        while (counter < greatestNumber->numberLength) {
            if (counter < smallestNumber->numberLength) {
                if (opResult & 0x40) {
                    if (greatestNumberDigits->chunkOfNumber1 <
                        (smallestNumberDigits->chunkOfNumber1 + ((opResult & 0x80) >> 7))) {
                        opResult = 0x80 | (10 + greatestNumberDigits->chunkOfNumber1 -
                                           smallestNumberDigits->chunkOfNumber1 - ((opResult & 0x80) >> 7));
                    } else {
                        opResult = (greatestNumberDigits->chunkOfNumber1 -
                                    smallestNumberDigits->chunkOfNumber1 - ((opResult & 0x80) >> 7));
                    }
                    subtractionDigits->chunkOfNumber1 = opResult & 0x1F;
                } else {
                    if (greatestNumberDigits->chunkOfNumber0 <
                        (smallestNumberDigits->chunkOfNumber0 + ((opResult & 0x80) >> 7))) {
                        opResult = 0x80 | 0x40 | (10 + greatestNumberDigits->chunkOfNumber0 -
                                                  smallestNumberDigits->chunkOfNumber0 - ((opResult & 0x80) >> 7));
                    } else {
                        opResult = 0x40 | (greatestNumberDigits->chunkOfNumber0 -
                                           smallestNumberDigits->chunkOfNumber0 - ((opResult & 0x80) >> 7));
                    }
                    subtractionDigits->chunkOfNumber0 = opResult & 0x1F;
                    greatestNumberDigits = greatestNumberDigits->previousChunk;
                    smallestNumberDigits = smallestNumberDigits->previousChunk;
                    nextChunk = subtractionDigits;
                    if ((counter + 1) < greatestNumber->numberLength) {
                        subtractionDigits->previousChunk = (bigNumberDigitsPtr) calloc(1, sizeof(bigNumberDigits));
                        subtractionDigits = subtractionDigits->previousChunk;
                        subtractionDigits->nextChunk = nextChunk;
                    }
                }
            } else {
                if (opResult & 0x40) {
                    opResult = greatestNumberDigits->chunkOfNumber1 - ((opResult & 0x80) >> 7);
                    subtractionDigits->chunkOfNumber1 = opResult & 0x1F;
                } else {
                    opResult = 0x40 | greatestNumberDigits->chunkOfNumber0 - ((opResult & 0x80) >> 7);
                    subtractionDigits->chunkOfNumber0 = opResult & 0x1F;
                    greatestNumberDigits = greatestNumberDigits->previousChunk;
                    nextChunk = subtractionDigits;
                    if ((counter + 1) < greatestNumber->numberLength) {
                        subtractionDigits->previousChunk = (bigNumberDigitsPtr) calloc(1, sizeof(bigNumberDigits));
                        subtractionDigits = subtractionDigits->previousChunk;
                        subtractionDigits->nextChunk = nextChunk;
                    }

                }
            }
            counter++;
        }
        Tail->nextChunk = subtractionDigits;
        subtractionDigits->previousChunk = Tail;
        tempNumber->digits = subtractionDigits;
    }
    return tempNumber;
}

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
                } else if (currentChar == '+' || currentChar == ' ') {
                    tempNumberStringPtr++;
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
            tempNumberStringPtr++;
        }
        numberLength--;
    }
    if (!numberLength)
        return NULL;
    bigNumberDigitsPtr Tail = (bigNumberDigitsPtr) calloc(1, sizeof(bigNumber));
    bigNumberPtr internalBigNumber = (bigNumberPtr) calloc(1, sizeof(bigNumber));
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

bigNumberPtr addBigNumbers(unsigned long long count, ...) {
    bigNumberPtr firstNumber = NULL;
    bigNumberPtr secondNumber = NULL;
    bigNumberPtr additionNumber = NULL;
    va_list addList;
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
    va_end(addList);
    return additionNumber;
}

bigNumberPtr subtractBigNumbers(unsigned long long count, ...) {
    va_list subtractList;
    va_start(subtractList, count);
    bigNumberPtr firstNumber = NULL;
    bigNumberPtr secondNumber = NULL;
    bigNumberPtr subtractionNumber = NULL;
    for (int i = 0; i < count;) {
        if (!i) {
            firstNumber = va_arg(subtractList, bigNumberPtr);
            secondNumber = va_arg(subtractList, bigNumberPtr);
            i += 2;
            subtractionNumber = _subtractBigNumbers(firstNumber, secondNumber);
        } else {
            firstNumber = subtractionNumber;
            secondNumber = va_arg(subtractList, bigNumberPtr);
            subtractionNumber = _subtractBigNumbers(firstNumber, secondNumber);
            freeBigNumber(firstNumber);
            i++;
        }
    }
    va_end(subtractList);
    return subtractionNumber;
}

bigNumberPtr getMaxBigNumber(unsigned long long count, ...) {
    va_list compareList;
    va_start(compareList, count);
    bigNumberPtr firstNumber = NULL;
    bigNumberPtr secondNumber = NULL;
    bigNumberPtr maxNumber = NULL;
    for (int i = 0; i < count;) {
        if (!i) {
            firstNumber = va_arg(compareList, bigNumberPtr);
            secondNumber = va_arg(compareList, bigNumberPtr);
            i += 2;
            maxNumber = _getMaxBigNumber(firstNumber, secondNumber);
        } else {
            firstNumber = maxNumber;
            secondNumber = va_arg(compareList, bigNumberPtr);
            maxNumber = _getMaxBigNumber(firstNumber, secondNumber);
            freeBigNumber(firstNumber);
            i++;
        }
    }
    if (!maxNumber)
        memcpy(maxNumber, firstNumber, sizeof(bigNumber));
    va_end(compareList);
    return maxNumber;
}

bigNumberPtr getMinBigNumber(unsigned long long count, ...) {
    bigNumberPtr firstNumber = NULL;
    bigNumberPtr secondNumber = NULL;
    bigNumberPtr maxNumber = NULL;
    va_list compareList;
    va_start (compareList, count);
    for (int i = 0; i < count;) {
        if (!i) {
            firstNumber = va_arg(compareList, bigNumberPtr);
            secondNumber = va_arg(compareList, bigNumberPtr);
            i += 2;
            maxNumber = _getMinBigNumber(firstNumber, secondNumber);
        } else {
            firstNumber = maxNumber;
            secondNumber = va_arg(compareList, bigNumberPtr);
            maxNumber = _getMinBigNumber(firstNumber, secondNumber);
            freeBigNumber(firstNumber);
            i++;
        }
    }
    if (!maxNumber)
        memcpy(maxNumber, firstNumber, sizeof(bigNumber));
    va_end(compareList);
    return maxNumber;
}

bigNumberPtr firstComplementBigNumber(bigNumberPtr bigNumber) {
    return _firstComplementBigNumber(bigNumber);
}

bigNumberPtr secondComplementBigNumber(bigNumberPtr bigNumber) {
    return _secondComplementBigNumber(bigNumber);
}

void printBigNumber(bigNumberPtr bigNumber) {
    if (!bigNumber || !bigNumber->numberLength || !bigNumber->digits)
        return;
    bigNumberDigitsPtr tempPtr = bigNumber->digits;
    if (bigNumber->isNegative)
        printf("-");
    unsigned long long counter = 0, maxLength = bigNumber->numberLength;
    char started = 0;
    if (bigNumber->numberLength % 2 != 0)
        maxLength++;
    while (counter < maxLength) {
        if (!started && tempPtr->chunkOfNumber0)
            started = 1;
        if (started)
            printf("%d", tempPtr->chunkOfNumber0);
        if (!started && (tempPtr->chunkOfNumber1 != 0 || bigNumber->numberLength == 1))
            started = 1;
        if (started)
            printf("%d", tempPtr->chunkOfNumber1);
        counter += 2;
        tempPtr = tempPtr->nextChunk;
    }
    if (!started)
        printf("0");
    printf("\n");
}

void freeBigNumber(bigNumberPtr internalBigNumber) {
    if (!internalBigNumber || !internalBigNumber->numberLength)
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
