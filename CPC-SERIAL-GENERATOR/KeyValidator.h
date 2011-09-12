#ifndef KEYVALIDATOR_H
#define KEYVALIDATOR_H

#include <QString>
#include "lunhalgorithm.h"

class KeyValidator {
public:
    static bool validate (QString key) {
        key = key.replace("-", "");
        key = key.toAscii().fromBase64(key.toAscii());
        const QString constantNumber = "658712";

        if ( key.contains(constantNumber))
            return LunhAlgorithm::lunhValidation(key);
        else
            return false;
    }
};

#endif // KEYVALIDATOR_H
