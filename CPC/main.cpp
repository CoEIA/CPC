#include <QtGui/QApplication>
#include <QTranslator>

#include "mainwindow.h"
#include "application.h"

int main(int argc, char *argv[])
{
    //QApplication a(argc, argv);

    Application a(argc, argv);
    a.loadTranslations(":/translations");
    Application::setLanguage(QLocale::system().name());
    a.setLayoutDirection(QObject::tr("LTR")=="RTL" ? Qt::RightToLeft : Qt::LeftToRight);

    QStringList args = a.arguments();

    QTextCodec::setCodecForCStrings(QTextCodec::codecForName("UTF-8"));
    QTextCodec::setCodecForTr(QTextCodec::codecForName("UTF-8"));

    QDir::setCurrent(QCoreApplication::applicationDirPath());

    QFile file(":/TestingGUI.qss");
    file.open(QFile::ReadOnly);
    QString StyleSheet = QLatin1String(file.readAll());
    a.setStyleSheet(StyleSheet);

    if ( args.count() > 1 ) {
        MainWindow w(true);
        w.show();
        return a.exec();
    }
    else {
        MainWindow w;
        w.show();
        return a.exec();
    }
}
