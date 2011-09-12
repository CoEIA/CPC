#ifndef ABOUTWIDGET_H
#define ABOUTWIDGET_H

#include <QWidget>

#include "settingshandler.h"
#include "constants.h"

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
