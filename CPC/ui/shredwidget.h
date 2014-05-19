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

#ifndef SHREDWIDGET_H
#define SHREDWIDGET_H

#include <QWidget>
#include <QListWidget>
#include <QProgressDialog>
#include <QFile>

#include "advancedshreddialog.h"

#include "..\util\constants.h"
#include "..\util\fileoperations.h"
#include "..\util\utilities.h"
#include "..\util\htmlreport.h"

namespace Ui {
    class ShredWidget;
}

class ShredWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ShredWidget(QWidget *parent = 0);
    ~ShredWidget();
    void retranslateUi();

    void addFile(QString filepath);
    void setShredLevel(int level);

private:
    Ui::ShredWidget *ui;

private slots:
    void clearAllItems();
    void clearSelectedItems();
    void addFiles();
    void addAdvancedFiles();
    void addDeletedFiles();
    void shredSlot();

protected:
      void changeEvent(QEvent* event);

private:
    bool containInList (const QString& fileName,  QListWidget* widget) ;
    void shredFiles (const QStringList& fileNames, OVERWRITE_TYPE type);
    QList<QString> showFillAllBellow(const QString& location);

private:
    QProgressDialog *progressDailog;
    int count;
    void logDeletedFile(const QString& filePath);
    QFile *logFile;
    QTextStream *writer;
    QString fileName;

private:
     QAction *addFilesAction, *addDeletedAction, *addWildCardsFilesAction,
                *clearSelectedAction, *clearAllAction;
};

#endif // SHREDWIDGET_H
