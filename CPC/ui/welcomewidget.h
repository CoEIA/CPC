#ifndef WELCOMEWIDGET_H
#define WELCOMEWIDGET_H

#include <QWidget>

namespace Ui {
    class WelcomeWidget;
}

class WelcomeWidget : public QWidget
{
    Q_OBJECT

public:
    explicit WelcomeWidget(QWidget *parent = 0);
    ~WelcomeWidget();
    void retranslateUi ();

private:
    Ui::WelcomeWidget *ui;

protected:
      void changeEvent(QEvent* event);

private slots:
    void beginScanProcessSlot();

signals:
    void scanProgressSignal();
};

#endif // WELCOMEWIDGET_H
