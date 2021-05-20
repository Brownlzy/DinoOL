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
	//��ȡ�ͻ�������
	mp_TCPSocket = mp_TCPServer->nextPendingConnection();
	if (!mp_TCPSocket)
	{
		QMessageBox::information(this, "QT����ͨ��", "δ��ȷ��ȡ�ͻ������ӣ�");
		return;
	}
	else
	{
		QMessageBox::information(this, "QT����ͨ��", "�ɹ����ܿͻ��˵�����");
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
		QMessageBox::information(this, "QT����ͨ��", "δ��ȷ��������");
		return;
	}
}

void Server::OnBtnInitSocket()
{
	mp_TCPServer = new QTcpServer();
    int port = ui.txtPort->text().toInt();
    if(!mp_TCPServer->listen(QHostAddress::Any, port))
    {
        QMessageBox::information(this, "QT����ͨ��", "�������˼���ʧ�ܣ�");
        return;
    }
    else
    {
        QMessageBox::information(this, "QT����ͨ��", "�����������ɹ���");
    }
    connect(mp_TCPServer, SIGNAL(newConnection()), this, SLOT(ServerNewConnection()));
}

void Server::OnBtnSendData()
{
	char sendMsgChar[1024] = {0};
    QString sendMsg = ui.txtToSend->toPlainText();
    if(sendMsg.isEmpty())
    {
        QMessageBox::information(this, "QT����ͨ��", "��������Ϊ�գ�����������");
        return;
    }
    strcpy_s(sendMsgChar, sendMsg.toStdString().c_str());
    if(mp_TCPSocket->isValid())
    {
        int sendRe = mp_TCPSocket->write(sendMsgChar, strlen(sendMsgChar));
        if( -1 == sendRe)
        {
            QMessageBox::information(this, "QT����ͨ��", "����˷�������ʧ�ܣ�");
        }
    }
    else
    {
        QMessageBox::information(this, "QT����ͨ��", "�׽�����Ч��");
    }
}

void Server::sServerDisConnection()
{
	QMessageBox::information(this, "QT����ͨ��", "��ͻ��˵����ӶϿ�");
    return;
}