/*
* 2021年6月27日17点45分
* By：Brownlzy
*/

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
			s.setWindowFlags(Qt::WindowMinimizeButtonHint | Qt::WindowCloseButtonHint);
			s.setFixedSize(751, 500);
			s.show();
			return a.exec();
		}
	} while (tmp == -2);
	return tmp;
}
