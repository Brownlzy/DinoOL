#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_dinool.h"
#include <QtGui/QMovie>
#include <QtCore/QProcess>
#include <QtGui/QGuiApplication>
#include <QtCore/QSignalMapper>
#include <QtCore/QTimer>
#include <QtGui/QKeyEvent>
#include <QtCore/QPropertyAnimation>
#include <QtNetwork/QTcpSocket>
#include <QtWidgets/QMessageBox>
#include <QtWidgets/QTableWidget>


class DinoOL : public QMainWindow
{
	Q_OBJECT

public:
	DinoOL(QWidget* parent = Q_NULLPTR);
	~DinoOL();
	void StartGame(int step = 0);
	void CleanAM(QMovie* pitem);
	void CleanAM(QObject* pitem);
	void CleanAM(QPropertyAnimation* pitem);
	bool isOnGround();
	bool isOnGround(int id);
	void setDinoState(QString);
	void setDinoState(QString pic, int id);
	void ProcessSMsg(QString);
	void SendPOS(int dx, int dy, int key, bool isPress);
	void SendReady();
	void RKey(int, int, int);

public slots:
	void printDino();
	void NetworkChk(QString);
	void keyPressEvent(QKeyEvent* e);
	void keyReleaseEvent(QKeyEvent* e);
	void StartStep1();
	void StartStep2();
	void printpos();
	void roadloop();
	void cloudloop();
	void printDinoL();
	void printDino1();
	void printDino2();

private slots:
	void on_actionRun_as_a_server_triggered();
	void on_actionExit_triggered();
	void on_actionConnect_a_server_triggered();
	void on_actionCreate_a_room_triggered();
	void on_actionJoin_a_room_triggered();
	void on_actionDebug_a_room_triggered();
	void on_btnCon_clicked();
	void on_btnCreRoom_clicked();
	void on_btnSend_clicked();
	void on_btnJoin_clicked();
	void on_btnHD_clicked();
	void ClientRecvData();
	void sDisConnection();

protected:
	virtual void resizeEvent(QResizeEvent* event) override;

private:
	Ui::DinoOLClass ui;
	QTcpSocket* mp_clientSocket;
	int isStarted = 0;
	int SPID = 100;
	int WebGame = 0;
	QTimer* ptimer = NULL;
	QTimer* pdtime = NULL;
	QTimer* proad = NULL;
	QTimer* ptcloud = NULL;
	QTimer* pttree = NULL;
	QTimer* ptbird = NULL;
	QTimer* ptdino = NULL;
	QMovie* movie_dino = NULL;
	QMovie* movie_cloud = NULL;
	QMovie* movie_road = NULL;
	QPropertyAnimation* pAnimation1 = NULL;
	QPropertyAnimation* pAnimation2 = NULL;
	QPropertyAnimation* pAnimationRoad = NULL;
	QPropertyAnimation* pAnimationCloud = NULL;
	QPropertyAnimation* pJumpDive = NULL;
	QPropertyAnimation* pLeftRight = NULL;
	QLabel* labBird = NULL;
	QLabel* labTree = NULL;
	QLabel* labCloud = NULL;
	int cloudid = 0;
	int height = 98;
	double vy0 = 542.2;
	double vx0 = 305.6;
	double G = 980.0;
	double horline = 0;
	double vy = 0;
	double vx = 0;
	double x, y;
	int tms = 10;
	bool isDive = false;
	int isMove = 0;
	bool isJump = false;
	double maxH;

	QTimer* Rptdino[2] = { NULL };
	QMovie* Rmovie_dino[2] = { NULL };
	int f[2] = { 0 };
	double Rvy[2] = { 0 };
	double Rvx[2] = { 0 };
	bool RisDive[2] = { 0 };
	int RisMove[2] = { 0 };
	bool RisJump[2] = { 0 };
	int fchk[2] = { 0 };

	QTimer* Lptdino = NULL;
	QMovie* Lmovie_dino = NULL;
	int LisOn = 0;
	double Lvy = 0;
	double Lvx = 0;
	bool LisDive = 0;
	int LisMove = 0;
	bool LisJump = 0;
};

int randNum(int Max);
