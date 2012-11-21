#ifndef COMMANDLINEPARSER_H
#define COMMANDLINEPARSER_H

#include <QStringList>
#include "constants.h"

class CommandLineParser
{
public:
    CommandLineParser(QStringList args);

    bool isValidParameter() const;
    bool isScanParameter() const;
    bool isShredQuickParameter() const;
    bool isShredSafeParameter() const;
    bool isShredThroughParameter() const;

    QString getPath() const ;

private:
    QString m_optionName;
    QString m_filePath;
    bool m_isValidParameter;
};

#endif // COMMANDLINEPARSER_H
