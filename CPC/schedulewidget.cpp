#include "schedulewidget.h"
#include "ui_schedulewidget.h"

#include <QFile>
#include <QDir>
#include <QTextStream>
#include <QDebug>
#include <QFileInfo>
#include <QMessageBox>

#include "utilities.h"
#include "fileoperations.h"

ScheduleWidget::ScheduleWidget(QWidget *parent) :
        QWidget(parent),
        ui(new Ui::ScheduleWidget)
{
    ui->setupUi(this);

    createScheduleFolder();
    updateScheduleList();
}

ScheduleWidget::~ScheduleWidget() {
    delete ui;

    if (m_dialogue !=NULL) {
        delete m_dialogue;
        m_dialogue=NULL;
    }
}

void ScheduleWidget::on_deleteButton_pressed() {
    if ( ! ui->listWidget->currentItem()) {
        QMessageBox::warning(this,tr("Task is not selected"),
                                 tr("please select the task first then press delete button"));
        return ;
    }

    QString taskFileName = ui->listWidget->currentItem()->text().split(":").at(1).split("Run").at(0).trimmed();

    if ( deleteTask(taskFileName) ) {
        QMessageBox::information(this,tr("Task deleteing is done"),
                tr("Task Deleting done"));
    }
    else {
        QMessageBox::warning(this,tr("Task Deleteing Error"),
                                 tr("cannot remove task"));
    }

    updateScheduleList();
}

void ScheduleWidget::on_newButton_pressed() {
    m_dialogue = new SchedulerDialogue(this);

    if ( m_dialogue->exec() == QDialog::Accepted ) {
        QStringList daysNotFormatted = m_dialogue->getDays();
        QString days = getFormattedDays(daysNotFormatted);
        QString time = m_dialogue->getTime();
        QString taskName = getNextTaskName();

        if ( createNewTask(taskName, time, days) )
            QMessageBox::information(this,tr("New Task Done"),
                    tr("You Task will be run in the time"));
        else
            QMessageBox::warning(this,tr("there is a task with the same information"),
                                     tr("Task is Already Exist"));
    }

    updateScheduleList();
}

void ScheduleWidget:: createScheduleFolder() {
    QDir currentDir;

    if ( !currentDir.exists("Schedules"))
        currentDir.mkdir("Schedules");
}

void ScheduleWidget:: checkForDisableDeleteButton() {
    QList<QStringList> tasks = getAllTasks();

    if ( tasks.isEmpty() ) {
        ui->listWidget->clear();
        ui->deleteButton->setEnabled(false);
        ui->listWidget->addItem(tr("The Scan Schedule is not active"));
    }
    else
        ui->deleteButton->setEnabled(true);
}

void ScheduleWidget:: updateScheduleList() {
    QList<QStringList> tasks = getAllTasks();
    ui->listWidget->clear();

    foreach(QStringList task, tasks) {
        QString name = task.at(0);
        QString time = task.at(1);
        QString days = task.at(2);

        QString row = "Task: " + name + " Run At: " + time + " On Day(s) : " + days;
        ui->listWidget->addItem(row);
    }

    checkForDisableDeleteButton();
}

QList<QString> ScheduleWidget:: getTasksFilePath() {
    QFileInfo fileInfo("Schedules");
    QString path = fileInfo.absoluteFilePath();

    return FileOperations::showAll(path, "*.txt", false);
}

QStringList ScheduleWidget:: readTaskInfo (const QString& taskPath) {
    QFile file(taskPath);
    QStringList info;

    if ( file.open(QIODevice::ReadOnly | QIODevice::Text ) ) {
        QTextStream stream(&file);

        info << stream.readLine();
        info << stream.readLine();
        info << stream.readLine();
    }

    file.close();
    return (info);
}

void ScheduleWidget:: writeTaskInfo(const QString& path, const QStringList& info) {
    QString fullPath = "Schedules\\" + path;
    QFile file(fullPath);

    if ( file.open(QIODevice::WriteOnly | QIODevice::Text )) {
        QTextStream stream(&file);

        stream.operator <<( info.at(0) + "\n");
        stream.operator <<( info.at(1) + "\n");
        stream.operator <<( info.at(2) + "\n");
    }

    file.close();
}

QList<QStringList> ScheduleWidget:: getAllTasks () {
    QList<QString> tasksPath = getTasksFilePath();
    QList<QStringList> allTasks;

    foreach(QString taskPath, tasksPath) {
        allTasks.append( readTaskInfo(taskPath));
    }

    return (allTasks);
}

QString ScheduleWidget:: getNextTaskName() {
    QList<QStringList> tasks = getAllTasks();
    QString fileName ;

    if (tasks.isEmpty() )
        fileName = "0";
    else {
        QStringList lastTask = tasks.last();
        int number = lastTask.at(0).toInt() + 1;
        fileName = QString("%1").arg(number);
    }

    return fileName;
}

bool ScheduleWidget:: isTaskInfoExist (QStringList& taskInfo) {
    QList<QStringList> tasks = getAllTasks();

    foreach(QStringList task, tasks) {
        QString time = task.at(1);
        QString days = task.at(2);

        if ( time == taskInfo.at(1) &&
             days == taskInfo.at(2) )
            return true;
    }

    return false;
}

bool ScheduleWidget:: createNewTask(const QString& taskName, const QString& time, const QString& days) {
    QStringList info;
    info << taskName << time << days;

    bool result = false;

    if ( isTaskInfoExist(info)) {
        result = false;
    }
    else {
        QString filePath = taskName + ".txt";
        writeTaskInfo( filePath, info);
        result = true;
    }

    return (result);
}

bool ScheduleWidget:: deleteTask (const QString& taskName) {
    QString filePath = "Schedules\\" + taskName + ".txt";
    bool returnValue = false;

    QFile file(filePath);
    if ( file.exists() ) {
        returnValue = file.remove();
    }

    file.close();
    return returnValue;
}

QString ScheduleWidget:: getFormattedDays (const QStringList& days) {
    QString result = "";

    if ( ! days.isEmpty() ) {
        for(int i=0; i<days.count(); i++) {
            result += days.at(i);

            if ( i < days.count() -1 )
                result += ",";
        }
    }

    return result;
}

void ScheduleWidget::retranslateUi () {
    ui->retranslateUi(this);
}

void ScheduleWidget::changeEvent(QEvent* event) {
    if (event->type() == QEvent::LanguageChange) {
        ui->retranslateUi(this);
        checkForDisableDeleteButton();
    }
    else
        QWidget::changeEvent(event);
}
