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

#include "shredwidget.h"
#include "ui_shredwidget.h"


#include <QAction>
#include <QMenu>
#include <QMenuBar>
#include <QFileDialog>
#include <QDebug>
#include <QMessageBox>
#include <QDate>
#include <QTime>

ShredWidget::ShredWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ShredWidget)
{
    ui->setupUi(this);
    ui->quickRadioButton->setChecked(true);

    QMenu *addShredButtonMenu = new QMenu;

    addFilesAction = new QAction(this);
    addDeletedAction = new QAction( this);
    addWildCardsFilesAction = new QAction(this);

    addShredButtonMenu->addAction(addFilesAction);
    addShredButtonMenu->addAction(addDeletedAction);
    addShredButtonMenu->addAction(addWildCardsFilesAction);

    ui->addShredButton->setMenu(addShredButtonMenu);

    QMenu *clearShredButtonMenu = new QMenu;

    clearSelectedAction = new QAction(this);
    clearAllAction = new QAction(this);

    clearShredButtonMenu->addAction(clearSelectedAction);
    clearShredButtonMenu->addAction(clearAllAction);

    ui->clearShredButton->setMenu(clearShredButtonMenu);

    connect(addFilesAction, SIGNAL(triggered()), this, SLOT(addFiles()));
    connect(addDeletedAction, SIGNAL(triggered()), this, SLOT(addDeletedFiles()));
    connect(addWildCardsFilesAction, SIGNAL(triggered()), this, SLOT(addAdvancedFiles()));

    connect(clearSelectedAction, SIGNAL(triggered()), this, SLOT(clearSelectedItems()));
    connect(clearAllAction, SIGNAL(triggered()), this, SLOT(clearAllItems()));
    connect(ui->shredButton, SIGNAL(clicked()), this, SLOT(shredSlot()));

    retranslateUi();
}

ShredWidget::~ShredWidget()
{
    delete ui;
}

void ShredWidget::addFile(QString filePath) {
    ui->shredListWidget->addItem(filePath);
}

void ShredWidget::setShredLevel(int level) {
    switch ( level ) {
    case 1:
        this->ui->quickRadioButton->setChecked(true);
        break;

    case 2:
        this->ui->safeRadioButton->setChecked(true);
        break;

    case 3:
        this->ui->thoroughRadioButton->setChecked(true);
        break;

    default:
        this->ui->quickRadioButton->setChecked(true);
    }
}

void ShredWidget::addAdvancedFiles() {
    AdvancedShredDialog *dialog = new AdvancedShredDialog(this);
    dialog->setModal(true);
    dialog->exec();

    if ( ! dialog->isCancled() ) {
        QStringList fileNames = dialog->getFiles();

        foreach(QString fileName, fileNames) {

            if ( ! fileName.isEmpty() && !containInList(fileName, ui->shredListWidget) )
                addFile(fileName);
        }
    }
}

void ShredWidget::addFiles(){
    QStringList fileNames = QFileDialog::getOpenFileNames(this);

    foreach(QString fileName,  fileNames) {
        if ( ! fileName.isEmpty() && !containInList(fileName, ui->shredListWidget) )
            addFile(fileName);
    }
}

bool ShredWidget:: containInList (const QString& fileName, QListWidget* widget) {
    int rows = widget->count();

    for (int i=0; i<rows; i++){
        QString text = widget->item(i)->text();

        if ( text == fileName)
            return true;
    }

    return false;
}

void ShredWidget::addDeletedFiles(){
    QString osVersion = Utilities::getOSVersion();
    QString location = "";

    if ( osVersion == "XP") {
        location = "%SystemDrive%\\RECYCLER\\%FILLPATH%";
    }
    else {
        location = "%SystemDrive%\\$Recycle.Bin\\%FILLPATH%";
    }

    QList<QString> files = showFillAllBellow(location);

    foreach(QString file, files)
        if ( ! containInList(file, ui->shredListWidget))
            addFile(file);
}

QList<QString> ShredWidget:: showFillAllBellow (const QString& location){
    QStringList locations = Utilities::handleLocation(location);
    QList<QString> files ;

    foreach(QString location , locations) {
        QList<QString> tmp = FileOperations::showAllBellow(location, "*.*",false);
        files.append(tmp);
    }

    return(files);
}

void ShredWidget::clearAllItems(){
    if ( ui->shredListWidget->count() != 0 )
        ui->shredListWidget->clear();
}

void ShredWidget::clearSelectedItems() {
    QList<QListWidgetItem*> items = ui->shredListWidget->selectedItems();

    foreach(QListWidgetItem *item, items) {
        delete item;
    }
}

void ShredWidget::shredSlot(){

    if ( ui->shredListWidget->count() <= 0 ) {
        QMessageBox::warning(this, tr("Missing Files"), tr("You Must Select Files Before Shred it"));
        return ;
    }

    OVERWRITE_TYPE type;

    if ( ui->quickRadioButton->isChecked() )
        type = ONE_OVERWRITE;

    if ( ui->safeRadioButton->isChecked() )
        type = THREE_OVERWRITE;

    if ( ui->thoroughRadioButton->isChecked())
        type = SEVEN_OVERWRITE;

    QStringList list;
    for(int i=0; i<ui->shredListWidget->count(); i++) {
        list.append( ui->shredListWidget->item(i)->text());
    }

    // check for log folder in same directory & make it if not existing
    QDir currentDir;

    if ( !currentDir.exists("Logs"))
        currentDir.mkdir("Logs");

    // make new log files
    fileName = "Logs\\Shreds- " + QDate::currentDate().toString("dd-MM-yyyy") + " " + QTime::currentTime().toString("hh-mm-ss A");
    logFile = new QFile(fileName);
    if ( ! logFile->open(QIODevice::WriteOnly | QIODevice::Text) )
        qDebug() << "Error" ;

    writer = new QTextStream(logFile);
    writer->setCodec("UTF-8");

    count = 0;
    progressDailog = new QProgressDialog(this);
    progressDailog->setModal(true);
    progressDailog->setRange(0,list.count());
    progressDailog->setLabelText(tr("Please Wait Until Deleteing All Items Selected"));

    shredFiles(list, type);

    progressDailog->setValue(list.count());
    progressDailog->cancel();
    logFile->close();

    // ask for showing report
    bool ret = Utilities::promptForReport();

    if ( ret ) {
        if ( HTMLReport::generateItemsRemovalReport(fileName) )
            qDebug() << "Generate Ok";
        else
            qDebug() << "Error Generating";
    }
}

void ShredWidget:: shredFiles (const QStringList& fileNames, OVERWRITE_TYPE type) {
    foreach(QString file, fileNames){
        QFileInfo info(file);

        count++;
        progressDailog->setValue(count);
        qApp->processEvents();
        if (progressDailog->wasCanceled()) {
           return ;
        }

        if ( info.isDir()) {
            bool ret = FileOperations::deleteAll(file);
            QString msg = ret ? "Remove True:" : "Remove Error:";
            QString resultMsg = msg + file ;
            logDeletedFile(resultMsg);
        }
        else {
            bool ret = FileOperations::deleteObject(file, type);
            QString msg = ret ? "Remove True:" : "Remove Error:";
            QString resultMsg = msg + file ;
            logDeletedFile(resultMsg);
        }
    }

    ui->shredListWidget->clear();
}

void ShredWidget::logDeletedFile(const QString &filePath) {
    writer->operator <<(filePath);
    writer->operator <<("\n");

    qDebug() << filePath;
}

void ShredWidget::retranslateUi () {
    addFilesAction->setText(tr("Add Files..."));
    addDeletedAction->setText(tr("Add all Recycle Bin Files"));
    addWildCardsFilesAction->setText(tr("Advanced Search.."));
    clearSelectedAction->setText(tr("Clear Selected Files From List"));
    clearAllAction->setText(tr("Clear All Files From List"));
}

void ShredWidget::changeEvent(QEvent* event) {
    if (event->type() == QEvent::LanguageChange) {
        ui->retranslateUi(this);
        retranslateUi();
    }
    else
        QWidget::changeEvent(event);
}
