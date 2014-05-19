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

#ifndef SCHEDULERDIALOGUE_H
#define SCHEDULERDIALOGUE_H

#include <QDialog>

namespace Ui {
    class SchedulerDialogue;
}

class SchedulerDialogue : public QDialog
{
    Q_OBJECT

public:
    explicit SchedulerDialogue(QWidget *parent = 0);
    ~SchedulerDialogue();

private:
    Ui::SchedulerDialogue *ui;
    QPushButton* OkButton;
    void retranslateUi();

protected:
      void changeEvent(QEvent* event);

public:
   QStringList getDays();
   QString     getTime();

public slots:
   void setOK(bool Enabled);

};

#endif // SCHEDULERDIALOGUE_H
