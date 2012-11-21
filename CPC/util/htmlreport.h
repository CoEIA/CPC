#ifndef HTMLREPORT_H
#define HTMLREPORT_H

#include <QString>

class HTMLReport
{
public:
    HTMLReport();

    static bool generateItemsRemovalReport (const QString& logFileName);

private:
    static QString getHTMLRowForAllPath(const QStringList& list);
};

#endif // HTMLREPORT_H
