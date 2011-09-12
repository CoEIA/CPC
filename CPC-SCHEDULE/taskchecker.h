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
