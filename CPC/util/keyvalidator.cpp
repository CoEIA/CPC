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

#include "keyvalidator.h"
#include "internethandler.h"

#include <QMessageBox>

bool KeyValidator:: validate (QString key) {
    key = key.replace("-", "");
    key = key.toAscii().fromBase64(key.toAscii());
    const QString constantNumber = "658712";
    if ( key.contains(constantNumber))
        return lunhValidation(key);
    else
        return false;
}

Server_Query_Response KeyValidator::internetValidator(const QString &key) {
    InternetHandler *net = new InternetHandler;
    QString result = net->fecth(key);

    qDebug() << "Result: " << result;

    if ( net->hasError() )
        return UNKNOWN_KEY;

    // validate over internet
    if ( result.contains("OK" ) ) {
        return CLEAN_KEY;
    }

    // key is already used
    else if ( result.contains("Used")) {
        return USED_KEY;
    }

    // key is illegal
    else if ( result.contains("NO") ) {
        return ILLEGAL_KEY;
    }

    // some error no internet or cannot access page (put in susbected state)
    else {
        return UNKNOWN_KEY;
    }
}

Server_Save_Response KeyValidator::writeKeyToServer(const QString &key) {
    QString ip = "";

    InternetHandler *writer = new InternetHandler;

    if ( writer->hasError() )
        return SAVING_ERROR;

    bool state = writer->writeInfo(key,ip);

    return state? SAVING_DONE: SAVING_ERROR ;
}

bool KeyValidator:: lunhValidation (QString input) {
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
