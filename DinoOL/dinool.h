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

#include "ui_dinool.h"
#include "dino.h"
#include "about.h"

#define DINOVER "v1.0-beta.4"

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
	void setOBSPic(QString pic, int id);
	void ProcessSMsg(QString);
	void SendPOS(int dx, int dy, int key, bool isPress);
	void SendObstacle(int kind, int dy = 0);
	void SendCL(int);
	int isAllReady();
	void RKey(int, int, int);

public slots:
	void NetworkChk(QString);
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

private slots:
	void on_actionRun_as_a_server_triggered();
	void on_actionExit_triggered();
	void on_actionConnect_a_server_triggered();
	void on_actionDebug_triggered();
	void on_actionRestart_triggered();
	void on_action_2_triggered();
	void on_btnCon_clicked();
	void on_btnCreRoom_clicked();
	void on_btnSend_clicked();
	void on_btnJoin_clicked();
	void on_btnHD_clicked();
	void ClientRecvData();
	void sDisConnection();
	void ProduceOBS();
	void reStart(QString);

protected:
	virtual void resizeEvent(QResizeEvent* event) override;

private:
	Ui::DinoOLClass ui;
	QTcpSocket* mp_clientSocket;
	QString buildTime;
	int isStarted = 0;
	int isPause = 0;
	int SPID = 100;
	int WebGame = 0;
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
	double vx0 = 305.6;
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
};

int randNum(int Max);
QString getWebSource(QUrl url);
