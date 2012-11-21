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
