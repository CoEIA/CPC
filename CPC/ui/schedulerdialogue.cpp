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
        //retranslate();
    }
    else
        QDialog::changeEvent(event);
}
