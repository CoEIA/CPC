#include "welcomewidget.h"
#include "ui_welcomewidget.h"
#include "scanwidget.h"

#include <QVBoxLayout>
#include <QDebug>

WelcomeWidget::WelcomeWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::WelcomeWidget)
{
    ui->setupUi(this);
    connect(ui->startScanButton, SIGNAL(clicked()), this, SLOT(beginScanProcessSlot()));
}

WelcomeWidget::~WelcomeWidget()
{
    delete ui;
}

void WelcomeWidget::beginScanProcessSlot(){
    emit scanProgressSignal();
}

void WelcomeWidget::retranslateUi () {
    ui->retranslateUi(this);
}

void WelcomeWidget::changeEvent(QEvent* event) {
    if (event->type() == QEvent::LanguageChange) {
        ui->retranslateUi(this);
        retranslateUi();
    }
    else
        QWidget::changeEvent(event);
}
