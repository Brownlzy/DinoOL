#include "dinool.h"
#include "server.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    DinoOL w;
    w.show();
    int tmp = a.exec();
    if (tmp == -1)
    {
        //w.close();
        Server s;
        s.show();
        return s.exec();
    }
    return tmp;
}
