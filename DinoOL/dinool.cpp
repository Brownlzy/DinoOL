/*
* 2021年5月19日17点38分
* By：Brownlzy
*/

#include "dinool.h"

DinoOL::DinoOL(QWidget* parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	buildTime = __TIMESTAMP__;
	QString tmp = "DinoOL ";
#ifdef _DEBUG
	tmp += QString::fromStdString(DINOVER) + " By:Brownlzy ====DEBUG====DEBUG====DEBUG====";
#else
	tmp += QString::fromStdString(DINOVER) + " By:Brownlzy";
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
	//ui.whitebg->hide();
	ui.frame_4->hide();
	ui.lab_2->move(x - 1 - ui.lab_2->width(), ui.lab_2->y());
	ui.lab_3->move(x - 1 - ui.lab_3->width(), ui.lab_3->y());
	ui.lab_6->move(x - 1 - ui.lab_6->width(), ui.lab_6->y());
	connect(ui.label_2, SIGNAL(linkActivated(QString)), this, SLOT(NetworkChk(QString)));
	connect(ui.label_3, SIGNAL(linkActivated(QString)), this, SLOT(NetworkChk(QString)));
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

	tmp = DINOVER + QString::fromUtf8(" BT:") + buildTime;
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
	if (isPause) return;
	if (isStarted == 0 && (e->key() == Qt::Key_Space || e->key() == Qt::Key_W))
	{
		isStarted = -1;
		ui.labelchklcs->show();
		QString  tmp = getWebSource(QUrl("https://brownlzy.github.io/DinoOLUpdateInfo.txt"));
		tmp += "!<<<<<!";
		//DinoOL_OTA_Info!101<v1.0.2<102<1<20BAF4F9<296KB!
		//DinoOL_OTA_Info!低于此比较号强制升级 < 当前最新版本号 < 比较号(int) < 是否可快速升级(bool) < CRC32校验码 < 大小!(结束标记)
		if (tmp.split("!")[0] == "DinoOL_OTA_Info")
		{
			if (tmp.split("!")[1].split("<")[0].toInt() > DINOVERNUM)
			{
				QMessageBox::critical(this, tr("版本已过期！"), tr("请在菜单-帮助(H)-关于处升级最新版！\n当前版本:") + QString::fromUtf8(DINOVER) + tr("\n当前最新版:") + tmp.split("!")[1].split("<")[1]);
				isStarted = 0;
				P1->setDinoState(":/pic/gif/dino_jump");
				P1->setGeometry(0.2 * this->frameGeometry().width(), 0.2 * this->frameGeometry().height() - 83, 44, 130);
				P1->setScaledContents(true);
			}
			else
			{
				ui.labelchklcs->hide();
				StartGame();
			}
			ui.labelchklcs->hide();
			return;
		}
		else
		{
			QMessageBox::critical(this, tr("无效许可！"), tr("请确定已联网且安装了DinoOLServer组件,\n或向开发者索要最新版！\n当前版本:") + QString::fromUtf8(DINOVER));
			isStarted = 0;
			P1->setDinoState(":/pic/gif/dino_jump");
			P1->setGeometry(0.2 * this->frameGeometry().width(), 0.2 * this->frameGeometry().height() - 83, 44, 130);
			P1->setScaledContents(true);
		}
		ui.labelchklcs->hide();
		return;
	}
	ui.labelchklcs->hide();
	if (isStarted < 4) return;
	if (e->key() == Qt::Key_Up && !WebGame)
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
	if (isPause) return;
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
			WebGame = 1;
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
		ui.labelLog->setText(tr("QT网络通信向服务端发送数据失败！"));
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
		ui.labelLog->setText(tr("QT网络通信向服务端发送数据失败！"));
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
		ui.labelLog->setText(tr("QT网络通信向服务端发送数据失败！"));
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
		ui.labelLog->setText(tr("QT网络通信向服务端发送数据失败！"));
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
		ui.labelLog->setText(tr("QT网络通信向服务端发送数据失败！"));
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
	return 1;
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
		if (!isPause && P2->isOn && !P2->isShining && isTouched(OBS[i], &P2->labDino))
		{
			if (ui.labP1P2Life->text().split(':')[1].toInt() <= 1)
			{
				GamePause();
				mOBS[i]->stop();
				isPause = 1;
				P2->setDinoState(":/pic/png/dino_fail");
			}
			else { P2->shining(); }
			ui.lifeP2->setText("<html><head/><body><p><img src = "":/pic/png/" + QString::number(ui.labP1P2Life->text().split(':')[1].toInt() - 1) + """ width = ""36"" height = ""44"" / >< / p>< / body>< / html>");
			ui.labP1P2Life->setText(ui.labP1P2Life->text().split(':')[0] + ":" + QString::number(ui.labP1P2Life->text().split(':')[1].toInt() - 1));
		}
		if (!isPause && !P1->isShining && isTouched(OBS[i], &P1->labDino))
		{
			if (!P2->isOn && !WebGame)
			{
				if (R[1]->isOn == 1 && (R[0]->isFail == 1 || R[1]->isFail == 1) || R[1]->isOn == 0)
				{
					if (ui.labP1P2Life->text().split(':')[0].toInt() <= 1)
					{

						GamePause();
						mOBS[i]->stop();
						isPause = 1;
						P1->setDinoState(":/pic/png/dino_fail");
						//ui.lab_7->setText("碰到障碍" + QString::number(i));
					}
					else { P1->shining(); }
					ui.lifeP1->setText("<html><head/><body><p><img src = "":/pic/png/" + QString::number(ui.labP1P2Life->text().split(':')[0].toInt() - 1) + """ width = ""36"" height = ""44""/></p></body></html>");
					ui.labP1P2Life->setText(QString::number(ui.labP1P2Life->text().split(':')[0].toInt() - 1) + ":" + ui.labP1P2Life->text().split(':')[1]);
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
	if (P2->isOn) P2->Pause();
	P1->Pause();
	P2->Pause();
	R[0]->Pause();
	R[0]->Pause();
	ui.labelFail->show();
	ui.labelFail->raise();
	pAnimationRoad->stop();
}

void DinoOL::refreshScore(int t)
{
	if (isPause) return;
	//if (vx0 < 1000)vx0 = t / 100 * 100 + 305.6;
	for (int i = 0; i < 6 && t < 1000; i++)
	{
		if (dy[i] && vOBS[i]) vOBS[i] = 0 - vx0 - t;
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
	kind = randNum(1000) + 1000;		//计算下一次创建障碍物时间ms
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
		QTimer::singleShot(1000, this, SLOT(ProduceOBS()));				//1秒后重新调用自己
	}
	if (WebGame && ui.tableRoomer->item(0, 1)->text().toInt() != SPID) return;
	int dy = 0;
	kind = randNum(3);
	if (!kind)			//kind = 0时，生成鸟
	{
		dy = randNum(10) + 1;
	}
	else
	{
		kind = randNum(6) + 10;
	}
	if (ui.tableRoomer->item(0, 1)->text().toInt() == SPID)
		SendObstacle(kind, dy);
	else if (!WebGame)
		ProduceOBS(kind, dy);
}

void DinoOL::reStart(QString)
{
	SendDC();
	qApp->exit(-2);
}

void DinoOL::ProduceOBS(int kind, int dy)
{
	/*kind:dinool.qrc文件名
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
	if (Score.elapsed() == 0)
	{
		Score.start();
		ui.frmScore->show();
	}
	if (!WebGame)
	{
		ui.frmLlife->show();
		if (!P2->isOn) ui.frmLlife->setGeometry(0, 0, 71, 61);
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
	ui.labelchklcs->show();
	QString  tmp = getWebSource(QUrl("https://brownlzy.github.io/DinoOLUpdateInfo.txt"));
	if (tmp.split("!")[0] == "DinoOL_OTA_Info")
	{
		if (tmp.split("!")[1].split("<")[0].toInt() > DINOVERNUM)
		{
			QMessageBox::critical(this, tr("版本已过期！"), tr("请在菜单-帮助(H)-关于处升级最新版！\n当前版本:") + QString::fromUtf8(DINOVER) + tr("\n当前最新版:") + tmp.split("!")[1].split("<")[1]);
			isStarted = 0;
			P1->setDinoState(":/pic/gif/dino_jump");
			P1->setGeometry(0.2 * this->frameGeometry().width(), 0.2 * this->frameGeometry().height() - 83, 44, 130);
			P1->setScaledContents(true);
		}
		else
		{
			ui.labelchklcs->hide();
			qApp->exit(-1);
		}
		ui.labelchklcs->hide();
		return;
	}
	else
	{
		QMessageBox::critical(this, tr("无效许可！"), tr("请确定已联网且安装了DinoOLServer组件,\n或向开发者索要最新版！\n当前版本:") + QString::fromUtf8(DINOVER));
		isStarted = 0;
		P1->setDinoState(":/pic/gif/dino_jump");
		P1->setGeometry(0.2 * this->frameGeometry().width(), 0.2 * this->frameGeometry().height() - 83, 44, 130);
		P1->setScaledContents(true);
	}
	ui.labelchklcs->hide();
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
	ui.frame_4->raise();
	if (ui.btnCon->text() == tr("已连接")) QTimer::singleShot(3000, ui.frame, SLOT(hide()));
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

void DinoOL::on_actionRestart_triggered()
{
	reStart("");
}

void DinoOL::on_action_2_triggered()
{
	About frmabout;
	frmabout.setWindowIcon(QIcon(":/pic/icon/DinoOL"));
	QString tmp = "DinoOL ";
	tmp += DINOVER;
	frmabout.ui.label_2->setText(tmp);
	tmp = "Build Time: ";
	tmp += __TIMESTAMP__;
	frmabout.ui.label_3->setText(tmp);
	frmabout.setModal(true);
	frmabout.setWindowFlags(Qt::WindowCloseButtonHint);
	//tmp = getWebSource(QUrl("https://brownlzy.github.io/DinoOLver.txt"));
	tmp = getWebSource(QUrl("https://brownlzy.github.io/DinoOLUpdateInfo.txt"));
	//DinoOL_OTA_Info!101<v1.0.2<102<1<20BAF4F9<296KB!
	//DinoOL_OTA_Info!低于此比较号强制升级 < 当前最新版本号 < 比较号(int) < 是否可快速升级(bool) < CRC32校验码 < 大小!(结束标记)
	tmp += "!<<<<<!";//防止下标越界;
	qDebug() << tmp;
	if (tmp.split("!")[0] == "DinoOL_OTA_Info")
	{
		if (tmp.split("!")[1].split("<")[2].toInt() > DINOVERNUM)
		{
			if (tmp.split("!")[1].split("<")[3].toInt() == 1)
			{
				frmabout.ui.pushButton_2->setEnabled(true);
				QString otaInfo = tmp.split("!")[1].split("<")[1] + "_";
				otaInfo += tmp.split("!")[1].split("<")[5];
				frmabout.ui.pushButton_2->setToolTip(otaInfo);
				//frmabout.ui.pushButton_2->setWhatsThis(tmp.split("!")[1]);
				frmabout.strCRC32 = tmp.split("!")[1].split("<")[4];
			}
			else
			{
				frmabout.ui.pushButton_2->setText(tr("请手动更新"));
				QString otaInfo = tmp.split("!")[1].split("<")[1] + "_";
				otaInfo += tmp.split("!")[1].split("<")[5];
				frmabout.ui.pushButton_2->setToolTip(otaInfo);
				frmabout.setWindowTitle(tr("DinoOL -有可用更新-"));
			}
		}
		else
		{
			frmabout.setFixedSize(291, 141);
		}
	}
	else
	{
		frmabout.setWindowTitle(tr("DinoOL -未授权版本-"));
	}
	frmabout.show();
	if (frmabout.exec());
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
		ui.labelLog->setText(tr("QT网络通信连接服务端失败！"));
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
		ui.labelLog->setText(tr("QT网络通信向服务端发送数据失败！"));
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
		ui.labelLog->setText(tr("QT网络通信向服务端发送数据失败！"));
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
		ui.labelLog->setText(tr("QT网络通信向服务端发送数据失败！"));
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
		ui.labelLog->setText(tr("QT网络通信接收服务端数据失败！"));
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

	return QTextCodec::codecForHtml(codeContent)->toUnicode(codeContent);
}
