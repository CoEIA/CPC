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

#ifndef SCHEDULEWIDGET_H
#define SCHEDULEWIDGET_H

#include <QWidget>
#include <QListWidgetItem>

#include "schedulerdialogue.h"

namespace Ui {
    class ScheduleWidget;
}

class ScheduleWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ScheduleWidget(QWidget *parent = 0);
    ~ScheduleWidget();
    void retranslateUi();

private:
    Ui::ScheduleWidget *ui;
    SchedulerDialogue * m_dialogue;

private slots:
    void on_newButton_pressed();
    void on_deleteButton_pressed();

protected:
      void changeEvent(QEvent* event);

private:
    void createScheduleFolder() ;
    void updateScheduleList();
    QList<QString> getTasksFilePath();
    QStringList readTaskInfo (const QString& taskPath);
    void writeTaskInfo(const QString& path, const QStringList& info);
    bool deleteTask (const QString& taskName);
    bool createNewTask(const QString& taskName, const QString& time, const QString& days) ;
    bool isTaskInfoExist (QStringList& taskInfo);
    QString getNextTaskName() ;
    QList<QStringList> getAllTasks ();
    void checkForDisableDeleteButton();
    QString getFormattedDays (const QStringList& days);
};

#endif // SCHEDULEWIDGET_H
