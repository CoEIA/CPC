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

#ifndef RESULTWIDGET_H
#define RESULTWIDGET_H

#include <QWidget>
#include <QMultiMap>
#include <QStandardItemModel>
#include <QProgressDialog>
#include <QFile>

#include "..\util\pluginsresult.h"
#include "..\util\MyQTreeView.h"
#include "..\util\CheckableItemModel.h"

namespace Ui {
    class ResultWidget;
}

class ResultWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ResultWidget(QWidget *parent = 0);
    ~ResultWidget();
    int getItemsCount() { return itemsCount; }
    void setItemsCount (int c) { itemsCount = c; }

private:
    Ui::ResultWidget *ui;
    //MyQTreeView* treeView;
    //QTreeView* treeView;

protected:
     void changeEvent(QEvent* event);

public:
    void setPluginsMap(QMultiMap<QString, PluginsResult> plugins);
    void dumpModel(const QStandardItemModel * M, const QModelIndex & Top, const  QString & S);
    int getRowsNumber();
    void handlePluginsRemoving(QString& text, PluginsResult& result);

    QList<QStandardItem*> getResultSubChildItems (const PluginsResultItems& plugins);
    QList<QStandardItem*> getSubChildItems (const PluginsResult& plugins) ;
    QList<QStandardItem*> getChildItems (const QString& categoryName);

    QStandardItemModel*createTreeModel() ;
    void retranslateUi();

private:
    QMultiMap<QString, PluginsResult>  pluginsMap;
    //QStandardItemModel *rootModel;
    CheckableItemModel *rootModel;
    int itemsCount ;
    QProgressDialog *progressDailog;
    int count;

    void logDeletedFile(const QString& filePath);
    QFile *logFile;
    QTextStream *writer;
    QString fileName;

private slots:
    void backSlot();
    void removeSlot();

signals:
    void showWelcomeWidgetSignal();
};

#endif // RESULTWIDGET_H
