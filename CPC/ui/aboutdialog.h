#ifndef ABOUTDIALOG_H
#define ABOUTDIALOG_H

#include <QDialog>

namespace Ui {
class aboutdialog;
}

class aboutdialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit aboutdialog(QWidget *parent = 0);
    ~aboutdialog();
    
private:
    Ui::aboutdialog *ui;
    void retranslateUi();

protected:
      void changeEvent(QEvent* event);

private slots:
    void abouthideSlot();
};

#endif // ABOUTDIALOG_H
