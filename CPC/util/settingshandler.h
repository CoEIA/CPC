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

#ifndef SETTINGSHANDLER_H
#define SETTINGSHANDLER_H

#include <QString>
#include <QDateTime>

#include "constants.h"

struct KeySettings {
    KeySettings(const QString& aKey, const QString& aTime, const QString& aVersion,
                const KeyState& aState, const QDateTime& aInstallTime,
                const QDateTime& aLastUsageTime ) {
        key = aKey ;
        time = aTime ;
        version = aVersion;
        state = aState ;
        installTime = aInstallTime ;
        lastUsageTime = aLastUsageTime;
    }

    KeySettings(const QString& aKey, const QString& aTime, const QString& aVersion,
                const KeyState& aState) {
        key = aKey ;
        time = aTime ;
        version = aVersion;
        state = aState ;
    }

    QString key;
    QString time;
    QString version;
    KeyState state ;
    QDateTime installTime, lastUsageTime;
};

class SettingsHandler
{
public:
    SettingsHandler();

    static void writeKey (const KeySettings& key) ;
    static void writeInstallTime (const QDateTime& key) ;
    static void writeLastUsageTime (const QDateTime& key) ;

    static void writeApplicationInfo();

    static KeySettings readKey ();

    static bool isFreeVersion();
    static bool isCleanState ();
    static bool isExpireVersion();
};

#endif // SETTINGSHANDLER_H
