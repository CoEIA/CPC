#include <QtCore/QCoreApplication>

#include <QTimer>
#include <QStringList>

#include "taskchecker.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    QDir::setCurrent(QCoreApplication::applicationDirPath());

    TaskChecker checker;

    QTimer *timer = new QTimer(NULL);
    QObject::connect(timer, SIGNAL(timeout()), &checker, SLOT(checkForTasks()));
    timer->setInterval(1000*30);
    timer->start();

    return a.exec();
}

