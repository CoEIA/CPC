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

#ifndef ENUMERATETHREAD_H
#define ENUMERATETHREAD_H

#include <QThread>
#include <QStringList>

class EnumerateThread: public QThread
{
    Q_OBJECT

public:
    EnumerateThread();
    void setData (const QString& filePath, const QStringList& exts);

signals:
    void addItemSignal(QString);
    void finishThreadSignal();

public slots:
    void stop() { stopped = true; }

private:
    void run();
    volatile bool stopped;

    void showAllBellow (const QString& path, const QStringList& regx, bool subdir);
    void showAll       (const QString& path, const QStringList& regx, bool subdir);

private:
    QString filePath;
    QStringList exts;

    int count ;
};

#endif // ENUMERATETHREAD_H
