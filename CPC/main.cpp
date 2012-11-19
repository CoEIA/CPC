#include <QtGui/QApplication>
#include <QTranslator>

#include "mainwindow.h"
#include "application.h"
#include "utilities.h"
#include "commandlineparser.h"

// CPC Application have 4 different Parameters:
// CPC.exe -Scan
// CPC.exe -Shred_Quick   filepath
// CPC.exe -Shred_Safe    filepath
// CPC.exe -Shred_Through filepath

int main(int argc, char *argv[])
{
    Application a(argc, argv);
    a.loadTranslations(":/translations");
    Application::setLanguage(QLocale::system().name());
    a.setLayoutDirection(QObject::tr("LTR")=="RTL" ? Qt::RightToLeft : Qt::LeftToRight);

    QTextCodec::setCodecForCStrings(QTextCodec::codecForName("UTF-8"));
    QTextCodec::setCodecForTr(QTextCodec::codecForName("UTF-8"));

    QDir::setCurrent(QCoreApplication::applicationDirPath());
    Utilities::loadEnglishStyle();

    QStringList args = a.arguments();
    CommandLineParser parser(args);

    MainWindow w(parser);
    w.show();

    return a.exec();
}
