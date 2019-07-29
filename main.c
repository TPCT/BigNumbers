#include "Bignumbers.h"

int main() {
    bigNumberPtr firstNumber = storeBigNumber("-1025");
    bigNumberPtr secondNumber = storeBigNumber("-210");
    bigNumberPtr minimumNumber = subtractBigNumbers(2, firstNumber, secondNumber);
    printBigNumber(minimumNumber);
    /*bigNumberPtr secondNumber = storeBigNumber("+00000");
    bigNumberPtr thirdNumber = storeBigNumber("+0100");
    bigNumberPtr fourthNumber = storeBigNumber("+0000000");
    bigNumberPtr additionNumber = addBigNumbers(4, firstNumber, secondNumber, thirdNumber, fourthNumber);
    */
    freeBigNumber(firstNumber);
    freeBigNumber(secondNumber);
    freeBigNumber(minimumNumber);
    /*freeBigNumber(secondNumber);
    freeBigNumber(thirdNumber);
    freeBigNumber(fourthNumber);
    freeBigNumber(additionNumber);*/
    return 0;
}
