#include "taskchecker.h"

#include <QTextStream>
#include <QProcess>
#include <QSettings>
#include <QDebug>

TaskChecker::TaskChecker()
{
}

void TaskChecker::checkForTasks() {
    QList<QStringList> tasks = getAllTasks();

    foreach(QStringList task, tasks) {
        QString name = task.at(0);
        QString time = task.at(1);
        QString days = task.at(2).toLower();

        if ( days.contains(getCurrentDay().toLower()) && time == getCurrentTime() ) {
            if ( lists.contains(name) )
                continue;

            //QString path = "\"" + getCPCPath() + "\" -scan";
            //QString path = "CPC.exe -scan";

            QString processName = "CPC.exe" ;

            //QDir dir;
            //QString path = "\"" + QDir::toNativeSeparators(dir.absolutePath() + "\\" + processName) + "\"";

            lunchProcess(processName);
            lists.append(name);
        }
    }
}

QList<QStringList> TaskChecker::getAllTasks () {
    QList<QString> tasksPath = getTasksFilePath();
    QList<QStringList> allTasks;

    foreach(QString taskPath, tasksPath) {
        allTasks.append( readTaskInfo(taskPath));
    }

    return (allTasks);
}

QStringList TaskChecker::readTaskInfo (const QString& taskPath) {
    QFile file(taskPath);
    QStringList info;

    if ( file.open(QIODevice::ReadOnly | QIODevice::Text ) ) {
        QTextStream stream(&file);

        info << stream.readLine();
        info << stream.readLine();
        info << stream.readLine();
    }

    file.close();
    return (info);
}

QList<QString> TaskChecker::getTasksFilePath() {
    QString path = getSchedulePath();

    return showAllBellow(path, "*.txt", false);
}

QList<QString> TaskChecker::showAllBellow (const QString& path, const QString& regx, bool subdir){
    QDir dir(path);
    QList<QString> result ;

    QStringList filters;
    filters << regx ;

    dir.setNameFilters(filters);

    if ( dir.exists(path) ) {
        foreach(QFileInfo info, dir.entryInfoList(QDir::NoDotAndDotDot | QDir::System | QDir::Hidden  | QDir::AllDirs | QDir::Files, QDir::DirsLast)) {
            if ( info.isDir() && subdir ) {
            }
            else if( info.isSymLink()) {
                result.append(info.filePath());
            }
            else {
                result.append(info.absoluteFilePath());
            }
        }
    }

    return result;
}

QString TaskChecker::getSchedulePath() {
    QSettings settings("CoEIA", "CPC 1.1");
    QString path = settings.value("SCHED_PATH","-").toString();

    QFileInfo file(path);
    path = file.absoluteFilePath();

    return path;
}

QString TaskChecker::getCPCPath () {
    QSettings settings("CoEIA", "CPC 1.1");
    QString path = settings.value("APP_PATH","-").toString() + "\\CPC.exe";

    return path;
}

QString TaskChecker::getCurrentTime() {
    QString time = QTime::currentTime().toString("HH:mm");
    return time;
}

QString TaskChecker::getCurrentDay(){
    QLocale loc(QLocale::English, QLocale::UnitedStates);
    QDate date = QDate::currentDate();
    QString day = loc.toString(date, "ddd").toUpper();

    return day;
}

void TaskChecker:: lunchProcess (QString path) {
    QProcess *process = new QProcess;

    process->start(path, QStringList() << "-scan");
    process->waitForStarted();
    process->waitForFinished(-1);
}
