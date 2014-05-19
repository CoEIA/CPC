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

#include "optionswidget.h"
#include "ui_optionswidget.h"

#include <QHBoxLayout>
#include <QDebug>

#include "..\util\utilities.h"

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
