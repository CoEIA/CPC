#include "aboutdialog.h"
#include "ui_aboutdialog.h"

#include <QApplication>
#include <QDialog>
#include <QDebug>

aboutdialog::aboutdialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::aboutdialog)
{
    ui->setupUi(this);
    this->setWindowTitle(tr("About"));
    this->connect(qApp,SIGNAL(lastWindowClosed()),this,SLOT(abouthideSlot()));
}

aboutdialog::~aboutdialog()
{
    delete ui;
}

void aboutdialog::abouthideSlot()
{
     qDebug() << "in hide slot";
     showNormal();
     this->setVisible(false);
}
