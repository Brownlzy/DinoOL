﻿/*
* 2021年5月19日17点38分
* By：Brownlzy
*/
#pragma execution_character_set("utf-8")

#include <fstream>

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
	int flag = 0, isRcCfg, isMax, x, y, w, h, isMoon, version;
	if (QFileInfo("DinoOL.cfg").exists())
	{
		std::ifstream fin("DinoOL.cfg");
		if (fin)
		{
			fin >> version >> isRcCfg >> isMax >> x >> y >> w >> h >> isMoon;
			if (version <= DINOVERNUM && version >= 1111)
				flag = 1;
			fin.close();
		}
		else
			flag = -1;
	}
	else
	{
		flag = -1;
	}
	do {
		DinoOL d;
		if (flag == 1)
		{
			d.CfgSet(isRcCfg, x, y, w, h, isMax, isMoon);
		}
		else if (flag == 0)
		{
			d.CfgSet(0);
		}
		d.show();
		tmp = a.exec();
		if (tmp == -1)
		{
			d.close();
			Server s;
			s.setWindowFlags(Qt::WindowMinimizeButtonHint | Qt::WindowCloseButtonHint);
			s.setFixedSize(751, 500);
			s.show();
			return a.exec();
		}
	} while (tmp == -2);
	return tmp;
}
