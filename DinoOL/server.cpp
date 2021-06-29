/*
* 2021年5月19日17点38分
* By：Brownlzy
*/

#include "server.h"

Server::Server(QWidget* parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	this->setWindowIcon(QIcon(":/pic/icon/Server"));
	ui.txtPort->setText("30628");
#ifndef _DEBUG
	ui.txtLog->setGeometry(0, 249, 271, 251);
	ui.txtGet->hide();
#endif // !_DEBUG
	connect(ui.btnListen, SIGNAL(clicked()), this, SLOT(OnBtnInitSocket()));
	connect(ui.btnSend, SIGNAL(clicked()), this, SLOT(OnBtnSendData()));
	//t.setInterval(3000);
	//connect(&t, SIGNAL(timeout()), this, SLOT(ChkCon()));
	//t.start();
	//signalMapper = new QSignalMapper(this);
	//connect(signalMapper, SIGNAL(mapped(int)), this, SLOT(sServerDisConnection(int)));
}

Server::~Server()
{

}

void Server::refreshPlayer(int isAdd, int sockid, QString PID)
{
	QTableWidgetItem* item;
	if (isAdd)
	{
		item = new QTableWidgetItem(QString::number(sockid));
		ui.tablePlayer->setItem(sockid, 0, item);
		item = new QTableWidgetItem(PID);
		ui.tablePlayer->setItem(sockid, 1, item);
		item = new QTableWidgetItem("Connected");
		ui.tablePlayer->setItem(sockid, 2, item);
		item = new QTableWidgetItem("Spare");
		ui.tablePlayer->setItem(sockid, 3, item);
	}
	else
	{
		for (int i = 0; i < 4; i++)
		{
			item = new QTableWidgetItem("");
			ui.tablePlayer->item(sockid, i)->setText("");
		}
		//在调用此函数前已执行
		//PlayerNum--;
		//ui.label_4->setText(QString::number(PlayerNum));
		for (int i = 0; i < ui.tableRoom->rowCount(); i++)
		{
			for (int j = 4; j <= 6; j++)
			{
				if (ui.tableRoom->item(i, j)->text() == QString::number(100 + sockid))
				{
					QString sendtmp = "ROOM$0$$";
					if (j == 4)
					{
						for (int k = 5; k < 4 + ui.tableRoom->item(i, 2)->text().toInt(); k++)
						{
							try
							{
								SendTo(ui.tableRoom->item(i, k)->text().toInt(), sendtmp);
							}
							catch (...)
							{

							}
						}
						ui.tableRoom->removeRow(i);
						RoomNum--;
						ui.label_3->setText(QString::number(RoomNum));
					}
					else
					{
						SendTo(PID, sendtmp);
						sendtmp = "ADDU$-1$" + QString::number(sockid + 100) + "$$$";
						for (int k = 4; k < 4 + ui.tableRoom->item(i, 2)->text().toInt(); k++)
						{
							if (k != j)
							{
								try
								{
									SendTo(ui.tableRoom->item(i, k)->text().toInt(), sendtmp);
								}
								catch (...)
								{

								}
							}
						}
						ui.tableRoom->item(i, j)->setText("");
						ui.tableRoom->item(i, 2)->setText(QString::number(ui.tableRoom->item(i, 2)->text().toInt() - 1));
					}
					return;
				}
			}
		}
	}
}

void Server::ProcessCMsg(QString msg)
{
	//PID$fun$argu1$argu2$$$$
	int PID = msg.split('$')[0].toInt();
	QString fun = msg.split('$')[1];
	if (fun == "POS")
	{
		//PID$POS$RoomID$...$$$
		QString tmp;
		tmp = "POS$" + msg.split('$')[0] + "$" + msg.split('$')[3] + "$$$";
		SendTo(msg.split('$')[2], tmp);
	}
	else if (fun == "OBS")
	{
		//PID$OBS$RoomID$...$$$
		QString tmp;
		tmp = "OBS$" + msg.split('$')[0] + "$" + msg.split('$')[3] + "$$$";
		SendTo(msg.split('$')[2], tmp);
	}
	else if (fun == "READY")
	{
		QString room = msg.split('$')[2];
		for (int i = 0; i < ui.tableRoom->rowCount(); i++)
		{
			if (ui.tableRoom->item(i, 0)->text() == room)
			{
				int ri[3];
				QString newready = "";
				for (int j = 0; j < 3; j++)
				{
					if (ui.tableRoom->item(i, 4 + j)->text().toInt() == PID)
						ri[j] = 1;
					else
						ri[j] = (ui.tableRoom->item(i, 7)->text().split('-')[j] == tr("是") ? 1 : 0);
				}
				newready += (ri[0] ? tr("是") : tr("否"));
				newready += "-";
				newready += (ri[1] ? tr("是") : tr("否"));
				newready += "-";
				newready += (ri[2] ? tr("是") : tr("否"));
				ui.tableRoom->item(i, 7)->setText(newready);
				SendTo(room, "READY$2$" + newready + "$$$");
				if (ui.tableRoom->item(i, 7)->text().split('-')[0] == tr("是")
					&& (!(ui.tableRoom->item(i, 5)->text() != "") || ui.tableRoom->item(i, 7)->text().split('-')[1] == tr("是"))
					&& (!(ui.tableRoom->item(i, 6)->text() != "") || ui.tableRoom->item(i, 7)->text().split('-')[2] == tr("是")))
				{
					ui.tableRoom->item(i, 3)->setText(tr("游戏中..."));
				}
				break;
			}
		}
	}
	else if (fun == "ROOM")
	{
		//PID$ROOM$1$ROOMID$$
		int isAdd = msg.split('$')[2].toInt();
		int row = ui.tableRoom->rowCount();
		if (isAdd)
		{
			for (int i = 0; i < row; i++)
			{
				if (ui.tableRoom->item(i, 0)->text() == msg.split('$')[3] || ui.tableRoom->item(i, 1)->text() == QString::number(PID))
				{
					SendTo(PID, tr("ROOM$0$$房间已存在$$")); //房间已存在
					return;
				}
			}
			ui.tableRoom->insertRow(row);
			ui.tableRoom->setItem(row, 0, new QTableWidgetItem(msg.split('$')[3]));
			ui.tableRoom->setItem(row, 1, new QTableWidgetItem(QString::number(PID)));
			ui.tableRoom->setItem(row, 2, new QTableWidgetItem("1"));
			ui.tableRoom->setItem(row, 3, new QTableWidgetItem(tr("等待玩家加入...")));
			ui.tableRoom->setItem(row, 4, new QTableWidgetItem(QString::number(PID)));
			ui.tableRoom->setItem(row, 5, new QTableWidgetItem(""));
			ui.tableRoom->setItem(row, 6, new QTableWidgetItem(""));
			ui.tableRoom->setItem(row, 7, new QTableWidgetItem(tr("否-否-否")));

			ui.tablePlayer->item(PID % 100, 3)->setText("ROOM:" + msg.split('$')[3]);

			RoomNum++;
			ui.label_3->setText(QString::number(RoomNum));
			SendTo(PID, "ROOM$1$" + msg.split('$')[3] + tr("$创建成功$$"));//创建成功
		}
		else
		{
			for (int i = 0; i < row; i++)
			{
				if (ui.tableRoom->item(i, 0)->text() == msg.split('$')[3] && ui.tableRoom->item(i, 1)->text() == QString::number(PID))
				{
					SendTo(ui.tableRoom->item(i, 5)->text().toInt(), tr("ROOM$-1$$房间已关闭$$"));
					SendTo(ui.tableRoom->item(i, 6)->text().toInt(), tr("ROOM$-1$$房间已关闭$$"));
					ui.tableRoom->removeRow(i);
					return;
				}
			}
			SendTo(PID, tr("ROOM$-2$删除失败$$$"));//删除失败
		}
	}
	else if (fun == "JOIN")
	{
		//PID$JOIN$1$RoomID$$$
		int isIn = msg.split('$')[2].toInt();
		int row = ui.tableRoom->rowCount();
		if (isIn)
		{
			if (isIn == 1)
			{
				for (int i = 0; i < row; i++)
				{
					if (ui.tableRoom->item(i, 0)->text() == msg.split('$')[3])
					{
						if (ui.tableRoom->item(i, 2)->text().toInt() < 3 && ui.tableRoom->item(i, 7)->text().startsWith("否"))
						{
							if (ui.tablePlayer->item(PID % 100, 3)->text() == "Spare")
							{
								for (int j = 4, flag = 0; j < 7; j++)
								{
									if (ui.tableRoom->item(i, j)->text() == "")
									{
										ui.tableRoom->item(i, j)->setText(QString::number(PID));
										break;
									}
								}
								ui.tableRoom->item(i, 2)->setText(QString::number(ui.tableRoom->item(i, 2)->text().toInt() + 1));
								ui.tablePlayer->item(PID % 100, 3)->setText("ROOM:" + msg.split('$')[3]);
								QString RoomMem;
								RoomMem = "JOIN$2$" + msg.split('$')[3] + tr("$是#") + ui.tableRoom->item(i, 4)->text() + tr("#Grey#@");
								for (int j = 5; j < 7; j++)
								{
									if (ui.tableRoom->item(i, j)->text() != "")
										RoomMem += "#" + ui.tableRoom->item(i, j)->text() + tr("#Grey#@");
									else
										RoomMem += "###@";
								}
								RoomMem += "###@###@$" + ui.tableRoom->item(i, 7)->text() + "$$$";
								SendTo(ui.tableRoom->item(i, 0)->text(), RoomMem);
								SendTo(PID, tr("JOIN$1$加入成功$$$")); //1表示加入成功
							}
							else
								SendTo(PID, tr("JOIN$-2$已在房间中$$$")); //-2表示已在房间中
						}
						else
							SendTo(PID, tr("JOIN$0$房间已满$$$")); //0表示房间已满
						return;
					}
				}
				SendTo(PID, tr("JOIN$-1$无房间$$$")); //-1表示无房间
			}
			else if (isIn == 2)	//随机加入房间（返回一个未满的房间ID让客户端加入）
			{
				for (int i = 0; i < row; i++)
				{
					if (ui.tableRoom->item(i, 0)->text().startsWith("R") && ui.tableRoom->item(i, 2)->text().toInt() < 3 && ui.tableRoom->item(i, 7)->text().startsWith("否"))
					{
						if (ui.tablePlayer->item(PID % 100, 3)->text() == "Spare")
						{
							for (int j = 4; j < 7; j++)
							{
								if (ui.tableRoom->item(i, j)->text() == "")
								{
									ui.tableRoom->item(i, j)->setText(QString::number(PID));
									break;
								}
							}
							ui.tableRoom->item(i, 2)->setText(QString::number(ui.tableRoom->item(i, 2)->text().toInt() + 1));
							ui.tablePlayer->item(PID % 100, 3)->setText("ROOM:" + ui.tableRoom->item(i, 0)->text());
							QString RoomMem;
							RoomMem = "JOIN$2$" + ui.tableRoom->item(i, 0)->text() + tr("$是#") + ui.tableRoom->item(i, 4)->text() + tr("#Grey#@");
							for (int j = 5; j < 7; j++)
							{
								if (ui.tableRoom->item(i, j)->text() != "")
									RoomMem += "#" + ui.tableRoom->item(i, j)->text() + tr("#Grey#@");
								else
									RoomMem += "###@";
							}
							RoomMem += "###@###@$" + ui.tableRoom->item(i, 7)->text() + "$$$";
							SendTo(ui.tableRoom->item(i, 0)->text(), RoomMem);
							SendTo(PID, tr("JOIN$1$加入成功$$$")); //1表示加入成功
						}
						else
							SendTo(PID, tr("JOIN$-2$已在房间中$$$")); //-2表示已在房间中
						return;
					}
				}
				SendTo(PID, tr("JOIN$-1$无空闲开放房间$$$")); //-1表示无房间
			}
		}
		else
		{
			for (int i = 0; i < row; i++)
			{
				for (int j = 4; j <= 6; j++)
				{
					if (ui.tableRoom->item(i, j)->text() == msg.split('$')[0])
					{
						QString sendtmp = "ROOM$0$$$$";
						if (j == 4)
						{
							for (int k = 4; k < 4 + ui.tableRoom->item(i, 2)->text().toInt(); k++)
							{
								SendTo(ui.tableRoom->item(i, k)->text().toInt(), sendtmp);
								ui.tablePlayer->item(ui.tableRoom->item(i, k)->text().toInt() % 100, 3)->setText("Spare");
							}
							ui.tableRoom->removeRow(i);
							RoomNum--;
							ui.label_3->setText(QString::number(RoomNum));
							ui.tablePlayer->item(PID % 100, 3)->setText("Spare");
						}
						else
						{
							SendTo(PID, sendtmp);
							QString RoomMem;
							RoomMem = "JOIN$2$" + ui.tableRoom->item(i, 0)->text() + tr("$是#") + ui.tableRoom->item(i, 4)->text() + tr("#Grey#@");
							ui.tableRoom->item(i, j)->setText("");
							for (int j = 5; j < 7; j++)
							{
								if (ui.tableRoom->item(i, j)->text() != "")
									RoomMem += "#" + ui.tableRoom->item(i, j)->text() + tr("#Grey#@");
								else
									RoomMem += "###@";
							}
							RoomMem += "###@###@$" + ui.tableRoom->item(i, 7)->text() + "$$$";
							SendTo(ui.tableRoom->item(i, 0)->text(), RoomMem);
							ui.tableRoom->item(i, 2)->setText(QString::number(ui.tableRoom->item(i, 2)->text().toInt() - 1));
							ui.tablePlayer->item(PID % 100, 3)->setText("Spare");
						}
						return;
					}
				}
			}
		}
	}
	else if (fun == "CGLIFE")
	{//PID$CGLIFE$ROOMID$lifenum
		QString tmp;
		tmp = "CGLIFE$" + msg.split('$')[0] + "$" + msg.split('$')[3] + "$$$";
		SendTo(msg.split('$')[2], tmp);
	}
	else if (fun == "DISCONNECT")
	{
		sServerDisConnection();
	}
}

void Server::SendTo(int PID, QString Msg)
{
	if (PID == 100)
	{
		ui.txtToSend->setPlainText(Msg);
		OnBtnSendData();
	}
	else
	{
		char sendMsgChar[1024] = { 0 };
		QString sendMsg = Msg;
		if (sendMsg.isEmpty())
		{
			ui.txtLog->append("Cannot send empty data");
			return;
		}
		strcpy_s(sendMsgChar, sendMsg.toStdString().c_str());
		if (connection[PID % 100])
		{
			if (mp_TCPSocket[PID % 100]->isValid())
			{
				int sendRe = mp_TCPSocket[PID % 100]->write(sendMsgChar, strlen(sendMsgChar));
				if (-1 == sendRe)
				{
					ui.txtLog->append(QString::number(PID % 100) + "Send data Error");
					refreshPlayer(0, PID % 100);
					connection[PID % 100] = 0;
					PlayerNum--;
				}
			}
			else
			{
				ui.txtLog->append(QString::number(PID % 100) + "invalid socket");
			}
		}
	}
}

void Server::SendTo(QString RID, QString Msg)
{
	for (int i = 0; i < ui.tableRoom->rowCount(); i++)
	{
		if (ui.tableRoom->item(i, 0)->text() == RID)
		{
			for (int j = 4; j < 7; j++)
			{
				int PID = ui.tableRoom->item(i, j)->text().toInt();
				SendTo(PID, Msg);
			}
			break;
		}
	}
}

void Server::ServerNewConnection()
{
	//获取客户端连接
	int sockid = 0;
	for (int i = 1; i < 100; i++)
	{
		if (connection[i] == 0)
		{
			sockid = i;
			break;
		}
	}
	mp_TCPSocket[sockid] = mp_TCPServer->nextPendingConnection();
	if (!mp_TCPSocket[sockid])
	{
		ui.txtLog->append(QString::number(sockid) + "Unable to get connection.");
		return;
	}
	else
	{
		ui.txtLog->append(QString::number(sockid) + "accept connection successful");
		connect(mp_TCPSocket[sockid], SIGNAL(readyRead()), this, SLOT(ServerReadData()));
		connect(mp_TCPSocket[sockid], SIGNAL(disconnected()), this, SLOT(sServerDisConnection()));
		if (sockid == 0)
		{
			connection[0] = 1;
			SendTo(100, tr("PID=100$已达到最大连接数！请稍后再试。"));
		}
		else
		{
			connection[sockid] = 1;
			refreshPlayer(1, sockid, QString::number(sockid + 100));
			SendTo(100 + sockid, "PID$" + QString::number(100 + sockid) + tr("$连接成功。"));
			PlayerNum++;
			ui.label_4->setText(QString::number(PlayerNum));
			QTimer::singleShot(1000, ui.btnChangA, SLOT(clicked));
		}
	}
}

void Server::ServerReadData()
{
	char buffer[1024] = { 0 };
	for (int i = 0; i < 99; i++)
	{
		if (connection[i])
			mp_TCPSocket[i]->read(buffer, 1024);
	}
	if (strlen(buffer) > 0)
	{
		QString showNsg = buffer;
		ui.txtGet->append(showNsg);
		ProcessCMsg(showNsg);
	}
	else
	{
		ui.txtLog->append("Error when receving");
		return;
	}
}

void Server::OnBtnInitSocket()
{
	if (ui.txtLog->toPlainText() != "") return;
	mp_TCPServer = new QTcpServer();
	int port = ui.txtPort->text().toInt();
	if (!mp_TCPServer->listen(QHostAddress::Any, port))
	{
		ui.txtLog->append("Server:Cannot listen on pork");
		return;
	}
	else
	{
		ui.txtLog->append("Server:Listening");
	}
	connect(mp_TCPServer, SIGNAL(newConnection()), this, SLOT(ServerNewConnection()));
}

void Server::OnBtnSendData()
{
	char sendMsgChar[1024] = { 0 };
	QString sendMsg = ui.txtToSend->toPlainText();
	if (sendMsg.isEmpty())
	{
		ui.txtLog->append("Cannot send empty data");
		return;
	}
	strcpy_s(sendMsgChar, sendMsg.toStdString().c_str());
	for (int i = 0; i < 99; i++)
	{
		if (connection[i])
		{
			if (mp_TCPSocket[i]->isValid())
			{
				int sendRe = mp_TCPSocket[i]->write(sendMsgChar, strlen(sendMsgChar));
				if (-1 == sendRe)
				{
					ui.txtLog->append("Sock[" + QString::number(i) + "]:Send data Error");
					refreshPlayer(0, i);
					connection[i] = 0;
					PlayerNum--;
					ui.label_4->setText(QString::number(PlayerNum));
				}
			}
			else
			{
				ui.txtLog->append(QString::number(i) + "invalid socket");
			}
		}
	}

}

void Server::sServerDisConnection()
{
	ui.txtLog->append("disconnect with client");
	ui.txtToSend->setPlainText("CTEST$$$$$$");
	OnBtnSendData();
	return;
}
void Server::ChkCon()
{
	ui.txtToSend->setPlainText("CTEST$$$$$$");
	OnBtnSendData();
	return;
}
void Server::on_btnChangA_clicked()
{
	QString tmp;
	tmp = "ARGU$" + ui.lineEdit->text() + "$" + ui.lineEdit_2->text() + "$" + ui.lineEdit_3->text() + "$$$";
	ui.txtToSend->setPlainText(tmp);
	OnBtnSendData();
}
