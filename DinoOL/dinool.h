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
    void CleanAM(int);
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
    QTimer* ptimer = nullptr;
    QTimer* pdtime = nullptr;
    QTimer* proad = nullptr;
    QTimer* ptcloud = nullptr;
    QTimer* pttree = nullptr;
    QTimer* ptbird = nullptr;
    float G = 9.8;
    double maxH;
    QMovie* movie = nullptr;
    QPropertyAnimation* pScaleAnimation1 = nullptr;
    QPropertyAnimation* pScaleAnimation2 = nullptr;
    QPropertyAnimation* pAnimationRoad = nullptr;
    QPropertyAnimation* pAnimationCloud = nullptr;
    double v = 300;
    bool isDive = false;
    QLabel* labBird = nullptr;
    QLabel* labTree = nullptr;
    QLabel* labCloud = nullptr;
    int cloudid = 0;
};

int randNum(int Max);