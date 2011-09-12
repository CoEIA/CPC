#include "htmlreport.h"

#include <QFile>
#include <QTextStream>
#include <QStringList>
#include <QDateTime>
#include <QTextBrowser>
#include <QDebug>
#include <QDesktopServices>

HTMLReport::HTMLReport()
{
}

bool HTMLReport ::generateItemsRemovalReport(const QString &logFileName) {
    QStringList succRemoval, errorRemoval;
    QFile logFile(logFileName);

    if ( !logFile.exists() ) {
        return false;
    }

    if ( ! logFile.open(QIODevice::ReadOnly | QIODevice::Text) ) {
        return false;
    }

    QTextStream stream(&logFile);

    while ( !stream.atEnd() ) {
        QString line = stream.readLine();

        if ( line.startsWith("Remove True"))
            succRemoval.append(line);
        else
            errorRemoval.append(line);
    }

    logFile.close();

    QString date = QDateTime::currentDateTime().toString("dd/MM/yyyy - h:m:s ap");
    QString succNumber = QString::number(succRemoval.count());
    QString errNumber = QString::number(errorRemoval.count());

    // generate HTML
    QFile reportFile(":/docs/htmlReport.html");
    if ( !reportFile.open(QIODevice::ReadOnly | QIODevice::Text))
        return false;

    QString originalCode = reportFile.readAll();

    QString reportCode = originalCode.replace("%DATE%", date).replace("%SUCC_NUM%", succNumber)
                         .replace("%ERR_NUM%", errNumber).replace("%FILE_ERR_PATH%", getHTMLRowForAllPath(errorRemoval))
                         .replace("%FILE_SUCC_PATH%", getHTMLRowForAllPath(succRemoval));

    //qDebug() << reportCode ;

    // save HTML file & open it using default browser
    QFile htmlFile(logFileName + "_Report.html");
    if ( !htmlFile.open(QIODevice::WriteOnly | QIODevice::Text))
        return false;

    QTextStream *htmlStream = new QTextStream(&htmlFile);
    htmlStream->operator <<(reportCode);
    htmlFile.close();

    QDesktopServices::openUrl(logFileName + "_Report.html");

    return true;
}

QString HTMLReport::getHTMLRowForAllPath(const QStringList& list) {
    QString result ;

    foreach(QString path, list) {
        QString row = QString("<tr class=\"o\"><td>%1</td></tr>").arg(path);
        result.append(row);
    }

    return result;
}
