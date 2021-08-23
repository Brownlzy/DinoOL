/*
* 2021年5月19日17点38分
* By：Brownlzy
*/

#pragma once

#include <QtWidgets/QMainWindow>
#include <QtGui/QMovie>
#include <QtCore/QProcess>
#include <QtGui/QGuiApplication>
#include <QtCore/QSignalMapper>
#include <QtCore/QTimer>
#include <QtCore/QTime>
#include <QtGui/QKeyEvent>
#include <QtCore/QPropertyAnimation>
#include <QtNetwork/QTcpSocket>
#include <QtWidgets/QMessageBox>
#include <QtWidgets/QTableWidget>
#include <QtNetwork/QNetworkAccessManager>
#include <QtCore/QTextCodec>
#include <QtNetwork/QNetworkReply>
#include <QtNetwork/QSslConfiguration>
#include <QtWidgets/QWhatsThis>
#include <QtCore/QString>
#include <QInputDialog>
#include <QDesktopServices>
#include <QPainter>
#include <fstream>

#include "ui_dinool.h"
#include "dino.h"
#include "loading.h"
#include "about.h"

class MoveLabel :public QLabel
{
	Q_OBJECT
private:
	bool isPressed = false;
public:
	MoveLabel(QWidget* aa = 0);

	void mousePressEvent(QMouseEvent*);
	void mouseReleaseEvent(QMouseEvent*);
	void mouseMoveEvent(QMouseEvent*);

signals:
	void ActionCurrentPos(const bool&, const QPoint& point = QPoint(0, 0));
	//————————————————
	//	版权声明：本文为CSDN博主「automoblie0」的原创文章，遵循CC 4.0 BY - SA版权协议，转载请附上原文出处链接及本声明。
	//	原文链接：https ://blog.csdn.net/automoblie0/article/details/100014812
};

class DinoOL : public QMainWindow
{
	Q_OBJECT

public:
	DinoOL(QWidget* parent = Q_NULLPTR);
	~DinoOL();
	void StartGame(int step = 0);
	void CleanAM(QMovie* pitem);
	void CleanAM(QPropertyAnimation* pitem);
	void setOBSPic(QString pic, int id);
	void ProcessSMsg(QString);
	void SendPOS(int dx, int dy, int key, bool isPress);
	void SendObstacle(int kind, int dy = 0);
	void SendCL(int);
	int isAllReady();
	void RKey(int, int, int);
	int readDataFile();
	int writeDataFile();
	int Initialize();
	void CfgSet(int isRember, int x = -1, int y = -1, int w = -1, int h = -1, int isMax = 0, int isMoon = 0);
	void Transparent();
	void Focus();

public slots:
	void NetworkChk(QString);
	void Menu(QString);
	void keyPressEvent(QKeyEvent* e);
	void keyReleaseEvent(QKeyEvent* e);
	void StartStep1();
	void StartStep2();
	void printpos();
	void roadloop();
	void cloudloop();
	void ProduceOBS(int kind, int dy);
	void printOBS();
	void GamePause();
	void refreshScore(int);
	int isTouched(QLabel*, QLabel*);
	void SendReady();
	void SendDC();
	void resizeDinoOL();
	void ChangeColor(QString);
	void SunMoon(int istoMoon = -1);
	void paintEvent(QPaintEvent*);
	void ActionCurrentPos(const bool& b, const QPoint& point);

private slots:
	void toServer();
	void ExitApp();
	void ConnectServer();
	void DebugInfo();
	void NewOne();
	void RestartApp();
	void ShowAbout();
	void Cheat();
	void Offline();
	void PayQR();
	void Help();
	void aTransparent();
	void btnCon();
	void btnWhatsThis();
	void btnRandRoom();
	void btnCreRoom();
	void btnExitRoom();
	void btnSend();
	void btnJoin();
	void btnHD();
	void chkCheat();
	void ClientRecvData();
	void sDisConnection();
	void ProduceOBS();
	void reStart(QString);

protected:
	virtual void resizeEvent(QResizeEvent* event) override;
	void closeEvent(QCloseEvent* event);

public:

	Ui::DinoOLClass ui;

private:
	About* pAbout;
	Update* pUpdate;

	QTcpSocket* mp_clientSocket;
	QString buildTime;
	int isStarted = 0;
	int isPause = 0;
	int SPID = 100;
	int WebGame = 0;
	int flag4life = 0;
	QTime t;
	QTimer* pdtime = NULL;
	QTimer* proad = NULL;
	QTimer* ptcloud = NULL;
	QMovie* movie_cloud = NULL;
	QMovie* movie_road = NULL;
	QPropertyAnimation* pAnimation1 = NULL;
	QPropertyAnimation* pAnimationRoad = NULL;
	QPropertyAnimation* pAnimationCloud = NULL;
	QLabel* labCloud = NULL;
	int cloudid = 0;
	int height = 98;
	double vy0 = 542.2;
	double vx0 = 705.6;
	double G = 980.0;
	double horline = 0;
	double x, y;
	int tms = 10;
	double maxH;

	QTimer* ptOBS = NULL;
	QTime tobs;
	QLabel* OBS[7] = { NULL };
	QMovie* mOBS[7] = { NULL };
	double vOBS[7] = { 0 };
	int dy[7] = { 0 };

	Dino* P1 = { NULL };
	Dino* P2 = { NULL };
	Dino** R = { NULL };

	QTime Score;

	QMovie S2M;
	int isSun = 1;

	int isWforP1 = 1;

	QPoint cursorCurrentPointF;
	bool cursouPresss, actionActive;
	MoveLabel* lab;
};

int randNum(int Max);
