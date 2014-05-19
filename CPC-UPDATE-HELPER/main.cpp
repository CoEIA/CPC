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

#include <QtCore/QCoreApplication>

#include <QDebug>
#include <QStringList>
#include <QProcess>
#include <QFile>
#include <QThread>
#include <QDir>

#include "keyvalidatornet.h"

void removeProcess  (const QString& path) ;
void lunchProcess   (const QString& path) ;
void killProcess    (const QString& name) ;

// processName, oldProcessPath
// kill 1, remove 2 , rename new file content to 1 and run it

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    QDir::setCurrent(QCoreApplication::applicationDirPath());

    QStringList args = a.arguments();

    if ( args.count() < 2 )
        return 1;

    // download exe
    KeyValidatorNet *exeDownloader = new KeyValidatorNet;
    QByteArray data = exeDownloader->fetchEXE();

    // processing
    killProcess(args[1]);
    removeProcess(args[2]);

    // write new file content to the new created file
    QFile newFile(args[2]);
    newFile.open(QIODevice::WriteOnly);
    newFile.write(data);
    newFile.close();

    lunchProcess(args[2]);

    //return a.exec();
    return 0;
}

void removeProcess (const QString& path) {
    QFile file(path);

    if ( file.exists()) {
        qDebug() << "file found: " << path;

        if ( file.remove() )
            qDebug() << "remove ok";
        else
            qDebug() << "remove error";
    }
    else {
        qDebug() << "File not found: " << path;
    }
}

void lunchProcess (const QString& path) {
    QProcess *process = new QProcess;
    process->start(path);
    process->waitForFinished(-1);
}

void killProcess (const QString& name) {
    QProcess *process = new QProcess;

    QString app = QString("taskkill /im %1 /f").arg(name);
    process->start(app);
    process->waitForFinished(-1);
}
