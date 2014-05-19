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

#ifndef TASKCHECKER_H
#define TASKCHECKER_H

#include <QObject>

#include <QDateTime>
#include <QFile>
#include <QDir>
#include <QFileInfo>

class TaskChecker: public QObject
{
    Q_OBJECT
public:
    TaskChecker();

    QString getSchedulePath();
    QString getCPCPath () ;
    QString getCurrentTime();
    QString getCurrentDay();

    QList<QString> showAllBellow (const QString& path, const QString& regx, bool subdir);
    QList<QString> getTasksFilePath() ;
    QList<QStringList> getAllTasks ();
    QStringList readTaskInfo (const QString& taskPath);

    void lunchProcess  (QString path);

public slots:
    void checkForTasks();

private:
    QStringList lists;
};

#endif // TASKCHECKER_H
