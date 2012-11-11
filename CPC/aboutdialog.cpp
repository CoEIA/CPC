#include "aboutdialog.h"
#include "ui_aboutdialog.h"

#include <QDialog>

aboutdialog::aboutdialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::aboutdialog)
{
    ui->setupUi(this);
    this->setWindowTitle(tr("About"));
}

aboutdialog::~aboutdialog()
{
    delete ui;
}
