#include "pluginswidget.h"
#include "ui_pluginswidget.h"
#include "utilities.h"
#include "constants.h"

#include <QDebug>
#include <QHBoxLayout>

PluginsWidget::PluginsWidget(const QMultiMap<QString, Plugins>& category, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PluginsWidget)
{
    ui->setupUi(this);
    this->category = category;
    itemNumber = 0;

    ui->treeView->setRootIsDecorated(false);
    ui->treeView->setModel(createTreeModel());
    ui->treeView->setColumnWidth(0, 350);
    ui->treeView->setHeaderHidden(true);
    ui->treeView->hideColumn(1);
}

PluginsWidget::~PluginsWidget()
{
    delete ui;
}

void PluginsWidget::resetTree() {
    QModelIndex index;
    dumpModel(rootModel, index , "");
}

void PluginsWidget:: dumpModel(
        const QStandardItemModel *treeModel, const QModelIndex &topIndex, const  QString &str ) {

    QModelIndex modelIndex;

    for( int r = 0; r <treeModel->rowCount( topIndex ); r++ ) {
        for( int c = 0; c <treeModel->columnCount( topIndex ); c++ ) {
            modelIndex = treeModel->index( r, c, topIndex );
            QStandardItem *sItem = static_cast<QStandardItem*> (modelIndex.internalPointer());
            checkBranchItems(sItem);
        }

        for( int c = 0; c <treeModel->columnCount( topIndex ); c++ ) {
             modelIndex = treeModel->index( r, c, topIndex );
             dumpModel( treeModel, modelIndex, str + " " );
        }
    }
}

void PluginsWidget:: checkBranchItems(QStandardItem * parent_item) {
    for (int i = 0; i < parent_item->rowCount(); ++i) {
        QStandardItem * child_item = parent_item->child(i);
        child_item->setCheckState(Qt::Checked);
        checkBranchItems(child_item);
    }
}

QStandardItemModel* PluginsWidget::createTreeModel( int rows) {
    int cols = 2;

    rootModel = new CheckableItemModel(rows, cols);

    for (int i=0; i<rows; i++) {
        itemNumber++;
        QStandardItem *plugins = new QStandardItem(PluginsCategory[i]);
        plugins->setEditable(false);
        plugins->setCheckable(true);
        plugins->setIcon(QIcon(":/icons/" + PluginsCategory[i] + ".PNG"));
        plugins->setCheckState(Qt::Checked);
        plugins->appendRows(getChildItems(PluginsCategory[i]));
        rootModel->setItem(i, 0, plugins);
    }

    rootModel->setHorizontalHeaderLabels(QStringList() << "Plugins" << "Status");
    return (rootModel);
}

QList<QStandardItem*> PluginsWidget::getChildItems (const QString& categoryName) {
    QList<QStandardItem*> list;
    QList<Plugins> plugins = category.values(categoryName);

    int rows = plugins.size();
    QStandardItemModel *model = new QStandardItemModel(rows, 2);

    for (int i=0; i<rows; i++){
        itemNumber++;
        QStandardItem *item = new QStandardItem(plugins.at(i).metaData.pluginsName);
        item->setData(QVariant::fromValue(plugins.at(i)));
        item->setEditable(false);
        item->setCheckable(true);
        item->setCheckState(Qt::Checked);
        item->appendRows(getSubChildItems(plugins.at(i)));
        list.append(item);
        model->setItem(i, 0, item);
    }

    return (list);
}

QList<QStandardItem*> PluginsWidget::getSubChildItems (const Plugins& plugins) {
    QList<QStandardItem*> list;

    int row = plugins.pluginsItems.size();
    for (int i=0; i<row; i++){
        //itemNumber++;
        QStandardItem *item = new QStandardItem(plugins.pluginsItems.at(i).name);
        item->setEditable(false);
        item->setCheckable(true);
        item->setCheckState(Qt::Checked);

        list.append(item);
    }

    return (list);
}

int PluginsWidget::getRowsNumber () {
    int sum = 0;

    for(int i=0; i<2; i++) {
        int value = category.values( PluginsCategory[i]).size();
        sum += value;
    }

    return sum;
}
