#ifndef LUNHALGORITHM_H
#define LUNHALGORITHM_H

#include <QString>

class LunhAlgorithm
{
public:
    LunhAlgorithm();

public:
    static char lunhGenerator (QString input);
    static bool lunhValidation (QString input);
};

#endif // LUNHALGORITHM_H
