#include "Bignumbers.h"

int main() {
    bigNumberPtr firstNumber = storeBigNumber("111");
    bigNumberPtr secondNumber = storeBigNumber("112");
    bigNumberPtr maximumNumber = getMaxBigNumber(2, firstNumber, secondNumber);
    printBigNumber(maximumNumber);
    /*bigNumberPtr secondNumber = storeBigNumber("+00000");
    bigNumberPtr thirdNumber = storeBigNumber("+0100");
    bigNumberPtr fourthNumber = storeBigNumber("+0000000");
    bigNumberPtr additionNumber = addBigNumbers(4, firstNumber, secondNumber, thirdNumber, fourthNumber);
    */
    freeBigNumber(firstNumber);
    freeBigNumber(secondNumber);
    freeBigNumber(maximumNumber);
    /*freeBigNumber(secondNumber);
    freeBigNumber(thirdNumber);
    freeBigNumber(fourthNumber);
    freeBigNumber(additionNumber);*/
    return 0;
}
