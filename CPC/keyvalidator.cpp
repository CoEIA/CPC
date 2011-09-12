
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
