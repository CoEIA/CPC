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

#include "aboutwidget.h"
#include "ui_aboutwidget.h"

AboutWidget::AboutWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AboutWidget)
{
    ui->setupUi(this);
}

AboutWidget::~AboutWidget()
{
    delete ui;
}

void AboutWidget::on_updateButton_clicked() {
    emit updateSignal(true);
}

void AboutWidget::on_okButton_clicked() {
    QString key = ui->keyLineEdit->text().trimmed();

    if ( key.size() <= 0 ) {
        QMessageBox::warning(this,tr("Missing Information"),
                                 tr("Please Write Your Serial Number First"));
        return;
    }

    validateKey(key);
}

bool AboutWidget:: validateKey (const QString& key) {
    bool retValue = false;

    if (KeyValidator::validate(key) ) {
        Server_Query_Response state = KeyValidator::internetValidator(key);

        if ( state == CLEAN_KEY) {
            Server_Save_Response keyState = KeyValidator::writeKeyToServer(key);
            if ( keyState == SAVING_DONE) {
                writeSettings(key, KNOWN_STATE); // save info in regsitry
                readSettings();     // read settings to gui elements

                //QMessageBox::information(this,tr("Thanks For Using Our Software"),
                                        //tr("Your Registration Process is Complete Successfully"));

                retValue = true;
            }
            else {
                QMessageBox::warning(this, tr("cannot accept your key"), tr("Please try again later"));
                retValue = false;
            }
        }
        else if ( state == USED_KEY ) {
            //QMessageBox::warning(this, tr("Already Used Serial Number "), tr("Please Write Your Serial Number"));
            retValue = false;
        }
        else if ( state == ILLEGAL_KEY) {
            QMessageBox::warning(this, tr("Illegal Serial Number"), tr("Please Write Your Serial Number Correctly"));
            retValue = false;
        }
        else {
            //writeSettings(key, UNKNOWN_STATE); // save info in regsitry

            //QUICK Fix Of GPL:
            writeSettings(key, KNOWN_STATE); // save info in regsitry
            readSettings();     // read settings to gui elements

            //QMessageBox::information(this, tr("CPC Need Internet Connection"), tr("your licence is accepted but you must establish internet connection to do last checking"));
            retValue = true;
        }
    }
    else {
        QMessageBox::warning(this, tr("Incorrect Serial Number"),
                             tr("Please Write Your Serial Number Correctly"));
        retValue = false;
    }

    return retValue;
}

void AboutWidget:: writeSettings (const QString& key, const KeyState& state) {
    KeySettings settings(key,"LifeTime", "Free Scan and Free Remove", state);
    SettingsHandler::writeKey(settings);
}

void AboutWidget::readSettings() {
    KeySettings settings = SettingsHandler::readKey();
    bool registerdVersion = KeyValidator::validate(settings.key);
    settingsValue(settings, registerdVersion);
}

void AboutWidget::settingsValue(const KeySettings& settings,  bool reg) {
    ui->keyLabel->setText(settings.key);
    ui->versionLabel->setText(settings.version);
    ui->timeLabel->setText(settings.time);

    if ( reg ) {
        ui->keyLineEdit->hide();
        ui->okButton->hide();
        ui->enterLabel->hide();
    }
    else {
        ui->keyLineEdit->show();
        ui->okButton->show();
        ui->enterLabel->show();

        QDateTime expireTime = settings.installTime.addDays(FREE_DAYS);
        int availableDays = QDateTime::currentDateTime().daysTo(expireTime);

        ui->timeLabel->setText(QString("%1 Days").arg(availableDays));

        // free version, write install time & last usage time.
        //SettingsHandler:: writeInstallTime(settings.installTime);
        //SettingsHandler:: writeLastUsageTime(settings.lastUsageTime);
    }
}

void AboutWidget::retranslateUi () {
    readSettings();
}

void AboutWidget::changeEvent(QEvent* event) {
    if (event->type() == QEvent::LanguageChange) {
        ui->retranslateUi(this);
        retranslateUi();
    }
    else
        QWidget::changeEvent(event);
}
