#include "dinool.h"
#include "server.h"
#include <QtWidgets/QApplication>
int main(int argc, char* argv[])
{
	int tmp;
	do {
		QApplication a(argc, argv);
		DinoOL w;
		w.show();
		tmp = a.exec();
		if (tmp == -1)
		{
			w.close();
			Server s;
			s.show();
			return s.exec();
		}
	} while (tmp == -2);
	return tmp;
}
