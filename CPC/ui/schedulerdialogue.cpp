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

#include "schedulerdialogue.h"
#include "ui_schedulerdialogue.h"

#include <QPushButton>

SchedulerDialogue::SchedulerDialogue(QWidget *parent) :
        QDialog(parent),
        ui(new Ui::SchedulerDialogue)
{
    ui->setupUi(this);
    this->setStyleSheet("QDialog {background-image: url(:/images/background.PNG);}");
    this->setWindowTitle(tr("Scheduler"));

    OkButton = ui->buttonBox->button(QDialogButtonBox::Ok);
    OkButton->setEnabled(false);

    for (int i=0; i<24;i++) {
        if (i<10)
            ui->HoursSB->addItem("0" + QString::number(i));
        else
            ui->HoursSB->addItem(QString::number(i));
    }

    for (int i=0; i<60;i++) {
        if (i<10)
            ui->MinsSB->addItem("0" + QString::number(i));
        else
            ui->MinsSB->addItem(QString::number(i));
    }
}

SchedulerDialogue::~SchedulerDialogue()
{
    delete ui;
}

void SchedulerDialogue::setOK(bool Enabled)
{
    if ( ui->SaturdayCB->isChecked() ||
         ui->SundayCB->isChecked() ||
         ui->MondayCB->isChecked() ||
         ui->TuesdayCB->isChecked() ||
         ui->WednesdayCB->isChecked() ||
         ui->ThursdayCB->isChecked() ||
         ui->FridayCB->isChecked() ) {
        OkButton->setEnabled(true);
    }
    else
        OkButton->setEnabled(false);
}

QString SchedulerDialogue::getTime()
{
    QString Hours   = ui->HoursSB->currentText();
    QString Mins    = ui->MinsSB->currentText();

    return Hours + ":" +Mins;
}

QStringList SchedulerDialogue::getDays() {
    QStringList list;

    if (ui->SaturdayCB->isChecked())
        list << "SAT";

    if (ui->SundayCB->isChecked())
        list << "SUN";

    if (ui->MondayCB->isChecked())
        list << "MON";

    if (ui->TuesdayCB->isChecked())
        list << "TUE";

    if (ui->WednesdayCB->isChecked())
        list << "WED";

    if (ui->ThursdayCB->isChecked())
        list << "THU";

    if (ui->FridayCB->isChecked())
        list << "FRI";

    return list;
}

void SchedulerDialogue::retranslateUi () {
    ui->retranslateUi(this);
}

void SchedulerDialogue::changeEvent(QEvent* event) {
    if (event->type() == QEvent::LanguageChange) {
        ui->retranslateUi(this);
    }
    else
        QDialog::changeEvent(event);
}
