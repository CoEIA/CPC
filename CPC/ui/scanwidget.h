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

#ifndef SCANWIDGET_H
#define SCANWIDGET_H

#include <QWidget>
#include <QStandardItemModel>
#include <QPair>

#include "..\util\plugins.h"
#include "..\util\pluginsresult.h"
#include "..\util\scannerthread.h"

namespace Ui {
    class ScanWidget;
}

class ScanWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ScanWidget(QWidget *parent = 0);
    ~ScanWidget();
    int getItemsCount() { return itemsCount; }

private:
    Ui::ScanWidget *ui;

protected:
    void changeEvent(QEvent* event);

private slots:
    void cancelScanSlot();
    void addItemSlot(QListWidgetItem*);
    void finishThreadSlot();
    void progressBarSlot(int);

signals:
    void showWelcomeWidgetSignal();
    void finishScanProcessSignal();

public:
    void clear();
    void startScanning(QStandardItemModel * M, const QModelIndex & Top, const  QString & S, int item);

    QMultiMap<QString, PluginsResult> getPluginsMap () {
        return thread.getPluginsMap();
    }

private:
    ScannerThread thread;
    int itemsCount;
    void retranslateUi();
};

#endif // SCANWIDGET_H
