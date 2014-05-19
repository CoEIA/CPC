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
