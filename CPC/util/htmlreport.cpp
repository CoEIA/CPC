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
