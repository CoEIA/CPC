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
