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

    QStringList getPaths() const ;

private:
    QString m_optionName;
    QStringList m_filePaths;
    bool m_isValidParameter;
};

#endif // COMMANDLINEPARSER_H
