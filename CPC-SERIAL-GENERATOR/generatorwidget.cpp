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

#include "generatorwidget.h"
#include "ui_generatorwidget.h"

#include "KeyGenerator.h"
#include "KeyValidator.h"

#include <QMessageBox>

GeneratorWidget::GeneratorWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::GeneratorWidget)
{
    ui->setupUi(this);

    qDebug() << QSqlDatabase::drivers();

    database = QSqlDatabase::addDatabase("QMYSQL");
    database.setDatabaseName("CPCLICENCE");
    database.setHostName("localhost");
    database.setUserName("root");
    database.setPassword("");

    if ( ! database.open() ) {
        qDebug() << "Error: " << database.lastError().text();
        return ;
    }

    model = new QSqlTableModel;
    model->setTable("GeneratedTable");
    model->select();

    connect(ui->saveIntoDbButton, SIGNAL(clicked()), this, SLOT(addSerial()));
    connect(ui->removeButton, SIGNAL(clicked()), this, SLOT(deleteSerial()));
    connect(ui->generateButton, SIGNAL(clicked()), this, SLOT(showSerial()));
    connect(ui->checkButton, SIGNAL(clicked()), this, SLOT(check()));

    ui->tableView->setModel(model);
    ui->tableView->setAlternatingRowColors(true);
    ui->tableView->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableView->resizeColumnToContents(1);
    ui->tableView->resizeColumnToContents(0);
    ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
}

GeneratorWidget::~GeneratorWidget()
{
    delete ui;
}

void GeneratorWidget:: showSerial() {

    QString time = getTime();
    QString id = getNextId();
    QString constantNumber = "658712";

    QString key = KeyGenerator::generateKey(time, id, constantNumber);

    if ( !key.endsWith("+") && KeyValidator::validate(key) ) {
        ui->serialLineEdit->setText(key);
    }
}

void GeneratorWidget:: check () {
    int count = 0;
    for (int i=0; i<100 ; i++ ){
        QString time = getTime();
        QString id = getNextId();
        QString constantNumber = "658712";

        QString key = KeyGenerator::generateKey(time, id, constantNumber);

        if ( KeyValidator::validate(key) ) {
            ui->serialLineEdit->setText(key);

            if ( ! existKey (key) && ! key.endsWith("+") ) {
                QSqlQuery query;
                QString time = QDate::currentDate().toString();
                if ( query.exec("insert into GeneratedTable(serial, generateTime) values('" + key + "' , '" + time +"')") ) {
                    qDebug() << "key: " << key;
                    count++;
                }
            }
        }
    }

    qDebug() << "Number of Key Saved: " << count;
}

void GeneratorWidget::addSerial() {
    QString key = ui->serialLineEdit->text();

    if ( KeyValidator::validate(key) ) {
        if ( ! existKey (key) && ! key.endsWith("+") ) {
            QSqlQuery query;
            QString time = QDate::currentDate().toString();
            if ( query.exec("insert into GeneratedTable(serial, generateTime) values('" + key + "' , '" + time +"')") )
                QMessageBox::information(this,"inserting Ok","inserting Ok");
            else
                QMessageBox::warning(this,"cannot inserting key","cannot inserting key");
        }
        else {
            QMessageBox::warning(this,"Key existing in DB","Key existing in DB");
        }
    }
    else {
        QMessageBox::warning(this,"is not valid key", key + " is not valid key");
    }

    model->select();
}

void GeneratorWidget::deleteSerial() {
    int no = model->index(ui->tableView->currentIndex().row(),0).data().toInt();

    QSqlQuery query;
    query.prepare("DELETE FROM GeneratedTable WHERE id = ?");
    query.addBindValue(no);

    query.exec();

    model->select();
}


bool GeneratorWidget:: existKey (QString key) {
   QSqlQuery query;

   query.prepare("SELECT * FROM GeneratedTable WHERE serial = ?");
   query.addBindValue(key);
   query.exec();

   while (query.next()) {
       return true;
   }

   return false;
}

QString GeneratorWidget:: getTime () {
    QTime time = QTime::currentTime();
    QString result = QString::number(time.msec());

    return result;
}

QString GeneratorWidget:: getNextId () {
    QString id = getRandomChars(5);
    return id;
}

QString GeneratorWidget:: getRandomChars (int length) {
    int difference = 'Z'-'A';
    QString result = "";

    for (int i=0; i<length; i++) {
        char c = 'A' + ( rand() % difference );
        result += c ;
    }

    return result;
}
