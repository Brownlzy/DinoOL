#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_server.h"
#include <QtNetwork/QTcpServer>
#include <QtNetwork/QTcpSocket>
#include <QtWidgets/QMessageBox>
#include <QtCore/QSignalMapper>
#include <QtWidgets/QTableWidget>
#include <QtCore/QTimer>

class Server : public QMainWindow
{
	Q_OBJECT

public:
	Server(QWidget* parent = Q_NULLPTR);
	~Server();
	void refreshPlayer(int isAdd, int sockid, QString PID = "");
	void ProcessCMsg(QString);
	void SendTo(int PID, QString Msg);
	void SendTo(QString RID, QString Msg);


private:
	Ui::Server ui;

	QTcpServer* mp_TCPServer;
	QTcpSocket* mp_TCPSocket[100];
	QSignalMapper* signalMapper;
	int connection[100] = { 0 };
	int PlayerNum = 0;
	int RoomNum = 0;

private slots:

	void OnBtnInitSocket();
	void OnBtnSendData();
	void ServerReadData();
	void ServerNewConnection();
	void sServerDisConnection();
	void on_btnChangA_clicked();
};
