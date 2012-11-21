#ifndef UPDATATHREAD_H
#define UPDATATHREAD_H

#include <QThread>

class UpdateChecker : public QThread
{
    Q_OBJECT

public:
    UpdateChecker() {}
    UpdateChecker(const double programVersion, const double dataBaseVersion);

    void setData(double a, double b) { programVersion = a; dataBaseVersion = b; }
    //void lunchUpdater();

private:
    void run();

signals:
    void finishUpdateSignal(bool update);
    void errorUpdateSignal();

private:
    double programVersion, dataBaseVersion;
};

#endif // UPDATATHREAD_H
