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

#include "advancedshreddialog.h"
#include "ui_advancedshreddialog.h"

#include "..\util\fileoperations.h"

#include <QFileDialog>
#include <QDebug>

AdvancedShredDialog::AdvancedShredDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AdvancedShredDialog)
{
    ui->setupUi(this);
    this->setWindowTitle(tr("Advanced Shred Options"));
    this->setStyleSheet("QDialog {background-image: url(:/images/background.PNG);}");

    progressDailog = new QProgressDialog(tr("Please wait until getting all items that match your need"),tr("Cancel"), 0,0);
    progressDailog->setModal(true);

    connect(ui->selectFilesButton, SIGNAL(clicked()), this, SLOT(selectFiles()));
    connect(ui->addFilesButton, SIGNAL(clicked()), this, SLOT(addFiles()));
    connect(ui->okButton, SIGNAL(clicked()), this, SLOT(AcceptFiles()));
    connect(ui->cancelButton, SIGNAL(clicked()), this, SLOT(cancelFiles()));

    connect(&thread, SIGNAL(addItemSignal(QString)), this, SLOT(addItemSlot(QString)));
    connect(&thread, SIGNAL(finishThreadSignal()), this, SLOT(finishEnumSlot()));

    connect(progressDailog, SIGNAL(canceled()), this, SLOT(progressCanceled()));
}

AdvancedShredDialog::~AdvancedShredDialog()
{
    delete ui;
}

void AdvancedShredDialog::addItemSlot(QString fileName) {
    if ( ! fileName.isEmpty() && !containInList(fileName) ) {
        ui->filesResultList->addItem(fileName);
        resultFiles.append(fileName);
    }
}

void AdvancedShredDialog::finishEnumSlot() {
    stopThread();
    progressDailog->cancel();
    isCancel = false;
}

void AdvancedShredDialog::selectFiles() {
    QString filePath = QFileDialog::getExistingDirectory(this, tr("Open Directory"),
                "/home",
                QFileDialog::ShowDirsOnly
                | QFileDialog::DontResolveSymlinks);

    if ( ! filePath.isEmpty() )
        ui->pathEdit->setText(filePath);
}

void AdvancedShredDialog::addFiles() {
    QString filePath = ui->pathEdit->text();

    if ( ! filePath.isEmpty() ) {
        QDir dir(filePath);

        if ( dir.exists() ) {
            QStringList exts = ui->extensionEdit->text().split(" ");

            thread.setData(filePath, exts);
            thread.start();

            progressDailog->show();
        }
    }
}

void AdvancedShredDialog::progressCanceled() {
    stopThread();
}

void AdvancedShredDialog::AcceptFiles() {
    isCancel = false;
    this->hide();
}

void AdvancedShredDialog::cancelFiles() {
    stopThread();
    progressDailog->cancel();
    isCancel = true;
    this->hide();
}

void AdvancedShredDialog:: stopThread() {
    if ( thread.isRunning() ) {
        thread.stop();
        thread.terminate();
    }
}

bool AdvancedShredDialog:: containInList (const QString& fileName) {
    int rows = ui->filesResultList->count();

    for (int i=0; i<rows; i++){
        QString text = ui->filesResultList->item(i)->text();

        if ( text == fileName)
            return true;
    }

    return false;
}

void AdvancedShredDialog::retranslateUi () {
    ui->retranslateUi(this);
}

void AdvancedShredDialog::changeEvent(QEvent* event) {
    if (event->type() == QEvent::LanguageChange) {
        ui->retranslateUi(this);
        //retranslate();
    }
    else
        QDialog::changeEvent(event);
}
