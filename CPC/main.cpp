#include <QtGui/QApplication>
#include <QTranslator>

#include <qtsingleapplication.h>
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
    QtSingleApplication instance(argc, argv);

    if (instance.sendMessage("There is already opened instance of this CPC.exe"))
        return 0;

//    Application a(argc, argv);
//    a.loadTranslations(":/translations");
//    Application::setLanguage(QLocale::system().name());
//    a.setLayoutDirection(QObject::tr("LTR")=="RTL" ? Qt::RightToLeft : Qt::LeftToRight);

    QTextCodec::setCodecForCStrings(QTextCodec::codecForName("UTF-8"));
    QTextCodec::setCodecForTr(QTextCodec::codecForName("UTF-8"));

    QDir::setCurrent(QCoreApplication::applicationDirPath());
    Utilities::loadEnglishStyle();

    QStringList args = instance.arguments();
    CommandLineParser parser(args);

    MainWindow w(parser);
    w.show();

    instance.setActivationWindow(&w);
    QObject::connect(&instance, SIGNAL(messageReceived(const QString&)),
             &w, SLOT(openAnotherInstanceMessage(const QString&)));

    return instance.exec();
}
