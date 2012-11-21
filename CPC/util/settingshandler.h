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
