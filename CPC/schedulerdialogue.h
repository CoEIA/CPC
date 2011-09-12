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
