#include "lunhalgorithm.h"

LunhAlgorithm::LunhAlgorithm()
{
}

char LunhAlgorithm:: lunhGenerator (QString input) {
    int factor = 2 ;
    int sum = 0;
    int n = 128;

    for (int i=input.size()-1 ; i>= 0;  i--) {
        int codePoint = (int) input[i].toAscii();
        int addend = factor * codePoint;

        factor = (factor == 2 ) ? 1 : 2;

        addend = (addend/n) + (addend%n);
        sum += addend;
    }

    int remainder = sum%n;
    int checkCodePoint = n - remainder;
    checkCodePoint %= n;

    return (char) checkCodePoint;
}

bool LunhAlgorithm:: lunhValidation (QString input) {
    int factor = 1 ;
    int sum = 0;
    int n = 128;

    for (int i=input.size() -1; i>=0; i--) {
        int codePoint = (int) input[i].toAscii();
        int addend = factor * codePoint;

        factor = (factor == 2 ) ? 1 : 2;

        addend = (addend/n) + (addend%n);
        sum += addend;
    }

    int remainder = sum % n;
    return ( remainder == 0 );
}
