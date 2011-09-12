#include "optionswidget.h"
#include "ui_optionswidget.h"

#include <QHBoxLayout>
#include <QDebug>

#include "utilities.h"

OptionsWidget::OptionsWidget(const QMultiMap<QString, Plugins>& category, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::OptionsWidget)
{
    ui->setupUi(this);
    this->category = category;

    pw = new PluginsWidget(this->category);

    QHBoxLayout* layout = new QHBoxLayout;
    layout->addWidget(pw);
    ui->groupBox->setLayout(layout);

    connect(ui->optionsComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(selectItemSlot(int)));
}

OptionsWidget::~OptionsWidget()
{
    delete ui;
}

void OptionsWidget:: selectItemSlot(int index) {
    emit selectItemSignal(index);
}

void OptionsWidget::on_restoreButton_clicked()
{
    pw->resetTree();
    ui->optionsComboBox->setCurrentIndex(0);

    Utilities::promptForRestore();
}

void OptionsWidget::retranslateUi () {
    ui->retranslateUi(this);
}

void OptionsWidget::changeEvent(QEvent* event) {
    if (event->type() == QEvent::LanguageChange) {
        ui->retranslateUi(this);
        //retranslate();
    }
    else
        QWidget::changeEvent(event);
}
