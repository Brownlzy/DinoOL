#include "dinool.h"

DinoOL::DinoOL(QWidget* parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	buildTime = __TIMESTAMP__;
	this->setWindowIcon(QIcon(":/pic/icon/DinoOL"));
	setDinoState(":/pic/gif/dino_start");
	ui.label_2->hide();
	ui.label_3->adjustSize();
	//ui.frame->hide();
	//ui.frame_2->hide();
	//ui.frame_3->hide();
	ui.labelchklcs->hide();
	ui.whitebg->hide();
	ui.frame_4->hide();
	ui.labelR1->setVisible(false);
	ui.labelR2->setVisible(false);
	ui.labelL->setVisible(false);
	ui.lab_2->move(x - 1 - ui.lab_2->width(), ui.lab_2->y());
	ui.lab_3->move(x - 1 - ui.lab_3->width(), ui.lab_3->y());
	ui.labelP->setVisible(false);
	ui.labelP1->setVisible(false);
	ui.labelP2->setVisible(false);
	ui.labelPL->setVisible(false);
	connect(ui.label_2, SIGNAL(linkActivated(QString)), this, SLOT(NetworkChk(QString)));
	maxH = vy0 * vy0 / (2 * G);
	pdtime = new QTimer(this);
	//proad = new QTimer(this);
	ptcloud = new QTimer(this);
	ptdino = new QTimer(this);
	ptOBS = new QTimer(this);
	Lptdino = new QTimer(this);
	Rptdino[0] = new QTimer(this);
	Rptdino[1] = new QTimer(this);
	ptdino->setInterval(tms);
	ptOBS->setInterval(tms);
	Lptdino->setInterval(tms);
	Rptdino[0]->setInterval(tms);
	Rptdino[1]->setInterval(tms);
	pdtime->setInterval(100);
	//connect(proad, SIGNAL(timeout()), this, SLOT(roadloop()));
	connect(ptcloud, SIGNAL(timeout()), this, SLOT(cloudloop()));
	connect(pdtime, SIGNAL(timeout()), this, SLOT(printpos()));
	connect(ptdino, SIGNAL(timeout()), this, SLOT(printDino()));
	connect(ptOBS, SIGNAL(timeout()), this, SLOT(printOBS()));
	connect(Lptdino, SIGNAL(timeout()), this, SLOT(printDinoL()));
	connect(Rptdino[0], SIGNAL(timeout()), this, SLOT(printDino1()));
	connect(Rptdino[1], SIGNAL(timeout()), this, SLOT(printDino2()));
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			ui.tableRoomer->setItem(i, j, new QTableWidgetItem(""));
		}
	}
	//pdtime->start();
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
}
void DinoOL::printpos()
{
	QString tmp;
	tmp = "(" + QString::number(ui.label->x()) + "," + QString::number(ui.label->y()) + ")";
	tmp = tmp + ",isDive:" + QString::number(isDive) + ",isJump:" + QString::number(isJump) + ",isMove:" + QString::number(isMove);
	tmp += ",vx=" + QString::number(vx) + ",vy=" + QString::number(vy) + ",onG=" + QString::number(isOnGround())
		+ ",f[0]=" + QString::number(f[0]) + ",f[1]=" + QString::number(f[1]);
	ui.lab->setText(tmp);
	ui.lab->adjustSize();

	tmp = "(" + QString::number(ui.labelR1->x()) + "," + QString::number(ui.labelR1->y()) + ")";
	tmp = tmp + ",RisDive[0]:" + QString::number(RisDive[0]) + ",RisJump[0]:" + QString::number(RisJump[0]) + ",RisMove[0]:" + QString::number(RisMove[0]);
	tmp += ",Rvx[0]=" + QString::number(Rvx[0]) + ",Rvy[0]=" + QString::number(Rvy[0]) + ",onG=" + QString::number(isOnGround(0));
	ui.lab_2->setText(tmp);
	ui.lab_2->adjustSize();

	tmp = "(" + QString::number(ui.labelL->x()) + "," + QString::number(ui.labelL->y()) + ")";
	tmp = tmp + ",LisDive:" + QString::number(LisDive) + ",LisJump:" + QString::number(LisJump) + ",LisMove:" + QString::number(LisMove);
	tmp += ",Lvx=" + QString::number(Lvx) + ",Lvy=" + QString::number(Lvy) + ",onG=" + QString::number(isOnGround(-1));
	ui.lab_4->setText(tmp);
	ui.lab_4->adjustSize();

	tmp = "(" + QString::number(ui.labelR2->x()) + "," + QString::number(ui.labelR2->y()) + ")";
	tmp = tmp + ",RisDive[1]:" + QString::number(RisDive[1]) + ",RisJump[1]:" + QString::number(RisJump[1]) + ",RisMove[1]:" + QString::number(RisMove[1]);
	tmp += ",Rvx[1]=" + QString::number(Rvx[1]) + ",Rvy[1]=" + QString::number(Rvy[1]) + ",onG=" + QString::number(isOnGround(1));
	ui.lab_3->setText(tmp);
	ui.lab_3->adjustSize();

	tmp = "buildTime " + buildTime + "|isWEBGAME " + QString::number(WebGame) + "|";
	for (int i = 0; i < 7; i++)
	{
		tmp += QString::number(maxH) + "|OBS[" + QString::number(i) + "]" + QString::number(vOBS[i]) + " " + QString::number(dy[i]) + " ";
		if (OBS[i] != NULL) tmp += "(" + QString::number(OBS[i]->x()) + "," + QString::number(OBS[i]->y()) + ") ";
	}
	ui.lab_5->setText(tmp);
	ui.lab_5->adjustSize();


	//pdtime->start();
}
void DinoOL::resizeEvent(QResizeEvent* event)
{
	//获取ui形成窗口宽度
	int x = this->frameGeometry().width();
	int y = this->frameGeometry().height();
	horline = 0.6 * y;
	qDebug() << x << "," << y;
	if (!isStarted)
	{
		ui.label->move(0.2 * x, 0.2 * y);
		ui.labRoad->move(0.2 * x, 0.2 * y + 34);
	}
	else if (isStarted == 1)
	{
		ui.label->move(0.2 * x, 0.2 * y - 81);
		ui.labRoad->move(0.2 * x, 0.2 * y - 43);
	}
	else if (isStarted == 2)
	{
		ui.label->move(0.2 * x, 0.2 * y);
		ui.labRoad->move(0.2 * x, 0.2 * y + 34);
	}
	else if (isStarted >= 3)
	{
		if (x < 8000)
		{
			ui.label->setGeometry(ui.label->x() > x - ui.label->width() ? x - ui.label->width() : ui.label->x(), horline - ui.label->height(), ui.label->width(), ui.label->height());
			ui.labelL->setGeometry(ui.labelL->x() > x - ui.labelL->width() ? x - ui.labelL->width() : ui.labelL->x(), horline - ui.labelL->height(), ui.labelL->width(), ui.labelL->height());
			ui.labelR1->setGeometry(ui.labelR1->x(), horline - ui.label->height(), ui.label->width(), ui.label->height());
			ui.labelR2->setGeometry(ui.labelR2->x(), horline - ui.label->height(), ui.label->width(), ui.label->height());
			ui.labRoad->move(ui.labRoad->x(), horline - 0.2551 * ui.label->height());
		}
		else
		{
			ui.label->setGeometry(ui.label->x() > x - ui.label->width() ? x - ui.label->width() : ui.label->x(), horline - ui.label->height(), 88 / 800.0 * x, 98 / 800.0 * x);
			ui.labRoad->setGeometry(0, (horline - 0.2551 * ui.label->height()), 2 * x, x / 100.0);
		}
	}
	ui.frame_4->move((x - ui.frame_4->width()) / 2, ui.frame_4->y());
	ui.label_2->move(ui.label->x(), ui.label->y() + 70);
	ui.label_3->move(ui.label->x(), ui.label->y() + 70);
	ui.whitebg->setGeometry(0, 0, x, y);
	ui.lab_2->move(x - 1 - ui.lab_2->width(), ui.lab_2->y());
	ui.lab_3->move(x - 1 - ui.lab_3->width(), ui.lab_3->y());
	ui.labelP2->move(ui.labelR2->x() + 0.5 * ui.labelR2->width() - 10, ui.labelR2->y() - 30);
	ui.labelP1->move(ui.labelR1->x() + 0.5 * ui.labelR1->width() - 10, ui.labelR1->y() - 30);
	ui.labelPL->move(ui.labelL->x() + 0.5 * ui.labelL->width() - 10, ui.labelL->y() - 30);
	ui.labelP->move(ui.label->x() + 0.5 * ui.label->width() - 10, ui.label->y() - 30);

	roadloop();
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

void DinoOL::keyPressEvent(QKeyEvent* e)
{
	if (isStarted == 0 && (e->key() == Qt::Key_Space || e->key() == Qt::Key_W))
	{
		isStarted = -1;
		ui.labelchklcs->show();
		if (getWebSource(QUrl("https://brownlzy.github.io/DinoOLtest.txt")) != "ALLOWED")
		{
			ui.labelchklcs->hide();
			QMessageBox::critical(this, "无许可！", "请向开发者征求许可！");
			isStarted = 0;
			setDinoState(":/pic/gif/dino_jump");
			ui.label->setGeometry(0.2 * this->frameGeometry().width(), 0.2 * this->frameGeometry().height() - 81, 44, 130);
			ui.label->setScaledContents(true);
			//qApp->exit(0);
			return;
		}
		ui.labelchklcs->hide();
		StartGame();
		return;
	}
	if (isStarted < 4) return;
	bool onG = isOnGround();
	if (e->key() == Qt::Key_Up && !WebGame)
	{
		if (!LisOn)
		{
			ui.labelL->setGeometry(0, -98, 88, 98);
			ui.labelL->setScaledContents(true);
			ui.labelL->setVisible(true);
			LisOn = 1;
			Lptdino->start();
		}
		else if (isOnGround(-1))
		{
			Lptdino->start();
			Lvy += vy0;
			LisJump = 1;
		}
	}
	if (e->key() == Qt::Key_W && onG)
	{
		ptdino->start();
		vy += vy0;
		isJump = 1;
		if (WebGame) SendPOS(ui.label->x(), horline - ui.label->y(), e->key(), 1);
	}
	if (!e->isAutoRepeat()) //&& onG)
	{
		if (WebGame) SendPOS(ui.label->x(), horline - ui.label->y(), e->key(), 1);
		switch (e->key())
		{
		case (Qt::Key_A): //A
			ptdino->start();
			if (onG)
			{
				isMove = -1;
				vx = 0 - vx0;
			}
			else
			{
				isMove = -2;
				//vx = 0;
			}
			break;
		case (Qt::Key_S): //S
			if (!isDive)// && !isJump)
			{
				setDinoState(":/pic/gif/dino_dive");
				isDive = true;
				ui.label->setGeometry(ui.label->x(), ui.label->y() + 38, 118, 60);
				ui.label->setScaledContents(true);
			}
			break;
		case (Qt::Key_D): //D
			ptdino->start();
			if (onG)
			{
				isMove = 1;
				vx = vx0;
			}
			else
			{
				isMove = 2;
				//vx = 0;
			}
			break;
		default:

			break;
		}
		if (!LisOn) return;
		onG = isOnGround(-1);
		switch (e->key())
		{
		case (Qt::Key_Left): //<-
			Lptdino->start();
			if (onG)
			{
				LisMove = -1;
				vx = 0 - vx0;
			}
			else
			{
				LisMove = -2;
			}
			break;
		case (Qt::Key_Down): //S
			if (!LisDive)// && !isJump)
			{
				setDinoState(":/pic/gif/dino_dive", -1);
				LisDive = true;
				ui.labelL->setGeometry(ui.labelL->x(), ui.labelL->y() + 38, 118, 60);
				ui.labelL->setScaledContents(true);
			}
			break;
		case (Qt::Key_Right): //D
			Lptdino->start();
			if (onG)
			{
				LisMove = 1;
				Lvx = vx0;
			}
			else
			{
				LisMove = 2;
			}
			break;

		default:
			break;
		}
	}
}

void DinoOL::keyReleaseEvent(QKeyEvent* e)
{
	if (isStarted < 4) return;
	bool onG = isOnGround();
	if (!e->isAutoRepeat())
	{
		if (WebGame) SendPOS(ui.label->x(), horline - ui.label->y(), e->key(), 0);
		switch (e->key())
		{
		case (Qt::Key_A): //A
			if (isOnGround())
			{
				vx = 0;
				ptdino->stop();
			}
			isMove = 0;
			break;
		case (Qt::Key_D): //D
			if (isOnGround())
			{
				vx = 0;
				ptdino->stop();
			}
			isMove = 0;
			break;
		case (Qt::Key_S): //S
			if (isDive)
			{
				setDinoState(":/pic/gif/dino_run");
				ui.label->setGeometry(ui.label->x(), ui.label->y() - 38, 88, 98);
				isDive = false;
				ui.label->setScaledContents(true);
			}
			break;

		default:
			break;
		}
		if (!LisOn) return;
		onG = isOnGround(-1);
		switch (e->key())
		{
		case (Qt::Key_Left): //A
			if (onG)
			{
				Lvx = 0;
				Lptdino->stop();
			}
			LisMove = 0;
			break;
		case (Qt::Key_Right): //D
			if (onG)
			{
				Lvx = 0;
				Lptdino->stop();
			}
			LisMove = 0;
			break;
		case (Qt::Key_Down): //S
			if (LisDive)
			{
				setDinoState(":/pic/gif/dino_run", -1);
				ui.labelL->setGeometry(ui.labelL->x(), ui.labelL->y() - 38, 88, 98);
				LisDive = false;
				ui.labelL->setScaledContents(true);
			}
			break;

		default:
			break;
		}

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
		setDinoState(":/pic/gif/dino_jump");
		ui.label->setGeometry(0.2 * x, 0.2 * y - 81, 44, 130);
		ui.label->setScaledContents(true);
		QTimer::singleShot(900, this, SLOT(StartStep1()));
		isStarted = 1;
		SendReady();
		break;
	case(1):
		setDinoState(":/pic/gif/dino_run");
		ui.label->setGeometry(0.2 * x, 0.2 * y, 44, 49);
		ui.label->setScaledContents(true);
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
		CleanAM(pAnimation2);
		pAnimation1 = new QPropertyAnimation(ui.label, "geometry");
		pAnimation1->setDuration(700);
		pAnimation1->setStartValue(QRect(ui.label->x(), ui.label->y(), ui.label->width(), ui.label->height()));
		pAnimation1->setEndValue(QRect(0, 0.6 * y - 2.0 * ui.label->height(), 2 * ui.label->width(), 2 * ui.label->height()));
		pAnimation2 = new QPropertyAnimation(ui.labRoad, "geometry");
		pAnimation2->setDuration(700);
		pAnimation2->setStartValue(QRect(ui.labRoad->x(), ui.labRoad->y(), ui.labRoad->width(), ui.labRoad->height()));
		//pAnimation2->setEndValue(QRect(0, 0.6 * y - 2.0 * ui.label->height() + 73, 250 * ui.labRoad->height(), 1.0 * ui.labRoad->height()));
		pAnimation2->setEndValue(QRect(0, 0.6 * y - 2.0 * ui.label->height() + 73, 6000, 24));
		pAnimation1->start();
		pAnimation2->start();
		ui.labRoad->setScaledContents(true);
		ui.labelR1->setGeometry(0, 0.6 * y - 98, 88, 98);
		ui.labelR2->setGeometry(0, 0.6 * y - 98, 88, 98);
		ui.labelR1->setScaledContents(true);
		ui.labelR2->setScaledContents(true);

		/*
		if (f[0])
		{
			ui.labelR1->setVisible(true);
			setDinoState(":/pic/gif/dino_run", 0);
		}
		if (f[1])
		{
			ui.labelR2->setVisible(true);
			setDinoState(":/pic/gif/dino_run", 1);
		}
		*/
		isStarted = 3;
		QTimer::singleShot(700, this, SLOT(roadloop()));
		//connect(pAnimationRoad, SIGNAL(finished()), this, SLOT(roadloop()));
		QTimer::singleShot(700, this, SLOT(cloudloop()));
		QTimer::singleShot(4000, this, SLOT(ProduceOBS()));
		break;
	}
}

bool DinoOL::isOnGround()
{
	double ly = ui.label->y();
	double lh = ui.label->height();
	double tmp = ly + lh - horline;
	if (tmp <= vy0 * tms / 1000.0 && tmp >= 0 - vy0 * tms / 1000.0)
	{
		return true;
	}
	return false;
}

bool DinoOL::isOnGround(int id)
{
	double ly;
	double lh;
	double tmp;
	switch (id)
	{
	case -1:
		ly = ui.labelL->y();
		lh = ui.labelL->height();
		tmp = ly + lh - horline;
		if (tmp <= vy0 * tms / 1000.0 && tmp >= 0 - vy0 * tms / 1000.0)
		{
			return true;
		}
		return false;
		break;
	case 0:
		ly = ui.labelR1->y();
		lh = ui.labelR1->height();
		tmp = ly + lh - horline;
		if (tmp <= vy0 * tms / 1000.0 && tmp >= 0 - vy0 * tms / 1000.0)
		{
			return true;
		}
		return false;
		break;
	case 1:
		ly = ui.labelR2->y();
		lh = ui.labelR2->height();
		tmp = ly + lh - horline;
		if (tmp <= vy0 * tms / 1000.0 && tmp >= 0 - vy0 * tms / 1000.0)
		{
			return true;
		}
		return false;
		break;
	}
}

void DinoOL::setDinoState(QString pic)
{
	if (movie_dino == NULL) movie_dino = new QMovie(this);
	movie_dino->stop();
	movie_dino->setFileName(pic);
	ui.label->setMovie(movie_dino);
	movie_dino->start();
}

void DinoOL::setDinoState(QString pic, int id)
{
	if (id >= 0)
	{
		if (Rmovie_dino[id] == NULL) Rmovie_dino[id] = new QMovie(this);
		Rmovie_dino[id]->stop();
		Rmovie_dino[id]->setFileName(pic);
		switch (id)
		{
		case 1:
			ui.labelR2->setMovie(Rmovie_dino[id]);
			break;
		case 0:
			ui.labelR1->setMovie(Rmovie_dino[id]);
			break;
		}
		Rmovie_dino[id]->start();
	}
	else
	{
		if (Lmovie_dino == NULL) Lmovie_dino = new QMovie(this);
		Lmovie_dino->stop();
		Lmovie_dino->setFileName(pic);
		ui.labelL->setMovie(Lmovie_dino);
		Lmovie_dino->start();
	}
}

void DinoOL::setOBSPic(QString pic, int id)
{
	if (mOBS[id] == NULL) mOBS[id] = new QMovie(this);
	mOBS[id]->stop();
	mOBS[id]->setFileName(pic);
	OBS[id]->setMovie(mOBS[id]);
	mOBS[id]->start();
}

void DinoOL::ProcessSMsg(QString msg)
{
	QString fun = msg.split('$')[0];
	if (fun == "POS")
	{//POS$PID$dx#dy#key#1$$$
		QString strRKey = msg.split('$')[2];
		if (msg.split('$')[1].toInt() == f[0])
		{
			ui.labelR1->move(strRKey.split('#')[0].toDouble(), horline - strRKey.split('#')[1].toDouble());
			RKey(0, strRKey.split('#')[2].toInt(), strRKey.split('#')[3].toInt());
		}
		else if (msg.split('$')[1].toInt() == f[1])
		{
			ui.labelR2->move(strRKey.split('#')[0].toDouble(), horline - strRKey.split('#')[1].toDouble());
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
		if (ui.tableRoomer->item(0, 3)->text().split('-')[0] == "是"
			&& (!(ui.tableRoomer->item(1, 1)->text() != "") || ui.tableRoomer->item(1, 3)->text() == "是")
			&& (!(ui.tableRoomer->item(2, 1)->text() != "") || ui.tableRoomer->item(2, 3)->text() == "是"))
		{
			for (int i = 0, j = 0; i < 3; i++)
			{
				if (ui.tableRoomer->item(i, 1)->text() != ui.menuSPID->title().split('=')[1])
				{
					f[j++] = ui.tableRoomer->item(i, 1)->text().toInt();
					continue;
				}
			}

			WebGame = 1;
		}
	}
	else if (fun == "ARGU")
	{
		G = msg.split('$')[1].toDouble();
		vx0 = msg.split('$')[2].toDouble();
		vy0 = msg.split('$')[3].toDouble();
		maxH = vy0 * vy0 / (2 * G);
	}
	else if (fun == "PID")
	{
		int pid = msg.split('$')[1].toInt();
		if (pid == 100)
		{
			mp_clientSocket->disconnect();
			ui.txtGet->setPlainText("PID=100 x disconnected");
		}
		else
		{
			SPID = pid;
			QString tmp = "SPID=" + QString::number(SPID);
			ui.menuSPID->setTitle(tmp);
			ui.label_3->move(ui.label_2->x(), ui.label_2->y());
			ui.label_2->setVisible(false);
			ui.label_3->setVisible(true);
		}
	}
	else if (fun == "ROOM")
	{
		int state = msg.split('$')[1].toInt();
		ui.labelLog->setText(msg.split('$')[3]);
		switch (state)
		{
		case 1:
			ui.menuROOM->setTitle("ROOM=" + msg.split('$')[2]);
			ui.tableRoomer->item(0, 0)->setText("是");
			ui.tableRoomer->item(0, 1)->setText(QString::number(SPID));
			ui.tableRoomer->item(0, 2)->setText("Grey");
			ui.tableRoomer->item(0, 3)->setText("否");
			break;
		default:
			break;
		}
	}
	else if (fun == "JOIN")
	{
		int state = msg.split('$')[1].toInt();
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
					ui.tableRoomer->item(i, 3)->setText("否");
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
		ui.labelLog->setText("QT网络通信向服务端发送数据失败！");
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
		ui.labelLog->setText("QT网络通信向服务端发送数据失败！");
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
		ui.labelLog->setText("QT网络通信向服务端发送数据失败！");
		return;
	}

}

void DinoOL::RKey(int id, int key, int isPress)
{
	bool onG = isOnGround(id);
	if (isPress)
	{
		if (key == Qt::Key_W && onG)
		{
			Rptdino[id]->start();
			Rvy[id] += vy0;
			RisJump[id] = 1;
		}
		switch (key)
		{
		case (Qt::Key_A): //A
			Rptdino[id]->start();
			if (onG)
			{
				RisMove[id] = -1;
				Rvx[id] = 0 - vx0;
			}
			else
			{
				RisMove[id] = -2;
				//vx = 0;
			}
			break;
		case (Qt::Key_S): //S
			if (!RisDive[id])// && !isJump)
			{
				setDinoState(":/pic/gif/dino_dive", id);
				RisDive[id] = true;
				if (id == 0)
				{
					ui.labelR1->setGeometry(ui.labelR1->x(), ui.labelR1->y() + 38, 118, 60);
					ui.labelR1->setScaledContents(true);
				}
				else
				{
					ui.labelR2->setGeometry(ui.labelR2->x(), ui.labelR2->y() + 38, 118, 60);
					ui.labelR2->setScaledContents(true);
				}
			}
			break;
		case (Qt::Key_D): //D
			Rptdino[id]->start();
			if (onG)
			{
				RisMove[id] = 1;
				Rvx[id] = vx0;
			}
			else
			{
				RisMove[id] = 2;
				//vx = 0;
			}
			break;
		default:

			break;
		}

	}
	else
	{
		switch (key)
		{
		case (Qt::Key_A): //A
			if (onG)
			{
				Rvx[id] = 0;
				Rptdino[id]->stop();
			}
			RisMove[id] = 0;
			break;
		case (Qt::Key_D): //D
			if (onG)
			{
				Rvx[id] = 0;
				Rptdino[id]->stop();
			}
			RisMove[id] = 0;
			break;
		case (Qt::Key_S): //S
			if (RisDive[id])
			{
				setDinoState(":/pic/gif/dino_run", id);
				if (id == 0)
				{
					ui.labelR1->setGeometry(ui.labelR1->x(), ui.labelR1->y() - 38, 88, 98);
					ui.labelR1->setScaledContents(true);
				}
				else
				{
					ui.labelR2->setGeometry(ui.labelR2->x(), ui.labelR2->y() - 38, 88, 98);
					ui.labelR2->setScaledContents(true);
				}
				RisDive[id] = false;
			}
			break;

		default:
			break;
		}
	}
}

void DinoOL::printOBS()
{
	//if (ui.labRoad->x() < -1200) ui.labRoad->move(ui.labRoad->x() + 1200, ui.labRoad->y());
	//ui.labRoad->move(ui.labRoad->x() - vx0 * tobs.elapsed() / 1000, ui.labRoad->y());
	for (int i = 0; i < 7; i++)
	{
		if (vOBS[i] == 0) continue;
		if (OBS[i]->x() < 0 - OBS[i]->width()) vOBS[i] = 0;
		OBS[i]->move(OBS[i]->x() + tobs.elapsed() * vOBS[i] / 1000, horline - dy[i] * maxH / 10 - OBS[i]->height() + 26);
	}
	tobs.start();
	ptOBS->start();
}

void DinoOL::printDino()
{
	if (ui.label->x() < 0) { vx = fabs(vx); isMove = fabs(isMove); }
	if (!WebGame && ui.label->x() > this->frameGeometry().width() - ui.label->width()) { vx = 0 - fabs(vx); isMove = 0 - fabs(isMove); }
	if (vy > 0 - vy0 * tms / 2000 || vy < vy0 * tms / 2000 && !isOnGround())
		vy -= G * tms / 1000;
	else if (isOnGround())
	{
		vy = 0;
		if (!isMove)
		{
			vx = 0;
			ptdino->stop();
		}
		ui.label->move(ui.label->x(), horline - ui.label->height());
	}
	if ((isMove >= -1 && isMove <= 1 && !isOnGround()) || isOnGround())
	{
		if (isOnGround() && isMove) vx = isMove / fabs(isMove) * vx0;
		ui.label->move(ui.label->x() + vx / 1000 * tms, ui.label->y() - vy / 1000 * tms);
	}
	else
		ui.label->move(ui.label->x() + vx / 1000 * tms, ui.label->y() - vy / 1000 * tms);
	if (isOnGround() && isJump == 1)
	{
		isJump = 0;
		if (!isDive)
			setDinoState(":/pic/gif/dino_run");
	}
	if (!isOnGround() && isJump == 0)
		isJump = 1;

	if (isOnGround() && isJump == 0)
	{
	}
	else if (!isOnGround() && isJump == 1)
	{
		if (!isDive)
			setDinoState(":/pic/png/dino");
	}
	ui.labelP->setVisible(true);
	if (!isDive)
		ui.labelP->move(ui.label->x() + 0.5 * ui.label->width() - 10, ui.label->y() - 30);
	else
		ui.labelP->move(ui.label->x() + 0.5 * ui.label->width() - 25, ui.label->y() - 68);

}

void DinoOL::printDinoL()
{
	if (ui.labelL->x() < 0) { Lvx = fabs(Lvx); LisMove = fabs(LisMove); }
	if (ui.labelL->x() > this->frameGeometry().width() - ui.labelL->width()) { Lvx = 0 - fabs(Lvx); LisMove = 0 - fabs(LisMove); }
	if (Lvy > 0 - vy0 * tms / 2000 || Lvy < vy0 * tms / 2000 && !isOnGround(-1))
		Lvy -= G * tms / 1000;
	else if (isOnGround(-1))
	{
		Lvy = 0;
		if (!LisMove)
		{
			vx = 0;
			Lptdino->stop();
		}
		ui.labelL->move(ui.labelL->x(), horline - ui.labelL->height());
	}
	if ((LisMove >= -1 && LisMove <= 1 && !isOnGround(-1)) || isOnGround(-1))
	{
		if (isOnGround(-1) && LisMove) Lvx = LisMove / fabs(LisMove) * vx0;
		ui.labelL->move(ui.labelL->x() + Lvx / 1000 * tms, ui.labelL->y() - Lvy / 1000 * tms);
	}
	else
		ui.labelL->move(ui.labelL->x() + Lvx / 1000 * tms, ui.labelL->y() - Lvy / 1000 * tms);
	if (isOnGround(-1) && LisJump == 1)
	{
		LisJump = 0;
		if (!LisDive)
			setDinoState(":/pic/gif/dino_run", -1);
	}
	if (!isOnGround(-1) && LisJump == 0)
		LisJump = 1;

	if (isOnGround(-1) && LisJump == 0)
	{
	}
	else if (!isOnGround(-1) && LisJump == 1)
	{
		if (!LisDive)
			setDinoState(":/pic/png/dino", -1);
	}
	ui.labelPL->setVisible(true);
	if (!isDive)
		ui.labelPL->move(ui.labelL->x() + 0.5 * ui.labelL->width() - 10, ui.labelL->y() - 30);
	else
		ui.labelPL->move(ui.labelL->x() + 0.5 * ui.labelL->width() - 25, ui.labelL->y() - 68);
}

void DinoOL::printDino1()
{
	if (ui.labelR1->x() < 0) { Rvx[0] = fabs(Rvx[0]); RisMove[0] = fabs(RisMove[0]); }
	if (Rvy[0] > 0 - Rvy[0] * tms / 2000 || Rvy[0] < vy0 * tms / 2000 && !isOnGround(0))
		Rvy[0] -= G * tms / 1000;
	else if (isOnGround(0))
	{
		Rvy[0] = 0;
		if (!RisMove[0])
		{
			Rvx[0] = 0;
			Rptdino[0]->stop();
		}
		ui.labelR1->move(ui.labelR1->x(), horline - ui.labelR1->height());
	}
	if ((RisMove[0] >= -1 && RisMove[0] <= 1 && !isOnGround(0)) || isOnGround(0))
	{
		if (isOnGround(0) && RisMove[0]) Rvx[0] = RisMove[0] / fabs(RisMove[0]) * vx0;
		ui.labelR1->move(ui.labelR1->x() + Rvx[0] / 1000 * tms, ui.labelR1->y() - Rvy[0] / 1000 * tms);
	}
	else
		ui.labelR1->move(ui.labelR1->x() + Rvx[0] / 1000 * tms, ui.labelR1->y() - Rvy[0] / 1000 * tms);
	if (isOnGround(0) && RisJump[0] == 1)
	{
		RisJump[0] = 0;
		if (!RisDive[0])
			setDinoState(":/pic/gif/dino_run", 0);
	}
	if (!isOnGround(0) && RisJump[0] == 0)
		RisJump[0] = 1;

	if (isOnGround(0) && RisJump[0] == 0)
	{
	}
	else if (!isOnGround(0) && RisJump[0] == 1)
	{
		if (!RisDive[0])
			setDinoState(":/pic/png/dino", 0);
	}
	ui.labelP1->setVisible(true);
	if (!isDive)
		ui.labelP1->move(ui.labelR1->x() + 0.5 * ui.labelR1->width() - 10, ui.labelR1->y() - 30);
	else
		ui.labelP1->move(ui.labelR1->x() + 0.5 * ui.labelR1->width() - 25, ui.labelR1->y() - 68);
}
void DinoOL::printDino2()
{
	if (ui.labelR2->x() < 0) { Rvx[1] = fabs(Rvx[1]); RisMove[1] = fabs(RisMove[1]); }
	if (Rvy[1] > 0 - Rvy[1] * tms / 2000 || Rvy[1] < vy0 * tms / 2000 && !isOnGround(1))
		Rvy[1] -= G * tms / 1000;
	else if (isOnGround(1))
	{
		Rvy[1] = 0;
		if (!RisMove[1])
		{
			Rvx[1] = 0;
			Rptdino[1]->stop();
		}
		ui.labelR2->move(ui.labelR2->x(), horline - ui.labelR2->height());
	}
	if ((RisMove[1] >= -1 && RisMove[1] <= 1 && !isOnGround(1)) || isOnGround(1))
	{
		if (isOnGround(1) && RisMove[1]) Rvx[1] = RisMove[1] / fabs(RisMove[1]) * vx0;
		ui.labelR2->move(ui.labelR2->x() + Rvx[1] / 1000 * tms, ui.labelR2->y() - Rvy[1] / 1000 * tms);
	}
	else
		ui.labelR2->move(ui.labelR2->x() + Rvx[1] / 1000 * tms, ui.labelR2->y() - Rvy[1] / 1000 * tms);
	if (isOnGround(1) && RisJump[1] == 1)
	{
		RisJump[1] = 0;
		if (!RisDive[1])
			setDinoState(":/pic/gif/dino_run", 1);
	}
	if (!isOnGround(1) && RisJump[1] == 0)
		RisJump[1] = 1;

	if (isOnGround(1) && RisJump[1] == 0)
	{
	}
	else if (!isOnGround(1) && RisJump[1] == 1)
	{
		if (!RisDive[1])
			setDinoState(":/pic/png/dino", 1);
	}
	ui.labelP2->setVisible(true);
	if (!isDive)
		ui.labelP2->move(ui.labelR2->x() + 0.5 * ui.labelR2->width() - 10, ui.labelR2->y() - 30);
	else
		ui.labelP2->move(ui.labelR2->x() + 0.5 * ui.labelR2->width() - 25, ui.labelR2->y() - 68);
}

void DinoOL::ProduceOBS()
{
	if (!(!WebGame || ui.tableRoomer->item(0, 1)->text().toInt() == SPID))
	{
		return;
	}
	int kind;
	int dy = 0;
	kind = randNum(3);
	if (!kind)			//kind = 0时，生成鸟
	{
		dy = randNum(11);
	}
	else
	{
		kind = randNum(6) + 10;
	}
	if (ui.tableRoomer->item(0, 1)->text().toInt() == SPID)
		SendObstacle(kind, dy);
	else
		ProduceOBS(kind, dy);
	kind = randNum(1000) + 1000;		//计算下一次创建障碍物时间ms
	if (!WebGame || ui.tableRoomer->item(0, 1)->text().toInt() == SPID)
		QTimer::singleShot(kind, this, SLOT(ProduceOBS()));
}

void DinoOL::ProduceOBS(int kind, int dy)
{
	/*kind:
	* 0-bird
	* 10-cactus_s_1 * 13-.......l_1
	* 11-.......s_2 * 14-.......l_2
	* 12-.......s_3 * 15-.......l_3
	*/
	int i;
	QString path;
	if (kind == 0) path = ":/pic/gif/bird";
	else
		path = ":/pic/obs/" + QString::number(kind);
	for (i = 0; i < 7; i++)
	{
		if (vOBS[i] == 0)
		{
			if (OBS[i] == NULL) OBS[i] = new QLabel(this);
			OBS[i]->show();
			setOBSPic(path, i);
			OBS[i]->adjustSize();
			OBS[i]->setGeometry(1920, horline - dy * maxH * 0.1 - 2. * OBS[i]->height() + 26, 2 * OBS[i]->width(), 2 * OBS[i]->height());
			OBS[i]->setScaledContents(true);
			OBS[i]->raise();
			vOBS[i] = 0. - vx0;
			DinoOL::dy[i] = dy;
			ptOBS->start();
			break;
		}
	}
}

void DinoOL::StartStep1() { StartGame(1); }
void DinoOL::StartStep2() { StartGame(2); }

void DinoOL::roadloop()
{
	int t;
	int y = this->frameGeometry().height();
	CleanAM(pAnimationRoad);
	if (pAnimationRoad == NULL)
		pAnimationRoad = new QPropertyAnimation(ui.labRoad, "pos");
	if (isStarted >= 3)
	{
		if (ui.labRoad->x() == -2400) ui.labRoad->move(0, ui.labRoad->y());
		//if (ui.labRoad->x() >= -2300)
		//{
		//t = (2400.0) / vx0;
		t = (ui.labRoad->x() + 0.4 * ui.labRoad->width()) / vx0;
		pAnimationRoad->setDuration(t * 1000);
		pAnimationRoad->setStartValue(QPoint(ui.labRoad->x(), ui.labRoad->y()));
		pAnimationRoad->setEndValue(QPoint(0 - ui.labRoad->width() * 0.4, ui.labRoad->y()));
		pAnimationRoad->setLoopCount(-1);
		pAnimationRoad->start();
		//proad->setInterval(t * 1000);
		//proad->start();
	//}
	//else
	//{
		//t = 1200 / vx0;
		//t = (0.4 * ui.labRoad->width()) / vx0;
		//ui.labRoad->move(0, ui.labRoad->y());
		//pAnimationRoad->setDuration(t * 1000);
		//pAnimationRoad->setStartValue(QPoint(0, ui.labRoad->y()));
		//pAnimationRoad->setEndValue(QPoint(0 - ui.labRoad->width() * 0.4, ui.labRoad->y()));
		//pAnimationRoad->start();
		//proad->setInterval(t * 1000);
		//proad->start();
	//}
	}
	else
	{
		pAnimationRoad->stop();
	}
	if (isStarted == 3)
	{
		ui.labelP->setVisible(true);
		ui.labelP->move(ui.label->x() + 0.5 * ui.label->width() - 10, ui.label->y() - 30);
		isStarted++;
	}

	if (f[0] && !fchk[0])
	{
		ui.labelP1->setText(QString::number(f[0]));
		ui.labelR1->setVisible(true);
		setDinoState(":/pic/gif/dino_run", 0);
		fchk[0]++;
	}
	if (f[1] && !fchk[1])
	{
		ui.labelP2->setText(QString::number(f[1]));
		ui.labelR2->setVisible(true);
		setDinoState(":/pic/gif/dino_run", 1);
		fchk[1]++;
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
	ui.label->raise();
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

void DinoOL::on_actionRun_as_a_server_triggered()
{
	qDebug() << getWebSource(QUrl("https://brownlzy.github.io/DinoOLtest.txt"));
	if (getWebSource(QUrl("https://brownlzy.github.io/DinoOLtest.txt")) != "ALLOWED")
	{
		ui.actionRun_as_a_server->setText("Can't get permission");
		return;
	}
	qApp->exit(-1);
}

void DinoOL::on_actionExit_triggered()
{
	qApp->exit(0);
}

void DinoOL::on_actionConnect_a_server_triggered()
{
	if (isStarted) return;
	ui.frame_4->move((this->frameGeometry().width() - ui.frame_4->width()) / 2, ui.frame_4->y());
	ui.frame_4->setVisible(true);
	if (ui.btnCon->text() == "已连接") QTimer::singleShot(3000, ui.frame, SLOT(hide()));
}

void DinoOL::on_actionCreate_a_room_triggered()
{
	ui.frame_2->setVisible(true);

}

void DinoOL::on_actionJoin_a_room_triggered()
{
	ui.frame_3->setVisible(true);

}

void DinoOL::on_actionDebug_triggered()
{
	ui.label->setFrameShape(QFrame::Box);
	ui.labelR1->setFrameShape(QFrame::Box);
	ui.labelR2->setFrameShape(QFrame::Box);
	ui.labelL->setFrameShape(QFrame::Box);
	for (int i = 0; i < 7; i++)
	{
		if (OBS[i] != NULL)OBS[i]->setFrameShape(QFrame::Box);
	}
	pdtime->start();
}

void DinoOL::on_btnCon_clicked()
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
		ui.labelLog->setText("QT网络通信连接服务端失败！");
		return;
	}
	connect(mp_clientSocket, SIGNAL(readyRead()), this, SLOT(ClientRecvData()));
	connect(mp_clientSocket, SIGNAL(disconnected()), this, SLOT(sDisConnection()));
}

void DinoOL::on_btnCreRoom_clicked()
{
	QString sendMsg = QString::number(SPID) + "$ROOM$1$" + ui.txtNewRoom->text() + "$$$";
	char sendMsgChar[1024] = { 0 };
	strcpy_s(sendMsgChar, sendMsg.toStdString().c_str());
	int sendRe = mp_clientSocket->write(sendMsgChar, strlen(sendMsgChar));
	if (sendRe == -1)
	{
		ui.labelLog->setText("QT网络通信向服务端发送数据失败！");
		return;
	}
}

void DinoOL::on_btnSend_clicked()
{
	QString sendMsg = ui.txtSend->toPlainText();
	char sendMsgChar[1024] = { 0 };
	strcpy_s(sendMsgChar, sendMsg.toStdString().c_str());
	int sendRe = mp_clientSocket->write(sendMsgChar, strlen(sendMsgChar));
	if (sendRe == -1)
	{
		ui.labelLog->setText("QT网络通信向服务端发送数据失败！");
		return;
	}
}

void DinoOL::on_btnJoin_clicked()
{
	QString sendMsg = QString::number(SPID) + "$JOIN$1$" + ui.txtRoom->text() + "$$$";
	char sendMsgChar[1024] = { 0 };
	strcpy_s(sendMsgChar, sendMsg.toStdString().c_str());
	int sendRe = mp_clientSocket->write(sendMsgChar, strlen(sendMsgChar));
	if (sendRe == -1)
	{
		ui.labelLog->setText("QT网络通信向服务端发送数据失败！");
		return;
	}

}

void DinoOL::on_btnHD_clicked()
{
	ui.frame_4->hide();
	this->setFocus();
}

void DinoOL::ClientRecvData()
{
	char recvMsg[1024] = { 0 };
	int recvRe = mp_clientSocket->read(recvMsg, 1024);
	if (recvRe == -1)
	{
		ui.labelLog->setText("QT网络通信接收服务端数据失败！");
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

QString getWebSource(QUrl url)
{
	QNetworkAccessManager manager;
	QEventLoop loop;
	QNetworkReply* reply;
	QNetworkRequest m_request(url);
	qDebug() << "Reading html code form " << url;
	QSslConfiguration config;
	QSslConfiguration conf = m_request.sslConfiguration();
	conf.setPeerVerifyMode(QSslSocket::VerifyNone);
	conf.setProtocol(QSsl::TlsV1SslV3);
	m_request.setSslConfiguration(conf);
	reply = manager.get(m_request);
	//请求结束并下载完成后，退出子事件循环
	QObject::connect(reply, SIGNAL(finished()), &loop, SLOT(quit()));
	QTimer::singleShot(15000, &loop, SLOT(quit()));
	//开启子事件循环
	loop.exec();

	QByteArray codeContent = reply->readAll();

	//将获取到的网页源码写入文件
	//一定要注意编码问题，否则很容易出现乱码的
	return QTextCodec::codecForHtml(codeContent)->toUnicode(codeContent);
}
