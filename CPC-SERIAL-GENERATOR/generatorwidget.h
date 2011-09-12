#ifndef GENERATORWIDGET_H
#define GENERATORWIDGET_H

#include <QWidget>
#include <QtSql/QtSql>

namespace Ui {
    class GeneratorWidget;
}

class GeneratorWidget : public QWidget
{
    Q_OBJECT

public:
    explicit GeneratorWidget(QWidget *parent = 0);
    ~GeneratorWidget();

private:
    Ui::GeneratorWidget *ui;

public slots:
    void addSerial();
    void deleteSerial();
    void check();
    void showSerial();

private:
    QSqlDatabase database;
    QSqlTableModel *model ;

private:
    QString getTime() ;
    QString getNextId();
    QString getRandomChars (int length);
    bool existKey (QString key);

};

#endif // GENERATORWIDGET_H
