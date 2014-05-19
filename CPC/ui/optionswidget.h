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

#ifndef OPTIONSWIDGET_H
#define OPTIONSWIDGET_H

#include <QWidget>
#include <QMultiMap>

#include "pluginswidget.h"

#include "..\util\plugins.h"
#include "..\util\constants.h"

namespace Ui {
    class OptionsWidget;
}

class OptionsWidget : public QWidget
{
    Q_OBJECT

public:
    explicit OptionsWidget(const QMultiMap<QString, Plugins>& category, QWidget *parent = 0);
    ~OptionsWidget();

private:
    Ui::OptionsWidget *ui;

protected:
      void changeEvent(QEvent* event);

private:
    QMultiMap<QString, Plugins> category;

public:
    PluginsWidget *pw ;
    int getItemNumber () { return pw->itemNumber; }
    void retranslateUi();

private slots:
    void on_restoreButton_clicked();
    void selectItemSlot(int index);

signals:
    void selectItemSignal(int index);
};

#endif // OPTIONSWIDGET_H
