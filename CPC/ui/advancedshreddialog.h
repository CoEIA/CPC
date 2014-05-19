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
