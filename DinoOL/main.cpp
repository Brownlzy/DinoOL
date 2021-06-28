/*
* 2021年5月19日17点38分
* By：Brownlzy
*/

#include "dinool.h"
#include "server.h"
#include <QtWidgets/QApplication>

int main(int argc, char* argv[])
{
	int tmp;
	if (QFileInfo("update.bat").exists())
	{
		QProcess::startDetached("update.bat");
		qApp->exit(-3);
	}
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
