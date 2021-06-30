/*
* 2021年5月19日17点38分
* By：Brownlzy
*/
#pragma execution_character_set("utf-8")

#include "dinool.h"
#include "server.h"
#include <QtWidgets/QApplication>
int main(int argc, char* argv[])
{
	int tmp = 0;
	QApplication a(argc, argv);
	if (QFileInfo("update.bat").exists())
	{
		QProcess::startDetached("update.bat");
		qApp->exit(-3);
	}
	if (argc == 2 && !strcmp(argv[1], "/DinoOLServer"))
	{
		Server s;
		s.setWindowFlags(Qt::WindowMinimizeButtonHint | Qt::WindowCloseButtonHint);
		s.setFixedSize(751, 500);
		s.show();
		return a.exec();
	}
	do {
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
