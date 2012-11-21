#ifndef ADVANCEDSHREDDIALOG_H
#define ADVANCEDSHREDDIALOG_H

#include <QDialog>
#include <QProgressDialog>

#include "..\util\enumeratethread.h"

namespace Ui {
    class AdvancedShredDialog;
}

class AdvancedShredDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AdvancedShredDialog(QWidget *parent = 0);
    ~AdvancedShredDialog();

    bool isCancled()        { return isCancel   ; }
    QStringList getFiles()  { return resultFiles; }
    void retranslateUi();

protected:
        void changeEvent(QEvent* event);

private:
    Ui::AdvancedShredDialog *ui;

private slots:
    void selectFiles();
    void addFiles();
    void AcceptFiles();
    void cancelFiles();
    void progressCanceled();
    void finishEnumSlot();

private slots:
    void addItemSlot(QString fileName) ;

private:
    bool containInList (const QString& fileName);
    void stopThread();

    bool isCancel ;
    QStringList resultFiles;
    int count;
    EnumerateThread thread;
    QProgressDialog *progressDailog;
};

#endif // ADVANCEDSHREDDIALOG_H
