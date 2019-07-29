#include "Bignumbers.h"

int main() {
    bigNumberPtr firstNumber = storeBigNumber("-111");
    bigNumberPtr secondNumber = storeBigNumber("112");
    bigNumberPtr thirdNumber = storeBigNumber("112");
    bigNumberPtr minimumNumber = getMinBigNumber(3, firstNumber, secondNumber, thirdNumber);
    printBigNumber(minimumNumber);
    /*bigNumberPtr secondNumber = storeBigNumber("+00000");
    bigNumberPtr thirdNumber = storeBigNumber("+0100");
    bigNumberPtr fourthNumber = storeBigNumber("+0000000");
    bigNumberPtr additionNumber = addBigNumbers(4, firstNumber, secondNumber, thirdNumber, fourthNumber);
    */
    freeBigNumber(firstNumber);
    freeBigNumber(secondNumber);
    freeBigNumber(thirdNumber);
    freeBigNumber(minimumNumber);
    /*freeBigNumber(secondNumber);
    freeBigNumber(thirdNumber);
    freeBigNumber(fourthNumber);
    freeBigNumber(additionNumber);*/
    return 0;
}
