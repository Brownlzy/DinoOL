#include "server.h"

Server::Server(QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);
	ui.txtPort->setText("30628");
	connect(ui.btnListen, SIGNAL(clicked()), this, SLOT(OnBtnInitSocket()));
	//connect(ui.m_sendData, SIGNAL(clicked()), this, SLOT(OnBtnSendData()));
}

Server::~Server()
{

}

void Server::ServerNewConnection()
{
	//获取客户端连接
	mp_TCPSocket = mp_TCPServer->nextPendingConnection();
	if (!mp_TCPSocket)
	{
		QMessageBox::information(this, "QT网络通信", "未正确获取客户端连接！");
		return;
	}
	else
	{
		QMessageBox::information(this, "QT网络通信", "成功接受客户端的连接");
		connect(mp_TCPSocket, SIGNAL(readyRead()), this, SLOT(ServerReadData()));
		connect(mp_TCPSocket, SIGNAL(disconnected()), this, SLOT(sServerDisConnection()));
	}
}

void Server::ServerReadData()
{
	char buffer[1024] = { 0 };
	mp_TCPSocket->read(buffer, 1024);
	if (strlen(buffer) > 0)
	{
		QString showNsg = buffer;
		ui.txtGet->append(showNsg);
	}
	else
	{
		QMessageBox::information(this, "QT网络通信", "未正确接收数据");
		return;
	}
}

void Server::OnBtnInitSocket()
{
	mp_TCPServer = new QTcpServer();
    int port = ui.txtPort->text().toInt();
    if(!mp_TCPServer->listen(QHostAddress::Any, port))
    {
        QMessageBox::information(this, "QT网络通信", "服务器端监听失败！");
        return;
    }
    else
    {
        QMessageBox::information(this, "QT网络通信", "服务器监听成功！");
    }
    connect(mp_TCPServer, SIGNAL(newConnection()), this, SLOT(ServerNewConnection()));
}

void Server::OnBtnSendData()
{
	char sendMsgChar[1024] = {0};
    QString sendMsg = ui.txtToSend->toPlainText();
    if(sendMsg.isEmpty())
    {
        QMessageBox::information(this, "QT网络通信", "发送数据为空，请输入数据");
        return;
    }
    strcpy_s(sendMsgChar, sendMsg.toStdString().c_str());
    if(mp_TCPSocket->isValid())
    {
        int sendRe = mp_TCPSocket->write(sendMsgChar, strlen(sendMsgChar));
        if( -1 == sendRe)
        {
            QMessageBox::information(this, "QT网络通信", "服务端发送数据失败！");
        }
    }
    else
    {
        QMessageBox::information(this, "QT网络通信", "套接字无效！");
    }
}

void Server::sServerDisConnection()
{
	QMessageBox::information(this, "QT网络通信", "与客户端的连接断开");
    return;
}