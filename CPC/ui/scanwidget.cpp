#include "scanwidget.h"
#include "ui_scanwidget.h"

#include <QDebug>
#include <QStandardItem>
#include <QStandardItemModel>
#include <QVariant>
#include <QFileInfo>
#include <QGraphicsPixmapItem>
#include <QGraphicsScene>
#include <QGraphicsView>

#include "..\util\fileoperations.h"
#include "..\util\constants.h"
#include "..\util\utilities.h"
#include "..\util\pluginsresult.h"

ScanWidget::ScanWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ScanWidget)
{
    ui->setupUi(this);
    connect(ui->cancelButton, SIGNAL(clicked()), this, SLOT(cancelScanSlot()));

    connect(&thread, SIGNAL(addItemSignal(QListWidgetItem*)), this, SLOT(addItemSlot(QListWidgetItem*)));
    connect(&thread, SIGNAL(finishThreadSignal()), this, SLOT(finishThreadSlot()));
    connect(&thread, SIGNAL(progressBarSignal(int)), this, SLOT(progressBarSlot(int)));
}

ScanWidget::~ScanWidget()
{
    delete ui;
}

void ScanWidget:: clear() {
    thread.clear();
    itemsCount = 0;

    if ( thread.isRunning() ) {
        thread.stop();
        thread.terminate();
    }

    ui->listWidget->clear();
    ui->progressBar->setMinimum(0);
    ui->progressBar->setValue(0);
}

void ScanWidget::cancelScanSlot() {
    if ( thread.isRunning() ) {
        thread.stop();
        thread.terminate();
    }

    qDebug() << "Stop";

    emit showWelcomeWidgetSignal();
}

void ScanWidget::addItemSlot(QListWidgetItem* item) {
    ui->listWidget->addItem(item);
}

void ScanWidget::finishThreadSlot() {
    itemsCount = thread.getItemsCount();
    emit finishScanProcessSignal();
}

void ScanWidget::progressBarSlot(int value) {
   value = ui->progressBar->value() + 1;
   ui->progressBar->setValue(value);
   QString text = QString(tr(" %1 Item(s) Found")).arg(value);
   ui->itemLabel->setText(text);
}

void ScanWidget::startScanning(QStandardItemModel *M, const QModelIndex &Top, const QString &S, int item) {

    if ( thread.isRunning() ) {
        thread.stop();
        thread.terminate();
    }
    else {
        ui->progressBar->setMinimum(0);
        ui->progressBar->setMaximum(item);

        thread.setData(M, Top, S, item);
        thread.start();
        qDebug() << "Start";
    }
}

void ScanWidget::retranslateUi () {
    ui->retranslateUi(this);
}

void ScanWidget::changeEvent(QEvent* event) {
    if (event->type() == QEvent::LanguageChange) {
        ui->retranslateUi(this);
    }
    else
        QWidget::changeEvent(event);
}
