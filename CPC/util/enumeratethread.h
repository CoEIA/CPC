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
