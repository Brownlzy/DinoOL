#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_dinool.h"

#include <QtGui/QMovie>
#include <QtCore/QProcess>
#include <QtCore/QSignalMapper>
#include <QtCore/QTimer>
#include <QtGui/QKeyEvent>
#include <QtCore/QPropertyAnimation>


class DinoOL : public QMainWindow
{
    Q_OBJECT

public:
    DinoOL(QWidget *parent = Q_NULLPTR);
    void StartGame(int step = 0);
    void CleanAM(int);

public slots:
    void NetworkChk(QString);
    void keyPressEvent(QKeyEvent* e);
    void keyReleaseEvent(QKeyEvent* e);
    void StartStep1();
    void StartStep2();
    void StartStep3();
    void StartStep4();
    void printpos();
    void roadloop();

protected:
    virtual void resizeEvent(QResizeEvent* event) override;

private:
    Ui::DinoOLClass ui;
    int isStarted = 0;
    QTimer* ptimer = nullptr;
    QTimer* pdtime = nullptr;
    float G = 9.8;
    double maxH;
    QMovie* movie = nullptr;
    QPropertyAnimation* pScaleAnimation1 = nullptr;
    QPropertyAnimation* pScaleAnimation2 = nullptr;
    QPropertyAnimation* pScaleAnimationRoad = nullptr;
    double v = 300;
};
