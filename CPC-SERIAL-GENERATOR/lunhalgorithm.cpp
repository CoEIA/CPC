/*
    CoEIA Privacy Control (CPC) - CPC provides easy and efficient methods of finding and
    erasing unwanted and unnecessary items on your computer.

    Copyright (C) 2014 Center of excellence in Information Assurance, All rights reserved.

    This program is free software; you can redistribute it and/or modify it under the terms
    of the GNU General Public License as published by the Free Software Foundation;
    either version 2 of the License, or (at your option) any later version.

    This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY;
    without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
    See the GNU General Public License for more details.

    You should have received a copy of the GNU General Public License along with this program;
    if not, write to the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
    Boston, MA 02110-1301 USA
*/

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
