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

#ifndef UTILITIES_H
#define UTILITIES_H

#include <QString>
#include <QMultiMap>

#include "plugins.h"
#include "pluginsparser.h"
#include "constants.h"

class Utilities
{
public:
    Utilities();

    static void lunchUpdate();

    static bool promptForUpdate();
    static void promptForNoUpdate();
    static void promptForFreeVersion();
    static void promptForErrorUpdate();
    static bool promptForReport();
    static void promptForRestore();

    static double getProgramVersion()  { return PROGRAM_VERSION  ; }
    static double getDatabaseVersion() { return DATABASE_VERSION ; }

    static QString getOSVersion ();
    static void printInDebug(const Plugins& plug);
    static QMultiMap<QString, Plugins> readAllPlugins(const QString& pluginsPath);

    static QString getCurrentUserName();

    static QString replaceVariable (const QString& path);
    static QStringList handleLocation (QString location);

    static void addScheduleInStartup();
    static void lunchScheduleNow();
    static void addToStartup(QString name, QString path);
    static bool isProcessRun (QString processName);
    static void lunchProcess (QString path, bool wait);

    static void loadEnglishStyle();
    static void loadArabicStyle();

    static QString buildQStringFromQStringList(QStringList arguments);

};

#endif // UTILITIES_H
