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
