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
