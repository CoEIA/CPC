#include <QtGui/QApplication>
#include <QTranslator>

#include "mainwindow.h"
#include "application.h"
#include "utilities.h"

int main(int argc, char *argv[])
{
    Application a(argc, argv);
    a.loadTranslations(":/translations");
    Application::setLanguage(QLocale::system().name());
    a.setLayoutDirection(QObject::tr("LTR")=="RTL" ? Qt::RightToLeft : Qt::LeftToRight);

    QStringList args = a.arguments();

    QTextCodec::setCodecForCStrings(QTextCodec::codecForName("UTF-8"));
    QTextCodec::setCodecForTr(QTextCodec::codecForName("UTF-8"));

    QDir::setCurrent(QCoreApplication::applicationDirPath());

    Utilities::loadEnglishStyle();

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
