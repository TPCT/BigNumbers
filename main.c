#include "Bignumbers.h"

int main() {
    bigNumberPtr firstNumber = storeBigNumber("0");
    bigNumberPtr secondNumber = storeBigNumber("999");
    bigNumberPtr addition = addBigNumbers(2, firstNumber, secondNumber);
    bigNumberPtr subtraction = subtractBigNumbers(2, firstNumber, secondNumber);
    bigNumberPtr max = getMaxBigNumber(2, firstNumber, secondNumber);
    bigNumberPtr min = getMinBigNumber(2, firstNumber, secondNumber);
    bigNumberPtr firstComplement = firstComplementBigNumber(firstNumber);
    bigNumberPtr secondComplement = secondComplementBigNumber(firstNumber);
    printBigNumber(addition);
    printBigNumber(max);
    printBigNumber(min);
    printBigNumber(firstComplement);
    printBigNumber(secondComplement);
    printBigNumber(subtraction);
    freeBigNumber(firstNumber);
    freeBigNumber(secondNumber);
    freeBigNumber(addition);
    freeBigNumber(subtraction);
    freeBigNumber(max);
    freeBigNumber(min);
    freeBigNumber(firstComplement);
    freeBigNumber(secondComplement);
    return 0;
}
