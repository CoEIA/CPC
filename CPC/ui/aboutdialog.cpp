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
    this->setStyleSheet("QDialog {background-image: url(:/images/background.PNG);}");
    this->setWindowTitle(tr("About"));
    this->connect(qApp,SIGNAL(lastWindowClosed()),this,SLOT(abouthideSlot()));
}

aboutdialog::~aboutdialog()
{
    delete ui;
}

void aboutdialog::abouthideSlot()
{
     showNormal();
     this->setVisible(false);
}

void aboutdialog::retranslateUi () {
    ui->retranslateUi(this);
}

void aboutdialog::changeEvent(QEvent* event) {
    if (event->type() == QEvent::LanguageChange) {
        ui->retranslateUi(this);
    }
    else
        QDialog::changeEvent(event);
}
