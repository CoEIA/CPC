#include <QtGui/QApplication>
#include "generatorwidget.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    GeneratorWidget w;
    w.show();

    return a.exec();
}
