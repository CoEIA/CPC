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
