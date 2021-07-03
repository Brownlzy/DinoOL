/*
* 2021年5月19日17点38分
* By：Brownlzy
*/
#pragma execution_character_set("utf-8")

#include <fstream>

#include "dinool.h"
#include "server.h"
#include "loading.h"
#include <QtWidgets/QApplication>
int main(int argc, char* argv[])
{
	int tmp = 0;
	QApplication a(argc, argv);
	Loading as;
	as.show();
	//as.exec();
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
	as.close();
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
		if (tmp == -3)
		{
			d.close();
			DinoOL d;
			d.setWindowFlag(Qt::FramelessWindowHint); /* 注意：如果单纯开启窗口透明层效果，在Windows系统中必须设置, 其他系统可忽略。*/
			d.setAttribute(Qt::WA_TranslucentBackground);
			//d.setWindowState(Qt::WindowMaximized);
			d.ui.actionTransparent->setChecked(Qt::Checked);
			d.ui.line_7->hide();
			d.ui.labMoon->hide();
			d.ui.labSun->hide();
			d.ui.labSunMoon->hide();
			d.show();
			tmp = a.exec();
		}
	} while (tmp == -2);
	return tmp;
}
