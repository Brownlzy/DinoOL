/*
* 2021年5月19日17点38分
* By：Brownlzy
*/

#include "dinool.h"

DinoOL::DinoOL(QWidget* parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	pAbout = new About(this);
	pUpdate = new Update(this, pAbout->ui.progressBar, pAbout->ui.label_3, pAbout->ui.textBrowser);
	pAbout->pUpdate = pUpdate;
	buildTime = RELEASETIME;
	QString tmp = "DinoOL ";
#ifdef _DEBUG
	tmp += QString::fromStdString(VERSION) + ".debug By:Brownlzy ====D====E====B====U====G====";
	ui.txtServer->setText("127.0.0.1:30628");
	ui.txtNewRoom->setText("debug");
	ui.txtRoom->setText("debug");
	ui.frame_4->setGeometry(0, 0, ui.frame_4->width(), 341);
#else
	tmp += QString::fromStdString(VERSION) + " By:Brownlzy";
	ui.actionDebug->setVisible(false);
	ui.actionCheat->setVisible(false);
#endif // _DEBUG
	ui.chkCheat->move(-80, 0);
	this->setWindowTitle(tmp);
	this->setWindowIcon(QIcon(":/pic/icon/DinoOL"));
	int x = this->frameGeometry().width();
	int y = this->frameGeometry().height();
	qDebug() << ui.centralWidget->pos();
	ui.labelFail->hide();
	ui.frmLlife->hide();
	ui.frmRlife->hide();
	ui.labMoon->hide();
	ui.labSunMoon->hide();
	ui.labMenu->setText("<html><head/><body><p><a href=""show""><img src="":/pic/png/show""/></a></p></body></html>");
	P1 = new Dino(x, y, this, this->centralWidget());
	P1->isOn = 1;
	P2 = new Dino(x, y, this, this->centralWidget());
	R = new Dino * [2];
	R[0] = new Dino(x, y, this, this->centralWidget());
	R[1] = new Dino(x, y, this, this->centralWidget());
	P1->setDinoState(":/pic/gif/dino_start");
	P1->move(0.2 * x, 0.2 * y);
	P1->adjustSize();
	P1->show(true);
	ui.frmScore->hide();
	ui.label_2->hide();
	ui.label_3->adjustSize();
	ui.labelchklcs->hide();
	ui.labReady->setText("<html><head/><body><p><img src="":/pic/png/6"" widyh=""162"" height=""198""/></p></body></html>");
	ui.labReady->adjustSize();
	ui.labReady->hide();
	//ui.whitebg->hide();
	ui.frame_4->hide();
	ui.lab_2->move(x - 1 - ui.lab_2->width(), ui.lab_2->y());
	ui.lab_3->move(x - 1 - ui.lab_3->width(), ui.lab_3->y());
	ui.lab_6->move(x - 1 - ui.lab_6->width(), ui.lab_6->y());
	maxH = vy0 * vy0 / (2 * G);
	pdtime = new QTimer(this);
	ptcloud = new QTimer(this);
	ptOBS = new QTimer(this);
	ptOBS->setInterval(tms);
	pdtime->setInterval(100);
	connect(ui.label_2, SIGNAL(linkActivated(QString)), this, SLOT(NetworkChk(QString)));
	connect(ui.label_3, SIGNAL(linkActivated(QString)), this, SLOT(NetworkChk(QString)));
	connect(ui.labMenu, SIGNAL(linkActivated(QString)), this, SLOT(Menu(QString)));
	connect(ui.labSun, SIGNAL(linkActivated(QString)), this, SLOT(ChangeColor(QString)));
	connect(ui.labMoon, SIGNAL(linkActivated(QString)), this, SLOT(ChangeColor(QString)));
	connect(ui.labelFail, SIGNAL(linkActivated(QString)), this, SLOT(reStart(QString)));
	connect(ptcloud, SIGNAL(timeout()), this, SLOT(cloudloop()));
	connect(pdtime, SIGNAL(timeout()), this, SLOT(printpos()));
	connect(ptOBS, SIGNAL(timeout()), this, SLOT(printOBS()));
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			ui.tableRoomer->setItem(i, j, new QTableWidgetItem(""));
		}
	}
	readDataFile();
	//pdtime->start();
	cursorCurrentPointF = QPoint(0, 0);
	cursouPresss = actionActive = false;
	lab = new MoveLabel(this->centralWidget());
	lab->setText("<html><head/><body><p><img src="":/pic/png/Drag"" width=""40"" height=""40""/></p></body></html>");
	lab->show();
	connect(lab, SIGNAL(ActionCurrentPos(const bool&, const QPoint&)),
		this, SLOT(ActionCurrentPos(const bool&, const QPoint&)));

	connect(ui.actionServer, SIGNAL(triggered()), this, SLOT(toServer()));
	connect(ui.actionExit, SIGNAL(triggered()), this, SLOT(ExitApp()));
	connect(ui.actionConnect, SIGNAL(triggered()), this, SLOT(ConnectServer()));
	connect(ui.actionDebug, SIGNAL(triggered()), this, SLOT(DebugInfo()));
	connect(ui.actionNewOne, SIGNAL(triggered()), this, SLOT(NewOne()));
	connect(ui.actionRestart, SIGNAL(triggered()), this, SLOT(RestartApp()));
	connect(ui.actionAbout, SIGNAL(triggered()), this, SLOT(ShowAbout()));
	connect(ui.actionCheat, SIGNAL(triggered()), this, SLOT(Cheat()));
	connect(ui.actionOffline, SIGNAL(triggered()), this, SLOT(Offline()));
	connect(ui.actionPay, SIGNAL(triggered()), this, SLOT(PayQR()));
	connect(ui.actionHelp, SIGNAL(triggered()), this, SLOT(Help()));
	connect(ui.actionTransparent, SIGNAL(triggered()), this, SLOT(aTransparent()));
	connect(ui.btnCon, SIGNAL(clicked()), this, SLOT(btnCon()));
	connect(ui.btnWhatsThis, SIGNAL(clicked()), this, SLOT(btnWhatsThis()));
	connect(ui.btnRandRoom, SIGNAL(clicked()), this, SLOT(btnRandRoom()));
	connect(ui.btnCreRoom, SIGNAL(clicked()), this, SLOT(btnCreRoom()));
	connect(ui.btnExitRoom, SIGNAL(clicked()), this, SLOT(btnExitRoom()));
	connect(ui.btnSend, SIGNAL(clicked()), this, SLOT(btnSend()));
	connect(ui.btnJoin, SIGNAL(clicked()), this, SLOT(btnJoin()));
	connect(ui.btnHD, SIGNAL(clicked()), this, SLOT(btnHD()));
	connect(ui.chkCheat, SIGNAL(clicked()), this, SLOT(chkCheat()));
}

DinoOL::~DinoOL()
{
	if (labCloud != NULL)
	{
		delete[] labCloud;
	}
	if (movie_cloud != NULL)
	{
		delete[] movie_cloud;
	}
	if (pAnimationCloud != NULL)
	{
		delete[] pAnimationCloud;
	}
	delete[] P1;
	delete[] P2;
	delete[] R[0];
	delete[] R[1];
	delete[] R;
}
void DinoOL::printpos()
{
	for (int i = 0; i < 7; i++)
	{
		if (OBS[i] != NULL)OBS[i]->setFrameShape(QFrame::Box);
	}

	QString tmp;
	tmp = "P1:(" + QString::number(P1->x()) + "," + QString::number(P1->y()) + ")";
	tmp = tmp + ",isDive:" + QString::number(P1->isDive) + ",isJump:" + QString::number(P1->isJump) + ",isMove:" + QString::number(P1->isMove);
	tmp += ",vx=" + QString::number(P1->vx) + ",vy=" + QString::number(P1->vy) + ",onG=" + QString::number(P1->isOnGround());
	ui.lab->setText(tmp);
	ui.lab->adjustSize();

	tmp = "R[0]:(" + QString::number(R[0]->x()) + "," + QString::number(R[0]->y()) + ")";
	tmp = tmp + ",isDive:" + QString::number(R[0]->isDive) + ",isJump:" + QString::number(R[0]->isJump) + ",isMove:" + QString::number(R[0]->isMove);
	tmp += ",vx=" + QString::number(R[0]->vx) + ",vy=" + QString::number(R[0]->vy) + ",onG=" + QString::number(R[0]->isOnGround());
	ui.lab_2->setText(tmp);
	ui.lab_2->adjustSize();

	tmp = "R[1]:(" + QString::number(R[1]->x()) + "," + QString::number(R[1]->y()) + ")";
	tmp = tmp + ",isDive:" + QString::number(R[1]->isDive) + ",isJump:" + QString::number(R[1]->isJump) + ",isMove:" + QString::number(R[1]->isMove);
	tmp += ",vx=" + QString::number(R[1]->vx) + ",vy=" + QString::number(R[1]->vy) + ",onG=" + QString::number(R[1]->isOnGround());
	ui.lab_3->setText(tmp);
	ui.lab_3->adjustSize();

	tmp = "P2:(" + QString::number(P2->x()) + "," + QString::number(P2->y()) + ")";
	tmp = tmp + ",isDive:" + QString::number(P2->isDive) + ",isJump:" + QString::number(P2->isJump) + ",isMove:" + QString::number(P2->isMove);
	tmp += ",vx=" + QString::number(P2->vx) + ",vy=" + QString::number(P2->vy) + ",onG=" + QString::number(P2->isOnGround());
	ui.lab_4->setText(tmp);
	ui.lab_4->adjustSize();

	tmp = VERSION + QString::fromUtf8(" BT:") + buildTime;
	ui.lab_5->setText(tmp);
	ui.lab_5->adjustSize();

	tmp = "OBS[0-6] MaxH:" + QString::number(maxH);
	for (int i = 0; i < 7; i++)
	{
		tmp += "|" + QString::number(i) + ":" + QString::number(vOBS[i]) + " " + QString::number(dy[i]) + " ";
		if (OBS[i] != NULL) tmp += "(" + QString::number(OBS[i]->x()) + "," + QString::number(OBS[i]->y()) + ") ";
		else tmp += "(-,-)";
	}
	ui.lab_6->setText(tmp);
	ui.lab_6->adjustSize();

	ui.lab_2->move(this->frameGeometry().width() - 1 - ui.lab_2->width(), ui.lab_2->y());
	ui.lab_3->move(this->frameGeometry().width() - 1 - ui.lab_3->width(), ui.lab_3->y());
	ui.lab_6->move(this->frameGeometry().width() - 1 - ui.lab_6->width(), ui.lab_6->y());
}
void DinoOL::resizeEvent(QResizeEvent* event)
{
	resizeDinoOL();
}
void DinoOL::closeEvent(QCloseEvent* event)
{
	if (ui.menuSPID->title() != "SPID") btnExitRoom();
	if (ui.actionCleanData->isChecked())
	{
		ui.actionMaxScore->setText("0");
		ui.actionJump->setText("0");
		ui.actionDive->setText("0");
		ui.actionStar->setText("0");
		ui.actionRunJump->setText("0");
		writeDataFile();
	}
	std::ofstream fout(qApp->applicationDirPath().toStdString() + "DinoOL.cfg");
	if (!fout) return;
	fout << VERSIONID << " " << ui.action_3->isChecked() << " " << this->windowState() << " ";
	fout << this->geometry().x() << " " << this->geometry().y() << " " << this->geometry().width() << " " << this->geometry().height() << " ";
	fout << !isSun << "\n" << "This_is_DinoOL_config_file.";
	fout.close();
}
void DinoOL::resizeDinoOL()
{
	//获取ui形成窗口宽度
	int x = this->frameGeometry().width();
	int y = this->frameGeometry().height();
	horline = 0.6 * y;
	P1->setfxy(x, y);
	if (ui.actionTransparent->isChecked())		//透明模式开启
	{
		lab->move(x - 60, y - 70);
		ui.labMenu->move(x - 302, y - 70);
	}
	else                                         //透明模式未开启
	{
		lab->move(x - 60, y - 120);
		ui.labMenu->move(x - 302, y - 120);
	}
	ui.line_7->move(x - 43, 0);
	ui.labMoon->move(x - 58, 50);
	ui.labSunMoon->move(x - 58, 50);
	ui.labSun->move(x - 58, 50);
	ui.labReady->move((x - ui.labReady->width()) * 0.5, (horline - ui.labReady->height()) * 0.5);
	if (P2 != NULL)P2->setfxy(x, y);
	if (R[0] != NULL)R[0]->setfxy(x, y);
	if (R[1] != NULL)R[1]->setfxy(x, y);
	qDebug() << x << "," << y;
	if (!isStarted)
	{
		P1->move(0.2 * x, 0.2 * y);
		P1->labDino.pos().setX(0.2 * x);
		ui.labRoad->move(0.2 * x, 0.2 * y + 34);
	}
	else if (isStarted == 1)
	{
		P1->move(0.2 * x, 0.2 * y - 81);
		ui.labRoad->move(0.2 * x, 0.2 * y - 43);
	}
	else if (isStarted == 2)
	{
		P1->move(0.2 * x, 0.2 * y);
		ui.labRoad->move(0.2 * x, 0.2 * y + 34);
	}
	else if (isStarted >= 3)
	{
		if (!WebGame && !isPause)
		{
			P1->setGeometry(P1->x() > x - P1->width() ? x - P1->width() : P1->x(), horline - P1->height(), P1->width(), P1->height());
			if (P2 != NULL) P2->setGeometry(P2->x() > x - P2->width() ? x - P2->width() : P2->x(), horline - P2->height(), P2->width(), P2->height());
			if (R[0] != NULL) R[0]->setGeometry(R[0]->x() > x - R[0]->width() ? x - R[0]->width() : R[0]->x(), horline - R[0]->height(), R[0]->width(), R[0]->height());
			if (R[1] != NULL) R[1]->setGeometry(R[1]->x() > x - R[1]->width() ? x - R[1]->width() : R[1]->x(), horline - R[1]->height(), R[1]->width(), R[1]->height());
		}
		else
		{
			P1->setGeometry(P1->x(), horline - P1->height(), P1->width(), P1->height());
			if (P2 != NULL) P2->setGeometry(P2->x(), horline - P2->height(), P2->width(), P2->height());
			if (R[0] != NULL) R[0]->setGeometry(R[0]->x(), horline - R[0]->height(), R[0]->width(), R[0]->height());
			if (R[1] != NULL) R[1]->setGeometry(R[1]->x(), horline - R[1]->height(), R[1]->width(), R[1]->height());

		}
		ui.labRoad->move(ui.labRoad->x(), horline - 24);
	}
	ui.frame_4->move((x - ui.frame_4->width()) / 2, ui.frame_4->y());
	ui.label_2->move(P1->x(), P1->y() + 70);
	ui.label_3->move(P1->x(), P1->y() + 70);
	//ui.whitebg->setGeometry(0, 0, x, y);
	ui.lab_2->move(x - 1 - ui.lab_2->width(), ui.lab_2->y());
	ui.lab_3->move(x - 1 - ui.lab_3->width(), ui.lab_3->y());
	ui.lab_6->move(x - 1 - ui.lab_6->width(), ui.lab_6->y());
	ui.labelFail->move((x - ui.labelFail->width()) / 2, (horline - ui.labelFail->height()) / 2);
	ui.frmScore->move(x - ui.frmScore->width() - 1, 0);
	//ui.frmScore->show();
	qDebug() << ui.frmScore->pos() << ui.frmScore->height();
	roadloop();
}

void DinoOL::ChangeColor(QString color)
{
	if (color == "TurnBlack")
	{
		SunMoon(1);
		ui.labSunMoon->show();
		ui.labSun->hide();
	}
	else if (color == "TurnWhite")
	{
		SunMoon(0);
		ui.labMoon->hide();
		ui.labSunMoon->show();
	}
}

void DinoOL::SunMoon(int istoMoon)
{
	if (istoMoon == -1)
	{
		if (isSun == 1)
		{
			ui.labSunMoon->hide();
			ui.labSun->show();
			ui.labSunMoon->clear();
		}
		else
		{
			ui.labSunMoon->hide();
			ui.labMoon->show();
			ui.labSunMoon->clear();
			QColor c = QColor("#202020");//黑
			QPalette p = this->palette();
			p.setColor(QPalette::Window, c);
			this->setPalette(p);
			ui.labelchklcs->setStyleSheet("font: 9pt ""SimSun"";color:#707070;");
			ui.label_2->setStyleSheet("font: 9pt ""黑体"";color:#707070;");
			ui.label_3->setStyleSheet("font: 9pt ""黑体"";color:#707070;");
		}
	}
	else if (istoMoon == 0)
	{
		isSun = 1;
		S2M.stop();
		S2M.setFileName(":pic/gif/M2S");
		ui.labSunMoon->setMovie(&S2M);
		S2M.start();
		QTimer::singleShot(500, this, SLOT(SunMoon()));
		QColor c = QColor("#F0F0F0");//灰
		QPalette p = this->palette();
		p.setColor(QPalette::Window, c);
		this->setPalette(p);
		ui.labelchklcs->setStyleSheet("font: 9pt ""SimSun"";");
		ui.label_2->setStyleSheet("font: 9pt ""黑体"";");
		ui.label_3->setStyleSheet("font: 9pt ""黑体"";");
	}
	else
	{
		isSun = 0;
		S2M.stop();
		S2M.setFileName(":pic/gif/S2M");
		ui.labSunMoon->setMovie(&S2M);
		S2M.start();
		QTimer::singleShot(500, this, SLOT(SunMoon()));
	}
}

void DinoOL::paintEvent(QPaintEvent*)
{
	//QPainter painter(this);
	/* 0x20为透明层颜色，可自定义设置为0x0到0xff */
	//painter.fillRect(this->rect(), QColor(0, 0, 0, 0x20));
}

void DinoOL::ActionCurrentPos(const bool& b, const QPoint& point)
{
	if (b)
	{
		this->move(this->pos() + (this->cursor().pos() - cursorCurrentPointF));
		cursorCurrentPointF = this->cursor().pos();
	}
	else
	{
		cursorCurrentPointF = point;
	}
	update();
}

void DinoOL::NetworkChk(QString str)
{
	QProcess process(this);
	process.setProgram("C:\\Windows\\System32\\msdt.exe");
	QStringList argu;
	argu << "-skip" << "TRUE" << "-path" << "C:\\Windows\\diagnostics\\system\\networking" << "-ep" << "NetworkDiagnosticsPNI";
	process.setArguments(argu);
	process.startDetached();
}

void DinoOL::Menu(QString s)
{
	if (s == "menu")
	{
		QMenu* popMenu = new QMenu(this);//定义一个右键弹出菜单
		popMenu->addMenu(ui.menuFile);
		popMenu->addMenu(ui.menuGame);
		popMenu->addMenu(ui.menu_S);
		popMenu->addMenu(ui.menuHelp);
		popMenu->exec(QCursor::pos());//弹出右键菜单，菜单位置为光标位置
	}
	else if (s == "close")
	{
		this->close();
	}
	else if (s == "min")
	{
		this->setWindowState(Qt::WindowMinimized);
	}
	else if (s == "max")
	{
		this->setWindowState(Qt::WindowMaximized);
		ui.labMenu->setText("<html><head/><body><p><a href=""close""><img src="":/pic/png/close""/></a><a href=""toside""><img src="":/pic/png/toSide""/></a><a href=""min""><img src="":/pic/png/Min""/></a><a href=""normal""><img src="":/pic/png/Normal""/></a><a href=""menu""><img src="":/pic/png/Menu""/></a><a href=""hide""><img src="":/pic/png/hide""/></a></p></body></html>");
	}
	else if (s == "normal")
	{
		this->setWindowState(Qt::WindowNoState);
		ui.labMenu->setText("<html><head/><body><p><a href=""close""><img src="":/pic/png/close""/></a><a href=""toside""><img src="":/pic/png/toSide""/></a><a href=""min""><img src="":/pic/png/Min""/></a><a href=""max""><img src="":/pic/png/Max""/></a><a href=""menu""><img src="":/pic/png/Menu""/></a><a href=""hide""><img src="":/pic/png/hide""/></a></p></body></html>");
	}
	else if (s == "hide")
	{
		ui.labMenu->setText("<html><head/><body><p><a href=""show""><img src="":/pic/png/show""/></a></p></body></html>");
	}
	else if (s == "show")
	{
		if (this->windowState() == Qt::MaximumSize)
			ui.labMenu->setText("<html><head/><body><p><a href=""close""><img src="":/pic/png/close""/></a><a href=""toside""><img src="":/pic/png/toSide""/></a><a href=""min""><img src="":/pic/png/Min""/></a><a href=""normal""><img src="":/pic/png/Normal""/></a><a href=""menu""><img src="":/pic/png/Menu""/></a><a href=""hide""><img src="":/pic/png/hide""/></a></p></body></html>");
		else
			ui.labMenu->setText("<html><head/><body><p><a href=""close""><img src="":/pic/png/close""/></a><a href=""toside""><img src="":/pic/png/toSide""/></a><a href=""min""><img src="":/pic/png/Min""/></a><a href=""max""><img src="":/pic/png/Max""/></a><a href=""menu""><img src="":/pic/png/Menu""/></a><a href=""hide""><img src="":/pic/png/hide""/></a></p></body></html>");
	}
	else if (s == "toside")
	{

	}

}

void DinoOL::keyPressEvent(QKeyEvent* e)
{
	if (isPause) return;
	if (isStarted == 0 && (e->key() == Qt::Key_Space || e->key() == Qt::Key_W || e->key() == Qt::Key_Up))
	{
		Loading l;
		l.show();
		isStarted = -1;
		if (e->key() == Qt::Key_W || e->key() == Qt::Key_Space)//若Up先按下则方向键给P1
			isWforP1 = 1;
		else
			isWforP1 = 0;
		ui.labelchklcs->show();
#ifndef _DEBUG
		if (!ui.actionOffline->isChecked())
		{
			int exec;
			if (pUpdate->isLatest)
				exec = 0;
			else
				exec = pUpdate->CheckUpdate();
			if (exec > 0)
			{
				l.close();
				pAbout->show();
				if (pAbout->ui.btnShowUpdateInfo->text() == "U") pAbout->ShowUpdateInfo();
				//pAbout->CheckUpdate();
				ui.labelchklcs->hide();
				isStarted = 0;
				return;
			}
		}
#endif // !_DEBUG
		ui.labelchklcs->hide();
		l.close();
		StartGame();
		return;
	}
	if (isStarted < 4) return;
	if (((e->key() == Qt::Key_Up && isWforP1) || (e->key() == Qt::Key_W && !isWforP1)) && !WebGame)
	{
		if (!P2->isOn && OBS[0] == NULL)
		{
			P2->setGeometry(0, -98, 88, 98);
			P2->setScaledContents(true);
			P2->changeP("P2");
			P2->show(1, 1);
			P2->isOn = 1;
			P2->start();
		}
		else if (P2->isOnGround())
		{
			P2->start();
			P2->vy += P2->vy0;
			P2->isJump = 1;
		}
	}
	bool onG = P1->isOnGround();
	int keyID = 83;
	if (!isWforP1 && e->key() == Qt::Key_Left) keyID = Qt::Key_A;
	if (!isWforP1 && e->key() == Qt::Key_Right) keyID = Qt::Key_D;
	if (!isWforP1 && e->key() == Qt::Key_Down) keyID = Qt::Key_S;
	if (!isWforP1 && e->key() == Qt::Key_Up) keyID = Qt::Key_W;

	if ((e->key() == Qt::Key_W && isWforP1 || e->key() == Qt::Key_Up && !isWforP1) && onG)
	{
		P1->start();
		P1->vy += P1->vy0;
		P1->isJump = 1;
		if (WebGame) SendPOS(P1->x(), horline - P1->y(), keyID, 1);
		if (P1->vx == 0)
		{
			ui.actionJump->setText(QString::number(ui.actionJump->text().toInt() + 1));
		}
		else
		{
			ui.actionRunJump->setText(QString::number(ui.actionRunJump->text().toInt() + 1));
		}
	}
	if (!e->isAutoRepeat())
	{
		if (WebGame) SendPOS(P1->x(), horline - P1->y(), keyID, 1);
		if (isWforP1 && (e->key() == Qt::Key_A || e->key() == Qt::Key_D || e->key() == Qt::Key_S) || !isWforP1 && (e->key() == Qt::Key_Left || e->key() == Qt::Key_Right || e->key() == Qt::Key_Down))
		{
			P1->keyPR(e->key(), 1);
			if (isWforP1 && e->key() == Qt::Key_S || !isWforP1 && e->key() == Qt::Key_Down)
			{
				ui.actionDive->setText(QString::number(ui.actionDive->text().toInt() + 1));
			}
		}
		if (P2->isOn && (!isWforP1 && (e->key() == Qt::Key_A || e->key() == Qt::Key_D || e->key() == Qt::Key_S) || isWforP1 && (e->key() == Qt::Key_Left || e->key() == Qt::Key_Right || e->key() == Qt::Key_Down))) P2->keyPR(e->key(), 1);
	}
}

void DinoOL::keyReleaseEvent(QKeyEvent* e)
{
	if (isPause) return;
	if (isStarted < 4) return;
	if (!e->isAutoRepeat())
	{
		int keyID = 83;
		if (!isWforP1 && e->key() == Qt::Key_Left) keyID = Qt::Key_A;
		if (!isWforP1 && e->key() == Qt::Key_Right) keyID = Qt::Key_D;
		if (!isWforP1 && e->key() == Qt::Key_Down) keyID = Qt::Key_S;
		if (WebGame) SendPOS(P1->x(), horline - P1->y(), keyID, 0);
		if (e->key() == Qt::Key_A || e->key() == Qt::Key_D || e->key() == Qt::Key_S)
			if (isWforP1)
				P1->keyPR(e->key(), 0);
			else if (P2->isOn)
				P2->keyPR(e->key(), 0);
		if (e->key() == Qt::Key_Left || e->key() == Qt::Key_Right || e->key() == Qt::Key_Down)
			if (P2->isOn && isWforP1)
				P2->keyPR(e->key(), 0);
			else
				P1->keyPR(e->key(), 0);
	}

}
void DinoOL::CleanAM(QPropertyAnimation* pitem)
{
	if (pitem != NULL)
	{
		pitem->stop();
	}
}
void DinoOL::CleanAM(QMovie* pitem)
{
	if (pitem != NULL)
	{
		pitem->stop();
	}
}

void DinoOL::StartGame(int step)
{
	int x = this->frameGeometry().width();
	int y = this->frameGeometry().height();
	switch (step)
	{
	case(0):
		P1->setDinoState(":/pic/gif/dino_jump");
		P1->setGeometry(0.2 * x, 0.2 * y - 83, 44, 130);
		P1->setScaledContents(true);
		QTimer::singleShot(900, this, SLOT(StartStep1()));
		isStarted = 1;
		break;
	case(1):
		P1->setDinoState(":/pic/gif/dino_run");
		P1->setGeometry(0.2 * x, 0.2 * y, 44, 49);
		P1->setScaledContents(true);
		CleanAM(pAnimation1);
		pAnimation1 = new QPropertyAnimation(ui.labRoad, "geometry");
		pAnimation1->setDuration(700);
		pAnimation1->setStartValue(QRect(ui.labRoad->x(), ui.labRoad->y(), ui.labRoad->width(), ui.labRoad->height()));
		pAnimation1->setEndValue(QRect(ui.labRoad->x(), ui.labRoad->y(), 0.6 * x, ui.labRoad->height()));
		pAnimation1->start();
		QTimer::singleShot(700, this, SLOT(StartStep2()));
		isStarted = 2;
		break;
	case(2):
		ui.label_2->hide();
		ui.label_3->hide();
		CleanAM(movie_road);
		movie_road = new QMovie(":/pic/png/road");
		ui.labRoad->setMovie(movie_road);
		movie_road->start();
		CleanAM(pAnimation1);
		pAnimation1 = new QPropertyAnimation(ui.labRoad, "geometry");
		pAnimation1->setDuration(700);
		pAnimation1->setStartValue(QRect(ui.labRoad->x(), ui.labRoad->y(), ui.labRoad->width(), ui.labRoad->height()));
		pAnimation1->setEndValue(QRect(0, 0.6 * y - 2.0 * P1->height() + 73, 6000, 24));
		P1->setfxy(x, y);
		P1->startAnima();
		pAnimation1->start();
		ui.labRoad->setScaledContents(true);
		R[0]->setGeometry(0, 0.6 * y - 98, 88, 98);
		R[1]->setGeometry(0, 0.6 * y - 98, 88, 98);
		R[0]->setScaledContents(true);
		R[1]->setScaledContents(true);
		isStarted = 3;
		QTimer::singleShot(700, this, SLOT(SendReady()));
		QTimer::singleShot(700, this, SLOT(roadloop()));
		QTimer::singleShot(700, this, SLOT(cloudloop()));
		QTimer::singleShot(700, P1, SLOT(showP()));
		QTimer::singleShot(700, this, SLOT(ProduceOBS()));
		break;
	}
}


void DinoOL::setOBSPic(QString pic, int id)
{
	if (mOBS[id] == NULL) mOBS[id] = new QMovie(this);
	mOBS[id]->stop();
	mOBS[id]->setFileName(pic);
	OBS[id]->clear();
	OBS[id]->setMovie(mOBS[id]);
	OBS[id]->adjustSize();
	mOBS[id]->start();
}

void DinoOL::ProcessSMsg(QString msg)
{
	QString fun = msg.split('$')[0];
	if (fun == "POS")
	{//POS$PID$dx#dy#key#1$$$
		QString strRKey = msg.split('$')[2];
		if (msg.split('$')[1].toInt() == R[0]->getP())
		{
			R[0]->move(strRKey.split('#')[0].toDouble(), horline - strRKey.split('#')[1].toDouble());
			RKey(0, strRKey.split('#')[2].toInt(), strRKey.split('#')[3].toInt());
		}
		else if (msg.split('$')[1].toInt() == R[1]->getP())
		{
			R[1]->move(strRKey.split('#')[0].toDouble(), horline - strRKey.split('#')[1].toDouble());
			RKey(1, strRKey.split('#')[2].toInt(), strRKey.split('#')[3].toInt());
		}
		else if (msg.split('$')[1].toInt() == SPID)
		{
			QString title = "Delay:" + QString::number(t.elapsed()) + "ms";
			ui.menuDelay->setTitle(title);
		}
	}
	else if (fun == "OBS")
	{
		if (isStarted < 4) return;
		QString obsData = msg.split('$')[2];
		ProduceOBS(obsData.split('#')[0].toInt(), obsData.split('#')[1].toInt());
		if (msg.split('$')[1].toInt() == SPID)
		{
			QString title = "Delay:" + QString::number(tobs.elapsed()) + "ms";
			ui.menuDelay->setTitle(title);
		}
	}
	else if (fun == "READY")
	{
		if (msg.split('$')[1] == "2")
		{
			for (int i = 0; i < 3; i++)
			{
				if (ui.tableRoomer->item(i, 1)->text() != "")
					ui.tableRoomer->item(i, 3)->setText(msg.split('$')[2].split('-')[i]);
			}
		}
		if (ui.tableRoomer->item(0, 3)->text().split('-')[0] == tr("是")
			&& (!(ui.tableRoomer->item(1, 1)->text() != "") || ui.tableRoomer->item(1, 3)->text() == tr("是"))
			&& (!(ui.tableRoomer->item(2, 1)->text() != "") || ui.tableRoomer->item(2, 3)->text() == tr("是")))
		{
			for (int i = 0, j = 0; i < 3; i++)
			{
				if (ui.tableRoomer->item(i, 1)->text() != ui.menuSPID->title().split('=')[1] && ui.tableRoomer->item(i, 3)->text() != "")
				{
					R[j]->changeP(ui.tableRoomer->item(i, 1)->text());
					R[j]->labP.setVisible(false);
					R[j]->isOn = 1;
					R[j]->setDinoState(":/pic/gif/dino_run");
					j++;
					continue;
				}
			}
			WebGame = 1;
			P1->WebGame = 1;
			R[0]->WebGame = 1;
			R[1]->WebGame = 1;
			if (isAllReady()) ProduceOBS();
			//if (ui.tableRoomer->item(0, 1)->text().toInt() == SPID)
				//QTimer::singleShot(3000, this, SLOT(ProduceOBS()));
		}
	}
	else if (fun == "ARGU")
	{
		P1->G = msg.split('$')[1].toDouble();
		P1->vx0 = msg.split('$')[2].toDouble();
		P1->vy0 = msg.split('$')[3].toDouble();
		R[0]->G = msg.split('$')[1].toDouble();
		R[0]->vx0 = msg.split('$')[2].toDouble();
		R[0]->vy0 = msg.split('$')[3].toDouble();
		R[1]->G = msg.split('$')[1].toDouble();
		R[1]->vx0 = msg.split('$')[2].toDouble();
		R[1]->vy0 = msg.split('$')[3].toDouble();
		maxH = vy0 * vy0 / (2 * G);
	}
	else if (fun == "PID")
	{
		int pid = msg.split('$')[1].toInt();
		if (pid == 100)
		{
			ui.labelLog->setText(tr("游戏服务器已满,请稍后再试!"));
			mp_clientSocket->disconnect();
			ui.txtGet->setPlainText("PID=100 x disconnected");
		}
		else
		{
			ui.labelLog->setText(tr("已连接至游戏服务器"));
			SPID = pid;
			QString tmp = "SPID=" + QString::number(SPID);
			ui.menuSPID->setTitle(tmp);
			ui.label_3->move(ui.label_2->x(), ui.label_2->y());
			ui.label_2->setVisible(false);
			ui.label_3->setVisible(true);
			WebGame = 1;
		}
	}
	else if (fun == "ROOM")
	{
		int state = msg.split('$')[1].toInt();
		ui.labelLog->setText(msg.split('$')[3]);
		switch (state)
		{
		case 0:
			//mp_clientSocket->disconnectFromHost();
			if (isStarted)
			{
				GamePause();
				ui.labelLog->setText(tr("收到服务器通知，由于房间关闭，已停止游戏！"));
				ui.frame_4->raise();
				ui.frame_4->show();
				ui.frame_4->raise();
			}
			break;
		case 1:
			ui.menuROOM->setTitle("ROOM=" + msg.split('$')[2]);
			ui.tableRoomer->item(0, 0)->setText(tr("是"));
			ui.tableRoomer->item(0, 1)->setText(QString::number(SPID));
			ui.tableRoomer->item(0, 2)->setText(tr("Grey"));
			ui.tableRoomer->item(0, 3)->setText(tr("否"));
			break;
		default:
			break;
		}
	}
	else if (fun == "JOIN")
	{
		int state = msg.split('$')[1].toInt();
		ui.labelLog->setText(msg.split('$')[2]);
		if (state == 2)
		{
			QString ExistMem = msg.split('$')[3];
			for (int i = 0; i < 3; i++)
			{
				for (int j = 0; j < 3; j++)
				{
					ui.tableRoomer->item(i, j)->setText(ExistMem.split('@')[i].split('#')[j]);
				}
				if (ui.tableRoomer->item(i, 1)->text() != "")
					ui.tableRoomer->item(i, 3)->setText(msg.split('$')[4].split('-')[i]);
				else
					ui.tableRoomer->item(i, 3)->setText("");
			}
			ui.menuROOM->setTitle("ROOM=" + msg.split('$')[2]);
		}
	}
	else if (fun == "ADDU")
	{
		int state = msg.split('$')[1].toInt();
		if (state == 1)
		{
			for (int i = 0; i < 3; i++)
			{
				if (ui.tableRoomer->item(i, 1)->text() == "")
				{
					ui.tableRoomer->item(i, 1)->setText(msg.split('$')[2]);
					ui.tableRoomer->item(i, 2)->setText(msg.split('$')[3]);
					ui.tableRoomer->item(i, 3)->setText(tr("否"));
					break;
				}
			}
		}
		else if (state == -1)
		{
			for (int i = 0; i < 3; i++)
			{
				if (ui.tableRoomer->item(i, 1)->text() == msg.split('$')[2])
				{
					if (i == 1 && ui.tableRoomer->item(i + 1, 1)->text() != "")
					{
						ui.tableRoomer->item(i, 1)->setText(ui.tableRoomer->item(2, 1)->text());
						ui.tableRoomer->item(i, 2)->setText(ui.tableRoomer->item(2, 2)->text());
						ui.tableRoomer->item(i, 3)->setText(ui.tableRoomer->item(2, 3)->text());
					}
					else
					{
						ui.tableRoomer->item(i, 1)->setText("");
						ui.tableRoomer->item(i, 2)->setText("");
						ui.tableRoomer->item(i, 3)->setText("");
					}
					break;
				}
			}
		}
	}
	else if (fun == "CGLIFE")
	{
		//CGLIFE$ROOMID$PID$LifeNum$$
		if (msg.split('$')[2].toInt() == SPID) return;
		for (int i = 0; i < 2; i++)
		{
			if (R[i]->getP() == msg.split('$')[1].toInt())
			{
				if (i == 0)
				{
					ui.lifeR1R->setText("<html><head/><body><p><img src = "":/pic/png/" + msg.split('$')[2] + """ width = ""36"" height = ""44""/></p></body></html>");
					if (msg.split('$')[2] == "0")
					{
						if (R[1]->isOn == 1 && (R[1 - i]->isFail == 1 || P1->isFail == 1) || R[1]->isOn == 0)
						{
							GamePause();
							isPause = 1;
							R[i]->stop();
							R[i]->setDinoState(":/pic/png/dino_fail");
						}
						else
						{
							R[i]->isFail = 1;
							R[i]->setDinoState("");
						}
					}
					else { R[i]->shining(); }
				}
				else if (i == 1)
				{
					ui.lifeR2R->setText("<html><head/><body><p><img src = "":/pic/png/" + msg.split('$')[2] + """ width = ""36"" height = ""44""/></p></body></html>");
					if (msg.split('$')[2] == "0")
					{
						if (R[1]->isOn == 1 && (R[1 - i]->isFail == 1 || P1->isFail == 1) || R[1]->isOn == 0)
						{
							GamePause();
							isPause = 1;
							R[i]->stop();
							R[i]->setDinoState(":/pic/png/dino_fail");
						}
						else
						{
							R[i]->isFail = 1;
							R[i]->setDinoState("");
						}
					}
					else { R[i]->shining(); }
				}
			}
		}
	}
}

void DinoOL::SendPOS(int dx, int dy, int key, bool isPress)
{
	QString sendMsg = QString::number(SPID) + "$POS$" + ui.menuROOM->title().split('=')[1] + "$"
		+ QString::number(dx) + "#" + QString::number(dy) + "#" + QString::number(key) + "#" + QString::number(isPress) + "$$$";
	char sendMsgChar[1024] = { 0 };
	strcpy_s(sendMsgChar, sendMsg.toStdString().c_str());
	int sendRe = mp_clientSocket->write(sendMsgChar, strlen(sendMsgChar));
	t.start();
	if (sendRe == -1)
	{
		ui.labelLog->setText(tr("向服务端发送数据失败！"));
		return;
	}

}

void DinoOL::SendObstacle(int kind, int dy)
{
	QString sendMsg = QString::number(SPID) + "$OBS$" + ui.menuROOM->title().split('=')[1] + "$"
		+ QString::number(kind) + "#" + QString::number(dy) + "#$$$";
	char sendMsgChar[1024] = { 0 };
	strcpy_s(sendMsgChar, sendMsg.toStdString().c_str());
	int sendRe = mp_clientSocket->write(sendMsgChar, strlen(sendMsgChar));
	tobs.start();
	if (sendRe == -1)
	{
		ui.labelLog->setText(tr("向服务端发送数据失败！"));
		return;
	}

}

void DinoOL::SendReady()
{
	if (ui.menuSPID->title() == "SPID" || ui.menuROOM->title() == "ROOM") return;
	QString sendMsg = QString::number(SPID) + "$READY$" + ui.menuROOM->title().split('=')[1] + "$$$$";
	char sendMsgChar[1024] = { 0 };
	strcpy_s(sendMsgChar, sendMsg.toStdString().c_str());
	int sendRe = mp_clientSocket->write(sendMsgChar, strlen(sendMsgChar));
	if (sendRe == -1)
	{
		ui.labelLog->setText(tr("向服务端发送数据失败！"));
		return;
	}

}
void DinoOL::SendDC()
{
	if (ui.menuSPID->title() == "SPID") return;
	QString sendMsg = QString::number(SPID) + "$DISCONNECT$$$$";
	char sendMsgChar[1024] = { 0 };
	strcpy_s(sendMsgChar, sendMsg.toStdString().c_str());
	int sendRe = mp_clientSocket->write(sendMsgChar, strlen(sendMsgChar));
	if (sendRe == -1)
	{
		ui.labelLog->setText(tr("向服务端发送数据失败！"));
		return;
	}

}

void DinoOL::SendCL(int lifeNum)
{
	if (ui.menuSPID->title() == "SPID" || ui.menuROOM->title() == "ROOM") return;
	QString sendMsg = QString::number(SPID) + "$CGLIFE$" + ui.menuROOM->title().split('=')[1] + "$" + QString::number(lifeNum) + "$$$";
	char sendMsgChar[1024] = { 0 };
	strcpy_s(sendMsgChar, sendMsg.toStdString().c_str());
	int sendRe = mp_clientSocket->write(sendMsgChar, strlen(sendMsgChar));
	if (sendRe == -1)
	{
		ui.labelLog->setText(tr("向服务端发送数据失败！"));
		return;
	}

}

int DinoOL::isAllReady()
{
	if (WebGame)
	{
		if (ui.tableRoomer->item(0, 3)->text() != tr("否") && ui.tableRoomer->item(1, 3)->text() != "否" && ui.tableRoomer->item(2, 3)->text() != "否")
			return 1;
		else
			return 0;
	}
	else
	{
		if (isStarted)
			return 1;
		else
			return 0;
	}
	return 0;
}

void DinoOL::RKey(int id, int key, int isPress)
{
	R[id]->keyPR(key, isPress);
}

int DinoOL::readDataFile()
{
	if (QFileInfo(qApp->applicationDirPath() + "gamedata.dat").exists())
	{
		std::ifstream fin;
		fin.open(qApp->applicationDirPath().toStdString() + "gamedata.dat");
		if (!fin)
		{
			return 1;
		}
		std::string strcrc;
		int vernum, s, j, d, rj, star, a;
		fin >> strcrc >> s >> j >> d >> rj >> star >> vernum;
		fin.close();
		a = s + j + d + rj + star + vernum + 2636;
		QByteArray raw = QByteArray::fromStdString(QString::number(a).toStdString());
		if (QString::number(calcCRC32(raw), 16).toUpper() == QString::fromStdString(strcrc))
		{
			ui.actionMaxScore->setText(QString::number(s));
			ui.actionJump->setText(QString::number(j));
			ui.actionDive->setText(QString::number(d));
			ui.actionStar->setText(QString::number(star));
			ui.actionRunJump->setText(QString::number(rj));
			if (s >= 10000)//分数记录大于10000时默认勾选离线模式
				ui.actionOffline->setChecked(Qt::Checked);
			return 0;
		}
		else
		{
			return writeDataFile();
		}
	}
	else
	{
		return writeDataFile();
	}
}

int DinoOL::writeDataFile()
{
	int a = VERSIONID + ui.actionMaxScore->text().toInt() + ui.actionJump->text().toInt() + ui.actionDive->text().toInt() + ui.actionRunJump->text().toInt() + ui.actionStar->text().toInt();
	a += 2636;
	QByteArray raw = QByteArray::fromStdString(QString::number(a).toStdString());
	//QString::number(calcCRC32(raw), 16).toUpper();
	std::ofstream fout;
	fout.open(qApp->applicationDirPath().toStdString() + "gamedata.dat", std::ios::trunc);
	if (!fout)
	{
		return 1;
	}
	fout << QString::number(calcCRC32(raw), 16).toUpper().toStdString() << " ";
	fout << ui.actionMaxScore->text().toInt() << " " << ui.actionJump->text().toInt() << " " << ui.actionDive->text().toInt() << " " << ui.actionRunJump->text().toInt() << " ";
	fout << ui.actionStar->text().toInt() << " ";
	fout << VERSIONID;
	fout << "\nPlease_DONT_change_the_file_manually!";
	fout.close();
	return 0;
}

int DinoOL::Initialize()
{

	disconnect(ui.label_2, SIGNAL(linkActivated(QString)), this, SLOT(NetworkChk(QString)));
	disconnect(ui.label_3, SIGNAL(linkActivated(QString)), this, SLOT(NetworkChk(QString)));
	disconnect(ui.labelFail, SIGNAL(linkActivated(QString)), this, SLOT(reStart(QString)));
	disconnect(ptcloud, SIGNAL(timeout()), this, SLOT(cloudloop()));
	disconnect(pdtime, SIGNAL(timeout()), this, SLOT(printpos()));
	disconnect(ptOBS, SIGNAL(timeout()), this, SLOT(printOBS()));
	CleanAM(pAnimationRoad);
	if (labCloud != NULL)
	{
		delete[]labCloud;
		labCloud = NULL;
	}
	/*
	if (pAnimationRoad != NULL)
		delete[] pAnimationRoad;
	*/
	if (movie_cloud != NULL)
	{
		delete[]movie_cloud;
		movie_cloud = NULL;
	}
	if (pAnimationCloud != NULL)
	{
		delete[]pAnimationCloud;
		pAnimationCloud = NULL;
	}

	delete[] P1;
	delete[] P2;
	delete[] R[0];
	delete[] R[1];
	delete[] R;
	delete[] pdtime;
	delete[] ptcloud;
	delete[] ptOBS;

	P1 = NULL;
	P2 = NULL;
	R = NULL;
	pdtime = NULL;
	ptcloud = NULL;
	ptOBS = NULL;
	isStarted = 0;

	buildTime = __TIMESTAMP__;
	QString tmp = "DinoOL ";
#ifdef _DEBUG
	tmp += QString::fromStdString(VERSION) + ".debug By:Brownlzy ====D====E====B====U====G====";
	ui.txtServer->setText("127.0.0.1:30628");
	ui.txtNewRoom->setText("debug");
	ui.txtRoom->setText("debug");
#else
	tmp += QString::fromStdString(VERSION) + " By:Brownlzy";
	ui.actionDebug->setVisible(false);
#endif // _DEBUG
	this->setWindowTitle(tmp);
	this->setWindowIcon(QIcon(":/pic/icon/DinoOL"));
	int x = this->frameGeometry().width();
	int y = this->frameGeometry().height();
	qDebug() << ui.centralWidget->pos();
	ui.labelFail->hide();
	ui.frmLlife->hide();
	ui.frmRlife->hide();
	P1 = new Dino(x, y, this, this->centralWidget());
	P1->isOn = 1;
	P2 = new Dino(x, y, this, this->centralWidget());
	R = new Dino * [2];
	R[0] = new Dino(x, y, this, this->centralWidget());
	R[1] = new Dino(x, y, this, this->centralWidget());
	P1->setDinoState(":/pic/gif/dino_start");
	P1->move(0.2 * x, 0.2 * y);
	P1->adjustSize();
	P1->show(true);
	ui.frmScore->hide();
	ui.label_2->hide();
	ui.label_3->adjustSize();
	ui.labelchklcs->hide();
	ui.labReady->setText("<html><head/><body><p><img src="":/pic/png/6"" widyh=""162"" height=""198""/></p></body></html>");
	ui.labReady->adjustSize();
	ui.labReady->hide();
	ui.frame_4->hide();
	ui.lab_2->move(x - 1 - ui.lab_2->width(), ui.lab_2->y());
	ui.lab_3->move(x - 1 - ui.lab_3->width(), ui.lab_3->y());
	ui.lab_6->move(x - 1 - ui.lab_6->width(), ui.lab_6->y());
	maxH = vy0 * vy0 / (2 * G);
	pdtime = new QTimer(this);
	ptcloud = new QTimer(this);
	ptOBS = new QTimer(this);
	ptOBS->setInterval(tms);
	pdtime->setInterval(100);
	connect(ui.label_2, SIGNAL(linkActivated(QString)), this, SLOT(NetworkChk(QString)));
	connect(ui.label_3, SIGNAL(linkActivated(QString)), this, SLOT(NetworkChk(QString)));
	connect(ui.labelFail, SIGNAL(linkActivated(QString)), this, SLOT(reStart(QString)));
	connect(ptcloud, SIGNAL(timeout()), this, SLOT(cloudloop()));
	connect(pdtime, SIGNAL(timeout()), this, SLOT(printpos()));
	connect(ptOBS, SIGNAL(timeout()), this, SLOT(printOBS()));
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			ui.tableRoomer->setItem(i, j, new QTableWidgetItem(""));
		}
	}
	resizeDinoOL();
	readDataFile();

	return 0;
}

void DinoOL::CfgSet(int isRember, int x, int y, int w, int h, int isMax, int isMoon)
{
	if (!isRember)
		ui.action_3->setChecked(false);
	else
	{
		if (isMax == 2)
		{
			this->setWindowState(Qt::WindowMaximized);
		}
		else
		{
			this->setGeometry(x, y, w, h);
		}
		if (isMoon)
		{
			ui.labMoon->show();
			ui.labSunMoon->clear();
			ui.labSun->hide();
			isSun = 0;
			QColor c = QColor("#202020");//黑
			QPalette p = this->palette();
			p.setColor(QPalette::Window, c);
			this->setPalette(p);
			ui.labelchklcs->setStyleSheet("font: 9pt ""SimSun"";color:#707070;");
			ui.label_2->setStyleSheet("font: 9pt ""黑体"";color:#707070;");
			ui.label_3->setStyleSheet("font: 9pt ""黑体"";color:#707070;");
			//ChangeColor("TurnBlack");
		}
	}
}

void DinoOL::Transparent()
{
	cursorCurrentPointF = QPoint(0, 0);
	cursouPresss = actionActive = false;
	this->setWindowFlag(Qt::FramelessWindowHint); /* 注意：如果单纯开启窗口透明层效果，在Windows系统中必须设置, 其他系统可忽略。*/
	this->setAttribute(Qt::WA_TranslucentBackground);
	//d.setWindowState(Qt::WindowMaximized);
	ui.actionTransparent->setChecked(Qt::Checked);
	ui.line_7->hide();
	ui.labMoon->hide();
	ui.labSun->hide();
	ui.labSunMoon->hide();
	ui.menuBar->hide();
	//lab = new MoveLabel(this->centralWidget());
	//lab->setText("<html><head/><body><p><img src="":/pic/png/Drag""/></p></body></html>");
	//lab->move(100, 100);
	//lab->show();
	//connect(lab, SIGNAL(ActionCurrentPos(const bool&, const QPoint&)),
	//	this, SLOT(ActionCurrentPos(const bool&, const QPoint&)));
}

void DinoOL::Focus()
{
	this->setFocus();
}

void DinoOL::printOBS()
{
	//int isPause = 0;
	//OBS[0]->setFrameShape(QFrame::Box);
	for (int i = 0; i < 7; i++)
	{
		if (vOBS[i] == 0) continue;
		if (OBS[i]->x() < 0 - OBS[i]->width()) vOBS[i] = 0;
		if (!isPause)
		{
			OBS[i]->move(OBS[i]->x() + tobs.elapsed() * vOBS[i] / 1000, horline - (dy[i] % 100) * maxH / 10 - OBS[i]->height());
		}
		else
		{
			OBS[i]->move(OBS[i]->x(), horline - (dy[i] % 100) * maxH / 10 - OBS[i]->height());
		}
		if (!isPause && P2->isOn && !P2->isShining && isTouched(OBS[i], &P2->labDino))
		{
			if (dy[i] > 10)//star
			{
				OBS[i]->clear();
				P2->shining(0, 10);
				dy[i] = -9;
				goto aaa;
			}
			if (ui.labP1P2Life->text().split(':')[1].toInt() <= 1)
			{
				GamePause();
				mOBS[i]->stop();
				isPause = 1;
				P2->setDinoState(":/pic/png/dino_fail");
			}
			else { P2->shining(); }
			ui.lifeP2->setText("<html><head/><body><p><img src = "":/pic/png/" + QString::number(ui.labP1P2Life->text().split(':')[1].toInt() - 1) + """ width = ""36"" height = ""44""/></p></body></html>");
			ui.labP1P2Life->setText(ui.labP1P2Life->text().split(':')[0] + ":" + QString::number(ui.labP1P2Life->text().split(':')[1].toInt() - 1));
		}
	aaa:
		if (!isPause && !P1->isShining && isTouched(OBS[i], &P1->labDino))
		{
			if (dy[i] > 10)//star
			{
				OBS[i]->clear();
				P1->shining(0, 10);
				dy[i] = -9;
				ui.actionStar->setText(QString::number(ui.actionStar->text().toInt() + 1));
				//continue;
			}
			else if (!P2->isOn && !WebGame)
			{
				if (R[1]->isOn == 1 && (R[0]->isFail == 1 || R[1]->isFail == 1) || R[1]->isOn == 0)
				{
					if (ui.labP1P2Life->text().split(':')[0].toInt() <= 1 && !ui.chkCheat->isChecked())
					{
						GamePause();
						mOBS[i]->stop();
						isPause = 1;
						P1->setDinoState(":/pic/png/dino_fail");
						//ui.lab_7->setText("碰到障碍" + QString::number(i));
					}
					else { P1->shining(); }
					if (!ui.chkCheat->isChecked())
					{
						ui.lifeP1->setText("<html><head/><body><p><img src = "":/pic/png/" + QString::number(ui.labP1P2Life->text().split(':')[0].toInt() - 1) + """ width = ""36"" height = ""44""/></p></body></html>");
						ui.labP1P2Life->setText(QString::number(ui.labP1P2Life->text().split(':')[0].toInt() - 1) + ":" + ui.labP1P2Life->text().split(':')[1]);
					}
				}
				else
				{
					P1->isFail = 1;
					P1->setDinoState("");
				}
			}
			else if (!P1->isShining)
			{
				if (ui.labP1P2Life->text().split(':')[0].toInt() <= 1)
				{
					GamePause();
					mOBS[i]->stop();
					isPause = 1;
					P1->setDinoState(":/pic/png/dino_fail");
				}
				else { P1->shining(); }
				ui.lifeP1->setText("<html><head/><body><p><img src = "":/pic/png/" + QString::number(ui.labP1P2Life->text().split(':')[0].toInt() - 1) + """ width = ""36"" height = ""44""/></p></body></html>");
				ui.lifeP1R->setText("<html><head/><body><p><img src = "":/pic/png/" + QString::number(ui.labP1P2Life->text().split(':')[0].toInt() - 1) + """ width = ""36"" height = ""44""/></p></body></html>");
				ui.labP1P2Life->setText(QString::number(ui.labP1P2Life->text().split(':')[0].toInt() - 1) + ":" + ui.labP1P2Life->text().split(':')[1]);
				if (WebGame)
					SendCL(ui.labP1P2Life->text().split(':')[0].toInt());
			}
		}
	}
	refreshScore(Score.elapsed() / 100);

	if (!isPause) { tobs.start(); ptOBS->start(); }
}

int DinoOL::isTouched(QLabel* lab1, QLabel* lab2)
{
	int x1 = lab1->x() + lab1->width() / 2;
	int y1 = lab1->y() + 6 + lab1->height() / 2;
	int x2 = lab2->x() + lab2->width() / 2;
	int y2 = lab2->y() + lab2->height() / 2;
	int dx = fabs(x1 - x2);
	int dy = fabs(y1 - y2);
	int r1 = qMin(lab1->width(), lab1->height()) / 2;
	int r2 = qMin(lab2->width(), lab2->height()) / 2;
	if (dx > lab1->width() && dx > lab2->width() || dy > lab1->height() + 6 && dy > lab2->height())return 0;
	double ds = sqrt(dx * dx + dy * dy);
	//lab1->setFrameShape(QFrame::Box);
	//lab1->setText(QString::number(ds - r1 - r2));
	if (ds - r1 - r2 < 0)
		return 1;
	return 0;
}

void DinoOL::GamePause()
{
	int curScore = Score.elapsed() / 100;
	ptOBS->stop();
	if (P2->isOn) P2->Pause();
	P1->Pause();
	P2->Pause();
	R[0]->Pause();
	R[0]->Pause();
	ui.labelFail->show();
	ui.labelFail->raise();
	pAnimationRoad->stop();
	if (curScore > ui.actionMaxScore->text().toInt())
	{
		ui.actionMaxScore->setText(QString::number(curScore));
	}
	writeDataFile();
}

void DinoOL::refreshScore(int t)
{
	if (isPause) return;
	//if (vx0 < 1000)vx0 = t / 100 * 100 + 305.6;
	for (int i = 0; i < 6; i++)
	{
		if (t < 1000 && vOBS[i] && dy[i] > 0 && dy[i] <= 11)
			vOBS[i] = 0 - vx0 - int(log10(t) - 1) * 100.;
		else if (dy[i] > 0 && dy[i] <= 11 && vOBS[i])
			vOBS[i] = 0 - vx0 - 3. * 100;
	}
	if (!WebGame && !ui.chkCheat->isChecked() && !P2->isOn && flag4life != t && (t == 100 || t == 500 || t == 1000 || t == 2000 || t == 5000 || t == 10000))
	{
		ui.labP1P2Life->setText(QString::number(ui.labP1P2Life->text().split(':')[0].toInt() + 1) + ":" + ui.labP1P2Life->text().split(':')[1].toInt());
		ui.lifeP1->setText("<html><head/><body><p><img src = "":/pic/png/" + ui.labP1P2Life->text().split(':')[0] + """ width = ""36"" height = ""44""/></p></body></html>");
		flag4life = t;
	}
	ui.score7->setText("<html><head/><body><p><img src="":/pic/png/" + QString::number(t % 10) + """ widyh=""27"" height=""33""/></p></body></html>");
	t /= 10;
	ui.score6->setText("<html><head/><body><p><img src="":/pic/png/" + QString::number(t % 10) + """ widyh=""27"" height=""33""/></p></body></html>");
	t /= 10;
	ui.score5->setText("<html><head/><body><p><img src="":/pic/png/" + QString::number(t % 10) + """ widyh=""27"" height=""33""/></p></body></html>");
	t /= 10;
	ui.score4->setText("<html><head/><body><p><img src="":/pic/png/" + QString::number(t % 10) + """ widyh=""27"" height=""33""/></p></body></html>");
	t /= 10;
	ui.score3->setText("<html><head/><body><p><img src="":/pic/png/" + QString::number(t % 10) + """ widyh=""27"" height=""33""/></p></body></html>");
	t /= 10;
	ui.score2->setText("<html><head/><body><p><img src="":/pic/png/" + QString::number(t % 10) + """ widyh=""27"" height=""33""/></p></body></html>");
	t /= 10;
	ui.score1->setText("<html><head/><body><p><img src="":/pic/png/" + QString::number(t % 10) + """ widyh=""27"" height=""33""/></p></body></html>");
	t /= 10;
	ui.score0->setText("<html><head/><body><p><img src="":/pic/png/" + QString::number(t % 10) + """ widyh=""27"" height=""33""/></p></body></html>");
}


void DinoOL::ProduceOBS()
{
	int kind;
	kind = randNum(500) + 1000;		//计算下一次创建障碍物时间ms
	if (!isPause && (ui.labReady->text().split("/")[4].left(1) == "-"))
	{
		if (ui.tableRoomer->item(0, 1)->text().toInt() == SPID || !WebGame)
			QTimer::singleShot(kind, this, SLOT(ProduceOBS()));
	}
	if (!((WebGame && isAllReady()) || !WebGame))
	{
		return;
	}
	if (ui.labReady->text().split("/")[4].left(1).toInt() > 1)			//开始前54321倒数
	{
		ui.labReady->show();
		ui.labReady->setText("<html><head/><body><p><img src="":/pic/png/" + QString::number(ui.labReady->text().split("/")[4].left(1).toInt() - 1) + """ widyh=""162"" height=""198""/></p></body></html>");
		ui.labReady->adjustSize();
		QTimer::singleShot(1000, this, SLOT(ProduceOBS()));				//1秒后重新调用
		return;
	}
	else if (ui.labReady->text().split("/")[4].left(1) != "-")
	{
		ui.labReady->hide();
		ui.labReady->setText("<html><head/><body><p><img src="":/pic/png/-1"" widyh=""27"" height=""33""/></p></body></html>");
		//暂时不使用
		/*
		if (!WebGame && !P2->isOn)	//单人单机模式初始生命数为最高分数以10为底的对数
		{
			ui.lifeP1->setText("<html><head/><body><p><img src = "":/pic/png/" + QString::number(int(log10(ui.actionMaxScore->text().toInt() / 10))) + """ width = ""36"" height = ""44"" / >< / p>< / body>< / html>");
			ui.labP1P2Life->setText(QString::number(int(log10(ui.actionMaxScore->text().toInt() / 10))) + ":3");
		}*/
		if (!WebGame && !P2->isOn)	//单人单机模式初始生命数为最高分数以10为底的对数
		{
			ui.lifeP1->setText("<html><head/><body><p><img src = "":/pic/png/1"" width = ""36"" height = ""44""/></p></body></html>");
			ui.labP1P2Life->setText("1:3");
		}
		P1->shining();
		if (P2->isOn) P2->shining();
		QTimer::singleShot(1000, this, SLOT(ProduceOBS()));				//1秒后重新调用自己
	}
	if (WebGame && ui.tableRoomer->item(0, 1)->text().toInt() != SPID) return;
	int dy = 0;
	kind = randNum(3);
	if (!kind)			//kind = 0时，生成鸟或星星
	{
		if (!WebGame)
		{
			kind = randNum(10);//kind为0生成星星1/30
			if (kind == 0)
			{
				kind = 2;
				dy %= 3;
				dy += 107;
			}
			/*else if (kind == 9)
			{
				kind = 3;
				dy %= 2;
				dy += 208;
			}*/
			else
			{
				kind = 0;
				dy = randNum(10) + 1;
			}
		}
		else
		{
			dy = randNum(10) + 1;
		}
	}
	else
	{
		kind = randNum(6) + 10;
		dy = 0;
	}
	if (ui.tableRoomer->item(0, 1)->text().toInt() == SPID)
		SendObstacle(kind, dy);
	else if (!WebGame)
		ProduceOBS(kind, dy);
}

void DinoOL::reStart(QString)
{
	SendDC();
	closeEvent(NULL);
	qApp->exit(-2);
}

void DinoOL::ProduceOBS(int kind, int dy)
{
	/*kind:dinool.qrc文件名
	* 0-bird
	* 2-star
	* 10-cactus_s_1 * 13-.......l_1
	* 11-.......s_2 * 14-.......l_2
	* 12-.......s_3 * 15-.......l_3
	*/
	int i;
	QString path;
	if (kind == 0) path = ":/pic/gif/bird";
	else if (kind == 2) path = ":/pic/gif/star";
	else if (kind == 3) path = ":/pic/png/H";
	else
		path = ":/pic/obs/" + QString::number(kind);
	for (i = 0; i < 7; i++)
	{
		if (vOBS[i] == 0)
		{
			if (OBS[i] == NULL) OBS[i] = new QLabel(this->centralWidget());
			OBS[i]->show();
			setOBSPic(path, i);
			OBS[i]->adjustSize();
			if (kind >= 10)
				OBS[i]->setGeometry(1920, horline - (dy % 100) * maxH * 0.1 - 2. * OBS[i]->height(), 2 * OBS[i]->width(), 2 * OBS[i]->height());
			else
				OBS[i]->setGeometry(1920, horline - 2. * OBS[i]->height(), 2 * OBS[i]->width(), 2 * OBS[i]->height());
			OBS[i]->setScaledContents(true);
			OBS[i]->raise();
			vOBS[i] = 0. - vx0;
			DinoOL::dy[i] = dy;
			ptOBS->start();
			break;
		}
	}
	if (Score.elapsed() == 0)
	{
		Score.start();
		ui.frmScore->show();
	}
	if (!WebGame)
	{
		ui.frmLlife->show();
		if (!P2->isOn)
		{
			ui.frmLlife->setGeometry(0, 0, 71, 61);
			if (ui.chkCheat->isChecked())
				ui.lifeP1->setText("<html><head/><body><p><img src = "":/pic/png/infinity"" width = ""36"" height = ""44""/></p></body></html>");
		}
	}
	else if (WebGame && isAllReady())
	{
		if (!R[1]->isOn && !R[0]->isOn)
		{
			ui.frmRlife->setGeometry(0, 0, 71, 61);
			ui.frmRlife->show();
		}
		else if (!R[1]->isOn)
		{
			ui.frmRlife->setGeometry(0, 0, 161, 61);
			ui.frmRlife->show();
		}
		else
		{
			ui.frmRlife->show();
		}
	}

}

void DinoOL::StartStep1() { StartGame(1); }
void DinoOL::StartStep2() { StartGame(2); }

void DinoOL::roadloop()
{
	if (isPause) return;
	int t;
	int y = this->frameGeometry().height();
	CleanAM(pAnimationRoad);
	if (pAnimationRoad == NULL)
		pAnimationRoad = new QPropertyAnimation(ui.labRoad, "pos");
	if (isStarted >= 3)
	{
		if (ui.labRoad->x() == -2400) ui.labRoad->move(0, ui.labRoad->y());
		t = (ui.labRoad->x() + 0.4 * ui.labRoad->width()) / vx0;
		pAnimationRoad->setDuration(t * 1000);
		pAnimationRoad->setStartValue(QPoint(ui.labRoad->x(), ui.labRoad->y()));
		pAnimationRoad->setEndValue(QPoint(0 - ui.labRoad->width() * 0.4, ui.labRoad->y()));
		pAnimationRoad->setLoopCount(-1);
		pAnimationRoad->start();
	}
	else
	{
		pAnimationRoad->stop();
	}
}

void DinoOL::cloudloop()
{
	if (labCloud == NULL)
	{
		labCloud = new QLabel[3];
		for (int i = 0; i < 3; i++)
		{
			labCloud[i].setParent(this);
		}
	}
	if (movie_cloud == NULL)
	{
		movie_cloud = new QMovie[3];
		for (int i = 0; i < 3; i++)
		{
			movie_cloud[i].setParent(this);
		}
	}
	if (pAnimationCloud == NULL)
	{
		pAnimationCloud = new QPropertyAnimation[3];
		for (int i = 0; i < 3; i++)
		{
			pAnimationCloud[i].setParent(this);
		}
	}
	int x = this->frameGeometry().width();
	int y = this->frameGeometry().height();
	pAnimationCloud[cloudid].setTargetObject(&labCloud[cloudid]);
	pAnimationCloud[cloudid].setPropertyName("pos");
	int t1, t2;
	float cy;
	t1 = randNum(500) + 4000;
	t2 = randNum(500) + 1500;
	cy = 0.2 + randNum(3) / 10.0;
	movie_cloud->setFileName(":/pic/png/cloud");
	labCloud[cloudid].setGeometry(0, 0.2 * y, 46, 13);
	labCloud[cloudid].setMovie(movie_cloud);
	labCloud[cloudid].setScaledContents(true);
	movie_cloud->start();
	labCloud[cloudid].setGeometry(x, 0.2 * y, 2 * labCloud[cloudid].width(), 2 * labCloud[cloudid].height());
	labCloud[cloudid].show();
	pAnimationCloud[cloudid].setDuration(t1);
	pAnimationCloud[cloudid].setStartValue(QPoint(x, cy * y));
	pAnimationCloud[cloudid].setEndValue(QPoint(0 - labCloud[cloudid].width(), cy * y));
	pAnimationCloud[cloudid].start();
	if (cloudid == 2)
	{
		cloudid = 0;
	}
	else
	{
		cloudid++;
	}
	ptcloud->setInterval(t2);
	ptcloud->start();
	if (isStarted == 3) isStarted++;
}

void DinoOL::toServer()
{
	ui.labelchklcs->show();
	Loading l;
	l.show();
	if (!ui.actionOffline->isChecked())
	{
		int exec;
		if (pUpdate->isLatest)
			exec = 0;
		else
			exec = pUpdate->CheckUpdate();
		if (exec > 0)
		{
			l.close();
			pAbout->show();
			if (pAbout->ui.btnShowUpdateInfo->text() == "U") pAbout->ShowUpdateInfo();
			//pAbout->CheckUpdate();
			ui.labelchklcs->hide();
			isStarted = 0;
			return;
		}
	}
	l.close();
	ui.labelchklcs->hide();
#ifdef _DEBUG
	qApp->exit(-1);
#else
	QStringList Par;
	Par << "/DinoOLServer";
	QProcess::startDetached(qApp->applicationFilePath(), Par);
#endif // _DEBUG

}

void DinoOL::ExitApp()
{
	closeEvent(NULL);
	qApp->exit(0);
}

void DinoOL::ConnectServer()
{
	if (isAllReady()) return;
	ui.frame_4->move((this->frameGeometry().width() - ui.frame_4->width()) / 2, ui.frame_4->y());
	ui.frame_4->setVisible(true);
	ui.frame_4->raise();
	if (ui.btnCon->text() == tr("已连接")) QTimer::singleShot(3000, ui.frame, SLOT(hide()));
}

void DinoOL::DebugInfo()
{
	if (P1 != NULL)P1->debug();
	if (P2 != NULL)P2->debug();
	if (R[0] != NULL)R[0]->debug();
	if (R[1] != NULL)R[1]->debug();
	for (int i = 0; i < 7; i++)
	{
		if (OBS[i] != NULL)OBS[i]->setFrameShape(QFrame::Box);
	}
	pdtime->start();
}

void DinoOL::NewOne()
{
	QProcess::startDetached(qApp->applicationFilePath());
}

void DinoOL::RestartApp()
{
	reStart("");
	//太乱了，以后再搞；
	//Initialize();
}

void DinoOL::ShowAbout() //关于
{
	pAbout->setWindowIcon(QIcon(":/pic/icon/DinoOL"));
	pAbout->show();
	pAbout->CheckUpdate();
	//QEventLoop loop;
	//QObject::connect(reply, SIGNAL(finished()), &loop, SLOT(quit()));
	//loop.exec();

}

void DinoOL::Cheat()
{
	if (ui.actionCheat->isChecked())
	{
		ui.chkCheat->setCheckState(Qt::Checked);
	}
	else
	{
		ui.chkCheat->setCheckState(Qt::Unchecked);
	}
}

void DinoOL::Offline()
{
	if (ui.actionOffline->isChecked())
	{
		bool isOK;
		QString strPass = QInputDialog::getText(this, tr("离线验证"), tr("请输入离线密码:"), QLineEdit::Normal, tr(""), &isOK);
		QByteArray raw = QByteArray::fromStdString(strPass.toStdString());
		qDebug() << QString::number(calcCRC32(raw), 16).toUpper();
		if (!isOK || (isOK && QString::number(calcCRC32(raw), 16).toUpper() != "E5AE29DF"))
			ui.actionOffline->setChecked(Qt::Unchecked);
	}
}

void DinoOL::PayQR()
{
	QLabel* plab = new QLabel("<html><head/><body><p><img src="":/pic/png/pay"" widyh=""191"" height=""191""/></p></body></html>");
	plab->setFixedSize(191, 191);
	plab->setWindowIcon(QIcon(":/pic/icon/DinoOL"));
	plab->setWindowFlags(Qt::WindowCloseButtonHint | Qt::WindowStaysOnTopHint);
	plab->setWindowTitle("注意:赞助是无偿自愿行为");
	plab->show();
}

void DinoOL::Help()
{
	QDesktopServices::openUrl(QUrl(QLatin1String("https://brownlzy.github.io/2021/07/02/DinoOL/")));
}

void DinoOL::aTransparent()
{
	if (ui.actionTransparent->isChecked())
	{
		//this->setWindowFlag(Qt::FramelessWindowHint); /* 注意：如果单纯开启窗口透明层效果，在Windows系统中必须设置, 其他系统可忽略。*/
		//this->setAttribute(Qt::WA_TranslucentBackground);
		qApp->exit(-3);
	}
	else
	{
		//this->setWindowFlags(Qt::WindowCloseButtonHint | Qt::WindowMinMaxButtonsHint);
		//qApp->exit(-4);
		reStart("");
	}
}

void DinoOL::btnCon()
{
	if (ui.menuSPID->title() != "SPID") return;
	mp_clientSocket = new QTcpSocket();
	QString ip = ui.txtServer->text().split(':')[0];
	QString tmp = ui.txtServer->text() + "::";
	int port = tmp.split(':')[1].toInt();
	if (port == 0) port = 30628;
	mp_clientSocket->connectToHost(ip, port);
	if (!mp_clientSocket->waitForConnected(30000))
	{
		ui.labelLog->setText(tr("连接服务端失败！"));
		return;
	}
	connect(mp_clientSocket, SIGNAL(readyRead()), this, SLOT(ClientRecvData()));
	connect(mp_clientSocket, SIGNAL(disconnected()), this, SLOT(sDisConnection()));
}

void DinoOL::btnWhatsThis()
{
	QWhatsThis::enterWhatsThisMode();
}

void DinoOL::btnRandRoom()
{
	if (ui.menuSPID->title() == "SPID")
	{
		ui.labelLog->setText(tr("你还未连接至服务器并取得有效SPID"));
		return;
	}
	if (ui.menuROOM->title() != "ROOM")
	{
		ui.labelLog->setText(tr("你已加入房间！"));
		return;
	}
	QString sendMsg = QString::number(SPID) + "$JOIN$2$$$$";
	char sendMsgChar[1024] = { 0 };
	strcpy_s(sendMsgChar, sendMsg.toStdString().c_str());
	int sendRe = mp_clientSocket->write(sendMsgChar, strlen(sendMsgChar));
	if (sendRe == -1)
	{
		ui.labelLog->setText(tr("向服务端发送数据失败！"));
		return;
	}

}

void DinoOL::btnCreRoom()
{
	if (ui.menuSPID->title() == "SPID")
	{
		ui.labelLog->setText(tr("你还未连接至服务器并取得有效SPID"));
		return;
	}
	if (ui.txtNewRoom->text().contains("$") || ui.txtNewRoom->text().contains("="))
	{
		ui.labelLog->setText(tr("房间号不能含有“$”，请重新输入"));
		return;
	}
	QString sendMsg = QString::number(SPID) + "$ROOM$1$" + ui.txtNewRoom->text() + "$$$";
	char sendMsgChar[1024] = { 0 };
	strcpy_s(sendMsgChar, sendMsg.toStdString().c_str());
	int sendRe = mp_clientSocket->write(sendMsgChar, strlen(sendMsgChar));
	if (sendRe == -1)
	{
		ui.labelLog->setText(tr("向服务端发送数据失败！"));
		return;
	}
}

void DinoOL::btnExitRoom()
{
	if (ui.menuROOM->title() == "ROOM")
	{
		ui.labelLog->setText(tr("你还未加入任何房间！"));
		return;
	}
	if (isStarted) GamePause();
	QString sendMsg = ui.menuSPID->title().split("=")[1];
	sendMsg += "$JOIN$0$" + ui.menuSPID->title().split("=")[1] + "$$$";
	char sendMsgChar[1024] = { 0 };
	strcpy_s(sendMsgChar, sendMsg.toStdString().c_str());
	int sendRe = mp_clientSocket->write(sendMsgChar, strlen(sendMsgChar));
	if (sendRe == -1)
	{
		ui.labelLog->setText(tr("向服务端发送数据失败！"));
		return;
	}
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			ui.tableRoomer->item(i, j)->setText("");
		}
	}
	ui.menuROOM->setTitle("ROOM");
}

void DinoOL::btnSend()
{
	QString sendMsg = ui.txtSend->toPlainText();
	char sendMsgChar[1024] = { 0 };
	strcpy_s(sendMsgChar, sendMsg.toStdString().c_str());
	int sendRe = mp_clientSocket->write(sendMsgChar, strlen(sendMsgChar));
	if (sendRe == -1)
	{
		ui.labelLog->setText(tr("向服务端发送数据失败！"));
		return;
	}
}

void DinoOL::btnJoin()
{
	if (ui.txtRoom->text().contains("$"))
	{
		ui.labelLog->setText(tr("房间号不能含有“$”，请重新输入"));
		return;
	}
	QString sendMsg = QString::number(SPID) + "$JOIN$1$" + ui.txtRoom->text() + "$$$";
	char sendMsgChar[1024] = { 0 };
	strcpy_s(sendMsgChar, sendMsg.toStdString().c_str());
	int sendRe = mp_clientSocket->write(sendMsgChar, strlen(sendMsgChar));
	if (sendRe == -1)
	{
		ui.labelLog->setText(tr("向服务端发送数据失败！"));
		return;
	}

}

void DinoOL::btnHD()
{
	ui.frame_4->hide();
	this->setFocus();
}


void DinoOL::chkCheat()
{
	if (ui.chkCheat->isChecked())
		ui.lifeP1->setText("<html><head/><body><p><img src = "":/pic/png/infinity"" width = ""36"" height = ""44"" / >< / p>< / body>< / html>");
	else
		ui.lifeP1->setText("<html><head/><body><p><img src = "":/pic/png/" + ui.labP1P2Life->text().split(':')[0] + """ width = ""36"" height = ""44"" / >< / p>< / body>< / html>");
}

void DinoOL::ClientRecvData()
{
	char recvMsg[1024] = { 0 };
	int recvRe = mp_clientSocket->read(recvMsg, 1024);
	if (recvRe == -1)
	{
		ui.labelLog->setText(tr("接收服务端数据失败！"));
		return;
	}
	QString showQstr = recvMsg;
	ui.txtGet->setText(showQstr);
	ProcessSMsg(showQstr);
}

void DinoOL::sDisConnection()
{
	ui.menuSPID->setTitle("SPID");
	ui.menuROOM->setTitle("ROOM");
}

int randNum(int Max)
{
	QTime time = QTime::currentTime();
	qsrand(time.msec() + time.second() * 1000);
	int n = qrand() % Max;
	return n;
}

MoveLabel::MoveLabel(QWidget* aa) :QLabel(aa)
{
}

void MoveLabel::mousePressEvent(QMouseEvent* e)
{
	if (e->button() == Qt::LeftButton)
	{
		isPressed = true;
		emit ActionCurrentPos(false, this->cursor().pos());
	}
}

void MoveLabel::mouseReleaseEvent(QMouseEvent* e)
{
	if (e->button() == Qt::LeftButton)
	{
		isPressed = false;
		emit ActionCurrentPos(false, QPoint(0, 0));
	}
}

void MoveLabel::mouseMoveEvent(QMouseEvent* e)
{
	if (isPressed)emit ActionCurrentPos(true, this->cursor().pos());
}
