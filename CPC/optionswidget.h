#ifndef OPTIONSWIDGET_H
#define OPTIONSWIDGET_H

#include <QWidget>
#include <QMultiMap>

#include "plugins.h"
#include "pluginswidget.h"
#include "constants.h"

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
