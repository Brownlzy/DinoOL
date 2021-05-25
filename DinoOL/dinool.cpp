#include "dinool.h"

DinoOL::DinoOL(QWidget* parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	buildTime = __TIMESTAMP__;
	this->setWindowIcon(QIcon(":/pic/icon/DinoOL"));
	int x = this->frameGeometry().width();
	int y = this->frameGeometry().height();
	qDebug() << ui.centralWidget->pos();
	ui.labelFail->hide();
	P1 = new Dino(x, y, this, this->centralWidget());
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
	ui.whitebg->hide();
	ui.frame_4->hide();
	ui.lab_2->move(x - 1 - ui.lab_2->width(), ui.lab_2->y());
	ui.lab_3->move(x - 1 - ui.lab_3->width(), ui.lab_3->y());
	ui.lab_6->move(x - 1 - ui.lab_6->width(), ui.lab_6->y());
	connect(ui.label_2, SIGNAL(linkActivated(QString)), this, SLOT(NetworkChk(QString)));
	connect(ui.labelFail, SIGNAL(linkActivated(QString)), this, SLOT(reStart(QString)));
	maxH = vy0 * vy0 / (2 * G);
	pdtime = new QTimer(this);
	ptcloud = new QTimer(this);
	ptOBS = new QTimer(this);
	ptOBS->setInterval(tms);
	pdtime->setInterval(100);
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
	delete[] P1;
	delete[] P2;
	delete[] R[0];
	delete[] R[1];
	delete[] R;
}
void DinoOL::printpos()
{
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

	tmp = "0.3.Alpha BT:" + buildTime;
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
	//获取ui形成窗口宽度
	int x = this->frameGeometry().width();
	int y = this->frameGeometry().height();
	horline = 0.6 * y;
	P1->setfxy(x, y);
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
		if (!WebGame)
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
		if (getWebSource(QUrl("https://brownlzy.github.io/DinoOLver.txt")) != DINOVER)
		{
			ui.labelchklcs->hide();
			QMessageBox::critical(this, "无许可！", "请向开发者征求许可！");
			isStarted = 0;
			P1->setDinoState(":/pic/gif/dino_jump");
			P1->setGeometry(0.2 * this->frameGeometry().width(), 0.2 * this->frameGeometry().height() - 83, 44, 130);
			P1->setScaledContents(true);
			//qApp->exit(0);
			return;
		}
		ui.labelchklcs->hide();
		StartGame();
		return;
	}
	if (isStarted < 4) return;
	if (e->key() == Qt::Key_Up && !WebGame)
	{
		if (!P2->isOn)
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
	if (e->key() == Qt::Key_W && onG)
	{
		P1->start();
		P1->vy += P1->vy0;
		P1->isJump = 1;
		if (WebGame) SendPOS(P1->x(), horline - P1->y(), e->key(), 1);
	}
	if (!e->isAutoRepeat())
	{
		if (WebGame) SendPOS(P1->x(), horline - P1->y(), e->key(), 1);
		if (e->key() == Qt::Key_A || e->key() == Qt::Key_D || e->key() == Qt::Key_S) P1->keyPR(e->key(), 1);
		if (P2->isOn && (e->key() == Qt::Key_Left || e->key() == Qt::Key_Right || e->key() == Qt::Key_Down)) P2->keyPR(e->key(), 1);
	}
}

void DinoOL::keyReleaseEvent(QKeyEvent* e)
{
	if (isStarted < 4) return;
	if (!e->isAutoRepeat())
	{
		if (WebGame) SendPOS(P1->x(), horline - P1->y(), e->key(), 0);
		if (e->key() == Qt::Key_A || e->key() == Qt::Key_D || e->key() == Qt::Key_S) P1->keyPR(e->key(), 0);
		if (P2->isOn && (e->key() == Qt::Key_Left || e->key() == Qt::Key_Right || e->key() == Qt::Key_Down)) P2->keyPR(e->key(), 0);
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
		SendReady();
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
		QTimer::singleShot(700, this, SLOT(roadloop()));
		QTimer::singleShot(700, this, SLOT(cloudloop()));
		QTimer::singleShot(700, P1, SLOT(showP()));
		QTimer::singleShot(4000, this, SLOT(ProduceOBS()));
		break;
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
		if (ui.tableRoomer->item(0, 3)->text().split('-')[0] == "是"
			&& (!(ui.tableRoomer->item(1, 1)->text() != "") || ui.tableRoomer->item(1, 3)->text() == "是")
			&& (!(ui.tableRoomer->item(2, 1)->text() != "") || ui.tableRoomer->item(2, 3)->text() == "是"))
		{
			for (int i = 0, j = 0; i < 3; i++)
			{
				if (ui.tableRoomer->item(i, 1)->text() != ui.menuSPID->title().split('=')[1])
				{
					R[j]->changeP(ui.tableRoomer->item(i, 1)->text());
					R[j]->labP.setVisible(false);
					R[j]->setDinoState(":/pic/gif/dino_run");
					j++;
					continue;
				}
			}
			WebGame = 1;
			P1->WebGame = 1;
			R[0]->WebGame = 1;
			R[1]->WebGame = 1;
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
	R[id]->keyPR(key, isPress);
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
			OBS[i]->move(OBS[i]->x() + tobs.elapsed() * vOBS[i] / 1000, horline - dy[i] * maxH / 10 - OBS[i]->height());
		}
		else
		{
			OBS[i]->move(OBS[i]->x(), horline - dy[i] * maxH / 10 - OBS[i]->height());
		}
		if (!WebGame && isTouched(OBS[i], &P1->labDino) && !P2->isOn)
		{
			GamePause();
			mOBS[i]->stop();
			isPause = 1;
			//ui.lab_7->setText("碰到障碍" + QString::number(i));
		}
	}
	refreshScore(Score.elapsed() / 100);
	tobs.start();
	if (!isPause)ptOBS->start();
}

int DinoOL::isTouched(QLabel* lab1, QLabel* lab2)
{
	int x1 = lab1->x() + lab1->width() / 2;
	int y1 = lab1->y() + 4 + lab1->height() / 2;
	int x2 = lab2->x() + lab2->width() / 2;
	int y2 = lab2->y() + lab2->height() / 2;
	int dx = x1 - x2;
	int dy = y1 - y2;
	int r1 = qMin(lab1->width(), lab1->height()) / 2;
	int r2 = qMin(lab2->width(), lab2->height()) / 2;

	double ds = sqrt(dx * dx + dy * dy);
	//lab1->setFrameShape(QFrame::Box);
	//lab1->setText(QString::number(ds - r1 - r2));
	if (ds - r1 - r2 < 0)
		return 1;
	return 0;
}

void DinoOL::GamePause()
{
	ptOBS->stop();
	if (!P2->isOn)
		P1->Pause();
	//P2->Pause();
	//R[0]->Pause();
	//R[0]->Pause();
	ui.labelFail->show();
	ui.labelFail->raise();
	pAnimationRoad->stop();
}

void DinoOL::refreshScore(int t)
{
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
	ui.score1->setText("<html><head/><body><p><img src="":/pic/png/" + QString::number(t / 100 % 1000) + """ widyh=""27"" height=""33""/></p></body></html>");
	t /= 10;
	ui.score0->setText("<html><head/><body><p><img src="":/pic/png/" + QString::number(t / 100 % 1000) + """ widyh=""27"" height=""33""/></p></body></html>");
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
	if ((!WebGame || ui.tableRoomer->item(0, 1)->text().toInt() == SPID) && !isPause)
	{
		if (Score.elapsed() == 0)
		{
			Score.start();
			ui.frmScore->show();
		}
		//refreshScore(Score.elapsed() / 100);
		QTimer::singleShot(kind, this, SLOT(ProduceOBS()));

	}
}

void DinoOL::reStart(QString)
{
	qApp->exit(-2);
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
			if (OBS[i] == NULL) OBS[i] = new QLabel(this->centralWidget());
			OBS[i]->show();
			setOBSPic(path, i);
			OBS[i]->adjustSize();
			OBS[i]->setGeometry(1920, horline - dy * maxH * 0.1 - 2. * OBS[i]->height(), 2 * OBS[i]->width(), 2 * OBS[i]->height());
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

void DinoOL::on_actionRun_as_a_server_triggered()
{
	//qDebug() << getWebSource(QUrl("https://brownlzy.github.io/DinoOLver.txt"));
	if (getWebSource(QUrl("https://brownlzy.github.io/DinoOLver.txt")) != DINOVER)
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
