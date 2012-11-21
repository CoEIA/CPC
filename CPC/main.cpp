#include <QtGui/QApplication>
#include <QTranslator>

#include <qtsingleapplication.h>
#include "ui\mainwindow.h"
#include "util\application.h"
#include "util\utilities.h"
#include "util\commandlineparser.h"

// CPC Application have 4 different Parameters:
// CPC.exe -Scan
// CPC.exe -Shred_Quick   filepath
// CPC.exe -Shred_Safe    filepath
// CPC.exe -Shred_Through filepath

int main(int argc, char *argv[])
{
    QtSingleApplication instance(argc, argv);

    if (instance.sendMessage(Utilities::buildQStringFromQStringList(instance.arguments())))
        return 0;

//    Application a(argc, argv);
//    a.loadTranslations(":/translations");


    QTranslator translator;
    translator.load(":/translations/" + QLocale::system().name() + ".qm");
    instance.installTranslator(&translator);

    //Application::setLanguage(QLocale::system().name());
    instance.setLayoutDirection(QObject::tr("LTR")=="RTL" ? Qt::RightToLeft : Qt::LeftToRight);

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
