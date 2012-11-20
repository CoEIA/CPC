#ifndef SHREDWIDGET_H
#define SHREDWIDGET_H

#include <QWidget>
#include <QListWidget>
#include <QProgressDialog>
#include <QFile>

#include "constants.h"

namespace Ui {
    class ShredWidget;
}

class ShredWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ShredWidget(QWidget *parent = 0);
    ~ShredWidget();
    void retranslateUi();

    void addFile(QString filepath);
    void setShredLevel(int level);

private:
    Ui::ShredWidget *ui;

private slots:
    void clearAllItems();
    void clearSelectedItems();
    void addFiles();
    void addAdvancedFiles();
    void addDeletedFiles();
    void shredSlot();

protected:
      void changeEvent(QEvent* event);

private:
    bool containInList (const QString& fileName,  QListWidget* widget) ;
    void shredFiles (const QStringList& fileNames, OVERWRITE_TYPE type);
    QList<QString> showFillAllBellow(const QString& location);

private:
    QProgressDialog *progressDailog;
    int count;
    void logDeletedFile(const QString& filePath);
    QFile *logFile;
    QTextStream *writer;
    QString fileName;

private:
     QAction *addFilesAction, *addDeletedAction, *addWildCardsFilesAction,
                *clearSelectedAction, *clearAllAction;
};

#endif // SHREDWIDGET_H
