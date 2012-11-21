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
