#include "updatechecker.h"
#include "internethandler.h"

#include <QStringList>
#include <QMessageBox>
#include <QDir>
#include <QApplication>
#include <QProcess>

UpdateChecker::UpdateChecker(const double programVersion, const double dataBaseVersion)
{
    this->programVersion = programVersion;
    this->dataBaseVersion = dataBaseVersion;
}

void UpdateChecker::run() {
    // read version from server
    InternetHandler *net = new InternetHandler;

    QString result = net->fecthVersion();

    if (net->hasError() ) {
        emit errorUpdateSignal();
        return ;
    }

    QString pVersion = (result.split("&")[0]).split(":")[1].trimmed();
    QString dVersion = (result.split("&")[1]).split(":")[1].trimmed();

    double pDouble = pVersion.toDouble();
    double dDouble = dVersion.toDouble();

    if ( pDouble > programVersion || dDouble > dataBaseVersion ) {
        emit finishUpdateSignal(true);  // there is an update
    }
    else {
        emit finishUpdateSignal(false); // no new update
    }
}

