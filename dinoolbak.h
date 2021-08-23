#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_dinool.h"
#include <QtGui/QMovie>
#include <QtCore/QProcess>
#include <QtGui/QGuiApplication>
#include <QtCore/QSignalMapper>
//#include <QtCore/QCoreApplication>
#include <QtCore/QTimer>
#include <QtGui/QKeyEvent>
#include <QtCore/QPropertyAnimation>


class DinoOL : public QMainWindow
{
    Q_OBJECT

public:
    DinoOL(QWidget *parent = Q_NULLPTR);
    ~DinoOL();
    void StartGame(int step = 0);
    void CleanAM(QMovie* pitem);
    void CleanAM(QObject* pitem);
    void CleanAM(QPropertyAnimation* pitem);
    void KeyRec(char key);

public slots:
    void NetworkChk(QString);
    void keyPressEvent(QKeyEvent* e);
    void keyReleaseEvent(QKeyEvent* e);
    void StartStep1();
    void StartStep2();
    void printpos();
    void roadloop();
    void cloudloop();
    void KeyW();
    void Keyr();
    void KeyS();
    void KeyE();
    void KeyQ();

private slots:
    void on_actionRun_as_a_server_triggered();
    void on_actionExit_triggered();
    void on_actionConnect_a_server_triggered();
    void on_actionCreate_a_room_triggered();
    void on_actionJoin_a_room_triggered();

protected:
    virtual void resizeEvent(QResizeEvent* event) override;

private:
    Ui::DinoOLClass ui;
    int isStarted = 0;
    QTimer* ptimer = NULL;
    QTimer* pdtime = NULL;
    QTimer* proad = NULL;
    QTimer* ptcloud = NULL;
    QTimer* pttree = NULL;
    QTimer* ptbird = NULL;
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
    int height = 98;
    double vy = 542.2;
    double vx = 305.6;
    double G = 980.0;
    bool isDive = false;
    int isMove = 0;
    bool isJump = false;
    int cloudid = 0;
    double maxH;
};

int randNum(int Max);