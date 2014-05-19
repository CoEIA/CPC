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

#ifndef PLUGINSWIDGET_H
#define PLUGINSWIDGET_H

#include <QWidget>
#include <QStandardItem>
#include <QStandardItemModel>
#include <QItemSelection>
#include <QDebug>
#include <QHBoxLayout>

#include "..\util\utilities.h"
#include "..\util\constants.h"
#include "..\util\plugins.h"
#include "..\util\MyQTreeView.h"
#include "..\util\CheckableItemModel.h"

namespace Ui {
    class PluginsWidget;
}

class PluginsWidget : public QWidget
{
    Q_OBJECT

public:
    explicit PluginsWidget(const QMultiMap<QString, Plugins>& category, QWidget *parent = 0);
    ~PluginsWidget();

private:
    Ui::PluginsWidget *ui;

public:
    CheckableItemModel *rootModel;
    QMultiMap<QString, Plugins> category;
    int itemNumber;
    void resetTree();
    void dumpModel(const QStandardItemModel *treeModel, const QModelIndex &topIndex, const  QString &str );
    void checkBranchItems(QStandardItem * parent_item);
    //QTreeView* tree;

private:
    int getRowsNumber();

    QStandardItemModel* createTreeModel( int rows = PLUGINS_SIZE);
    QList<QStandardItem*> getSubChildItems (const Plugins& plugins);
    QList<QStandardItem*> getChildItems (const QString& categoryName);
};

#endif // PLUGINSWIDGET_H
