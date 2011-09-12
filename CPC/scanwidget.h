#ifndef SCANWIDGET_H
#define SCANWIDGET_H

#include <QWidget>
#include <QStandardItemModel>
#include <QPair>

#include "plugins.h"
#include "pluginsresult.h"
#include "scannerthread.h"

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
