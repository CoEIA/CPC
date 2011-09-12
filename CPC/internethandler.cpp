#include "internethandler.h"

#include <QEventLoop>
#include "constants.h"

InternetHandler::InternetHandler()
{
    manager = new QNetworkAccessManager(this);
    errorFlag = false;
}

QString InternetHandler::fecth(const QString& key) {
    connect(manager, SIGNAL(finished(QNetworkReply*)), this, SLOT(replyFinished(QNetworkReply*)));
    QString url = QString(SER_INFO).arg(key);
    manager->get(QNetworkRequest(QUrl(url)));

    QEventLoop eventLoop;
    connect(manager, SIGNAL(finished(QNetworkReply *)), &eventLoop, SLOT(quit()));
    eventLoop.exec();

    return result;
}

QString InternetHandler::fecthVersion() {
    connect(manager, SIGNAL(finished(QNetworkReply*)), this, SLOT(replyFinished(QNetworkReply*)));
    manager->get(QNetworkRequest(QUrl(SER_VERSION)));

    QEventLoop eventLoop;
    connect(manager, SIGNAL(finished(QNetworkReply *)), &eventLoop, SLOT(quit()));
    eventLoop.exec();

    return result;
}

QByteArray InternetHandler::fetchEXE() {
    connect(manager, SIGNAL(finished(QNetworkReply*)), this, SLOT(replyFinishedBytes(QNetworkReply*)));
    manager->get(QNetworkRequest(QUrl(SER_EXE)));

    QEventLoop eventLoop;
    connect(manager, SIGNAL(finished(QNetworkReply *)), &eventLoop, SLOT(quit()));
    eventLoop.exec();

    return resultBytes;
}

bool InternetHandler::writeInfo(const QString& key, const QString& ip) {
    connect(manager, SIGNAL(finished(QNetworkReply*)), this, SLOT(replyFinishedBool(QNetworkReply*)));
    QString url = QString(SER_SAVE).arg(key,ip);
    manager->get(QNetworkRequest(QUrl(url)));

    QEventLoop eventLoop;
    connect(manager, SIGNAL(finished(QNetworkReply *)), &eventLoop, SLOT(quit()));
    eventLoop.exec();

    return info;
}

void InternetHandler::replyFinished(QNetworkReply *np){
    if (np->error() != QNetworkReply::NoError) {
        errorFlag = true;
        return;
    }

    QByteArray data = np->readAll();
    QString str(data);

    result = str;
}

void InternetHandler::replyFinishedBytes(QNetworkReply *np){
    if (np->error() != QNetworkReply::NoError) {
        errorFlag = true;
        return;
    }

    QByteArray data = np->readAll();
    resultBytes = data;
}

void InternetHandler::replyFinishedBool(QNetworkReply *np){
    if (np->error() != QNetworkReply::NoError) {
        errorFlag = true;
        return;
    }

    QByteArray data = np->readAll();
    QString result(data);

    if ( result == "DONE" )
        info = true;
    else
        info = false;
}
