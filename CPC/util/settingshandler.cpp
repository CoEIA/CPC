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

#include "settingshandler.h"

#include <QSettings>
#include <QDateTime>
#include <QApplication>
#include <QDir>

SettingsHandler::SettingsHandler()
{
}

void SettingsHandler:: writeKey (const KeySettings& key){
    QSettings settings("CoEIA", "CPC");

    settings.setValue("key", key.key);
    settings.setValue("version",key.version);
    settings.setValue("Time", key.time);
    settings.setValue("State", key.state);
    settings.setValue("InstallTime",key.installTime);
    settings.setValue("LastUsageTime", key.lastUsageTime);
}

void SettingsHandler:: writeInstallTime (const QDateTime& installTime){
    QSettings settings("CoEIA", "CPC");
    settings.setValue("InstallTime",installTime);
}

void SettingsHandler:: writeLastUsageTime (const QDateTime& lastUsageTime){
    QSettings settings("CoEIA", "CPC");
    settings.setValue("LastUsageTime", lastUsageTime);
}

void SettingsHandler:: writeApplicationInfo() {
    QString appPath = QDir::toNativeSeparators(QApplication::applicationDirPath());
    QString schedPath =  QDir::toNativeSeparators(appPath + "\\" + "Schedules");

    QSettings settings("CoEIA", "CPC");
    settings.setValue("APP_PATH", appPath);
    settings.setValue("SCHED_PATH", schedPath);
}

KeySettings SettingsHandler:: readKey () {
    QSettings settings("CoEIA", "CPC");

    QString key = settings.value("key","-").toString();
    QString version = settings.value("version","Demo").toString();
    QString time = settings.value("Time", "-").toString();
    KeyState state = (KeyState) (settings.value("state", UNKNOWN_STATE).toInt());
    QDateTime installTime = settings.value("InstallTime", QDateTime::currentDateTime()).toDateTime();
    QDateTime lastUsageTime = settings.value("LastUsageTime", QDateTime::currentDateTime()).toDateTime();

    return KeySettings(key, time, version, state, installTime, lastUsageTime);
}

bool SettingsHandler::isExpireVersion () {
    bool state = false ;

    QSettings settings("CoEIA", "CPC");
    QDateTime current = QDateTime::currentDateTime();

    QDateTime installTime = settings.value("InstallTime", current).toDateTime();
    QDateTime lastUsageTime = settings.value("LastUsageTime", current).toDateTime();


    if ( lastUsageTime > current )
        state = true;

    else if ( lastUsageTime < current) {
        if ( current < installTime.addDays(FREE_DAYS))
            state = false;
        else
            state = true;
    }
    else {
        // free version, write install time & last usage time
        writeInstallTime(current);
        writeLastUsageTime(current);
        state = false;
    }

    return state;
}

bool SettingsHandler:: isFreeVersion() {
    QSettings settings("CoEIA", "CPC");
    QString version = settings.value("version","Demo").toString();

    if ( version == "Demo")
        return true;
    else
        return false;
}

bool SettingsHandler:: isCleanState () {
    QSettings settings("CoEIA", "CPC");
    KeyState state = (KeyState) (settings.value("state", UNKNOWN_STATE).toInt());

    if ( state == KNOWN_STATE )
        return true;
    else
        return false;
}
