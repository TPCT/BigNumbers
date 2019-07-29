#include "Bignumbers.h"

int main() {
    bigNumberPtr firstNumber = storeBigNumber("+114");
    bigNumberPtr secondNumber = storeBigNumber("+125");
    bigNumberPtr thirdNumber = storeBigNumber("+14568");
    bigNumberPtr fourthNumber = storeBigNumber("+14568");
    bigNumberPtr additionNumber = addBigNumbers(4, firstNumber, secondNumber, thirdNumber, fourthNumber);
    printBigNumber(additionNumber);
    freeBigNumber(firstNumber);
    freeBigNumber(secondNumber);
    freeBigNumber(thirdNumber);
    freeBigNumber(fourthNumber);
    freeBigNumber(additionNumber);
    return 0;
}
