#ifndef KEYVALIDATORNET_H
#define KEYVALIDATORNET_H

#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkReply>
#include <QDebug>

class KeyValidatorNet : public QObject{
public:
    KeyValidatorNet();
    Q_OBJECT

public:
    QByteArray fetchEXE();


public slots:
    void replyFinishedBytes(QNetworkReply* np);

private:
    QNetworkAccessManager *manager;
    QByteArray resultBytes;
};

#endif // KEYVALIDATORNET_H
