#include "dinool.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    DinoOL w;
    w.show();
    return a.exec();
}
