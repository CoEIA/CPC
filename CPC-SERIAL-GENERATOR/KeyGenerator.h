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

#ifndef KEYGENERATOR_H
#define KEYGENERATOR_H

#include <QString>
#include "lunhalgorithm.h"

class KeyGenerator {
public:
    static QString generateKey (QString time, QString serial, QString constant) {
        QString variables = time + serial + constant ;
        QString result = variables + LunhAlgorithm::lunhGenerator(variables);
        QString key = getKey(result.toAscii().toBase64());

        return key;
    }

private:
    static QString getKey (QString input) {
        QString result = "" ;

        for (int i=0; i<input.size(); i++) {
            if ( i % 5 != 0 || i == 0)
                result += input[i];
            else if ( i%5 == 0){
                result += "-" + input[i];
            }
        }

        return (result);
    }
};

#endif // KEYGENERATOR_H
