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

};

#endif // UTILITIES_H
