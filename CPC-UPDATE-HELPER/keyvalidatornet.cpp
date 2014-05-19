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
