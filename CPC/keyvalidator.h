#ifndef KEYVALIDATOR_H
#define KEYVALIDATOR_H

#include <QString>

#include "constants.h"

class KeyValidator {
public:
    static bool validate (QString key);
    static Server_Query_Response internetValidator (const QString& key);
    static Server_Save_Response writeKeyToServer (const QString& key) ;

private:
    static bool lunhValidation (QString input);
};

#endif // KEYVALIDATOR_H
