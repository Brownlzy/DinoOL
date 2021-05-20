#pragma once

#include <QDialog>
#include "ui_server.h"
#include <QtNetwork/QTcpServer>
#include <QtNetwork/QTcpSocket>
#include <QtWidgets/QMessageBox>

class Server : public QDialog
{
	Q_OBJECT

public:
	Server(QWidget *parent = Q_NULLPTR);
	~Server();

private:
	Ui::Server ui;

    QTcpServer* mp_TCPServer;
    QTcpSocket* mp_TCPSocket;
private slots:

    void OnBtnInitSocket();
    void OnBtnSendData();
    void ServerReadData();
    void ServerNewConnection();
    void sServerDisConnection();
};
