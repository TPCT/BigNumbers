#include "Bignumbers.h"
#include "time.h"
#include "limits.h"

int main() {
    bigNumberPtr firstNumber = storeBigNumber("15489");
    bigNumberPtr secondNumber = storeBigNumber("1");
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
    freeBigNumber(secondNumber);
    freeBigNumber(addition);
    freeBigNumber(subtraction);
    freeBigNumber(max);
    freeBigNumber(min);
    freeBigNumber(firstComplement);
    freeBigNumber(secondComplement);
    freeBigNumber(firstNumber);
    return 0;
}
