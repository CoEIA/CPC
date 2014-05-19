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

#ifndef ABOUTWIDGET_H
#define ABOUTWIDGET_H

#include <QWidget>
#include <QMessageBox>

#include "..\util\settingshandler.h"
#include "..\util\constants.h"
#include "..\util\keyvalidator.h"
#include "..\util\internethandler.h"
#include "..\util\constants.h"
#include "..\util\utilities.h"

namespace Ui {
    class AboutWidget;
}

class AboutWidget : public QWidget
{
    Q_OBJECT

public:
    explicit AboutWidget(QWidget *parent = 0);
    ~AboutWidget();

private:
    Ui::AboutWidget *ui;

protected:
    void changeEvent(QEvent* event);

public:
    void settingsValue(const KeySettings& settings, bool state);
    void readSettings();
    bool validateKey(const QString& key) ;
    void retranslateUi();

private slots:
    void on_okButton_clicked();
    void on_updateButton_clicked();

signals:
    void updateSignal(bool showMessage);

private:
    void writeSettings (const QString& key, const KeyState& state);
    bool isFreeVersion();
};

#endif // ABOUTWIDGET_H
