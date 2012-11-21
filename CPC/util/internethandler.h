#ifndef KEYVALIDATORNET_H
#define KEYVALIDATORNET_H

#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkReply>
#include <QDebug>

class InternetHandler : public QObject{
public:
    InternetHandler();
    Q_OBJECT

public:
    QString fecth(const QString& key);
    QString fecthVersion();
    QByteArray fetchEXE();

    bool writeInfo(const QString& key, const QString& ip);
    bool hasError () { return errorFlag; }

public slots:
    void replyFinished(QNetworkReply* np);
    void replyFinishedBytes(QNetworkReply* np);
    void replyFinishedBool(QNetworkReply* np);

private:
    QNetworkAccessManager *manager;
    QString result ;
    QByteArray resultBytes;
    bool info;
    bool errorFlag;
};

#endif // KEYVALIDATORNET_H
