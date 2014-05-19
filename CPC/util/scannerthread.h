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

#ifndef SCANNERTHREAD_H
#define SCANNERTHREAD_H

#include <QThread>
#include <QStandardItemModel>
#include <QListWidgetItem>
#include <QMultiMap>

#include "plugins.h"
#include "pluginsresult.h"

class ScannerThread: public QThread
{
    Q_OBJECT

public:
    ScannerThread();
    void clear();
    void setData(QStandardItemModel * M, const QModelIndex & Top, const  QString & S, int item);
    int getItemsCount() { return itemsCount; }

    QMultiMap<QString, PluginsResult> getPluginsMap () {
        return pluginsMap;
    }

signals:
    void progressBarSignal(int);
    //void showImage(const QString& imgPath);
    void addItemSignal(QListWidgetItem*);
    void finishThreadSignal();

public slots:
    void stop() { stopped = true; }

private:
    void run();
    volatile bool stopped;

private:
    void dumpModel(const QStandardItemModel * M, const QModelIndex & Top, const  QString & S );
    void handlePlugins (Plugins& plugins, const QString& text);

    QList<QString> showAllBellow (PluginsPath& path);
    QList<QString> showFillAllBellow (PluginsPath& path);
    QList<QString> showMatchWithinSub (PluginsPath& path);
    QList<QString> showFillMatchWithinSub (PluginsPath& path);
    QList<QString> showMatch (PluginsPath& path);
    QList<QString> showFillMatch (PluginsPath& path);

    QList<QString> showAllValuesBellow (PluginsPath& path);
    QList<QString> showAllKeysBellow (PluginsPath& path);

    void showInList(const QList<QString>& paths, Plugins& plugins);
    bool isImage   (const QString& path);
    void showImage (const QString& path);

    QString getIcon (const QString& name);

private:
    QStandardItemModel* modelPtr;
    QModelIndex index;
    QString text;
    int items;
    QModelIndexList modelList;

private:
    QStandardItemModel model;
    QString categoryName;
    QMultiMap<QString, PluginsResult> pluginsMap;
    int itemsCount ;
};

#endif // SCANNERTHREAD_H
