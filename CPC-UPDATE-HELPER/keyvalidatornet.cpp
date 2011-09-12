#include "keyvalidatornet.h"

#include <QEventLoop>

const QString TEST_EXE = "http://127.0.0.1/wajdy/CPC.exe";
const QString SER_EXE = "http://licenses.coeia.edu.sa/software/cpc/bin/CPC.exe";

KeyValidatorNet::KeyValidatorNet()
{
    manager = new QNetworkAccessManager(this);
}

QByteArray KeyValidatorNet::fetchEXE() {
    connect(manager, SIGNAL(finished(QNetworkReply*)), this, SLOT(replyFinishedBytes(QNetworkReply*)));
    manager->get(QNetworkRequest(QUrl(SER_EXE)));

    QEventLoop eventLoop;
    connect(manager, SIGNAL(finished(QNetworkReply *)), &eventLoop, SLOT(quit()));
    eventLoop.exec();

    return resultBytes;
}

void KeyValidatorNet::replyFinishedBytes(QNetworkReply *np){
    QByteArray data = np->readAll();
    resultBytes = data;
}
