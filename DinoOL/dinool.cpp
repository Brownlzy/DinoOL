#include "dinool.h"

DinoOL::DinoOL(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);
    QMovie* movie = new QMovie(":/pic/gif/dino_start");
    ui.label->setMovie(movie);
    connect(ui.label_2, SIGNAL(linkActivated(QString)),this,SLOT(NetworkChk(QString)));
    movie->start();
    maxH = 1670.0 / G;
    pdtime = new QTimer(this);
    proad = new QTimer(this);
    ptcloud = new QTimer(this);
    pdtime->setInterval(100);
    connect(proad, SIGNAL(timeout()), this, SLOT(roadloop()));
    connect(ptcloud, SIGNAL(timeout()), this, SLOT(cloudloop()));
    connect(pdtime, SIGNAL(timeout()), this, SLOT(printpos()));
    //pdtime->start();
}
DinoOL::~DinoOL()
{
    if (labCloud != nullptr)
    {
           delete[] labCloud;
    }
}
void DinoOL::printpos()
{
    QString tmp;
    tmp = "(" + QString::number(ui.label->x()) + "," + QString::number(ui.label->y()) + ")";
    ui.lab->setText(tmp);
    pdtime->start();
}
void DinoOL::resizeEvent(QResizeEvent* event)
{
    //获取ui形成窗口宽度
    int x = this->frameGeometry().width();
    int y = this->frameGeometry().height();
    qDebug()<<x<<","<<y;
    if (!isStarted)
    {
        ui.label->move(0.2 * x, 0.2 * y);
        ui.labRoad->move(0.2 * x, 0.2 * y + 34);
    }
    else if (isStarted == 1)
    {
        ui.label->move(0.2 * x, 0.2 * y - 81);
        ui.labRoad->move(0.2 * x, 0.2 * y - 43);
    }
    else if (isStarted == 2)
    {
        ui.label->move(0.2 * x, 0.2 * y);
        ui.labRoad->move(0.2 * x, 0.2 * y + 34);
    }
    else if (isStarted >= 3)
    {
        if (x < 8000)
        {
            ui.label->setGeometry(ui.label->x() > x - ui.label->width() ? x - ui.label->width() : ui.label->x(), 0.6 * y - ui.label->height(),88,98);
            ui.labRoad->move(ui.labRoad->x(), 0.6 * y - 0.2551 * ui.label->height());
        }
        else
        {
            ui.label->setGeometry(ui.label->x() > x - ui.label->width() ? x - ui.label->width() : ui.label->x(), 0.6 * y - ui.label->height(), 88 / 800.0 * x, 98 / 800.0 * x);
            ui.labRoad->setGeometry(0, (0.6 * y - 0.2551 * ui.label->height()), 2 * x, x / 100.0);
        }
    }
    ui.label_2->move(ui.label->x(), ui.label->y() + 70);
    ui.whitebg->setGeometry(0, 0, x, y);
    roadloop();
}

void DinoOL::NetworkChk(QString str)
{
    QProcess process(this);
    process.setProgram("C:\\Windows\\System32\\msdt.exe");
    QStringList argu;
    argu << "-skip" << "TRUE" << "-path" << "C:\\Windows\\diagnostics\\system\\networking" << "-ep" << "NetworkDiagnosticsPNI";
    process.setArguments(argu);
    process.startDetached();
}

void DinoOL::keyPressEvent(QKeyEvent* e)
{
    if (e->key() == Qt::Key_W && ui.label->y() == (int)(0.6 * this->frameGeometry().height() - ui.label->height()))
    {
        movie = new QMovie(":/pic/png/dino");
        ui.label->setMovie(movie);
        movie->start();
        pScaleAnimation2 = new QPropertyAnimation(ui.label, "pos");
        pScaleAnimation2->setEasingCurve(QEasingCurve::OutQuad);
        pScaleAnimation2->setDuration(300);
        pScaleAnimation2->setStartValue(QPoint(ui.label->x(), ui.label->y()));
        pScaleAnimation2->setEndValue(QPoint(ui.label->x(), ui.label->y() - maxH));
        pScaleAnimation2->start();
        isStarted = 4;
        QTimer::singleShot(300, this, SLOT(StartStep3()));
    }
    if (!e->isAutoRepeat())
    {
        if (!isStarted && e->key() == Qt::Key_Space)
        {
            StartGame();
            return;
        }
            //int flag = 0;
        switch (e->key())
        {
        case (Qt::Key_A): //A
            if (ui.label->x() >= 0 && ui.label->y() == (int)(0.6 * this->frameGeometry().height() - ui.label->height()))
            {
                pScaleAnimation1 = new QPropertyAnimation(ui.label, "pos");
                pScaleAnimation1->setDuration(ui.label->x() / 0.5);
                pScaleAnimation1->setStartValue(QPoint(ui.label->x(), ui.label->y()));
                pScaleAnimation1->setEndValue(QPoint(0, ui.label->y()));
                pScaleAnimation1->start();
            }
            break;
        case (Qt::Key_S): //S
            if (!isDive && ui.label->y() == (int)(0.6 * this->frameGeometry().height() - ui.label->height()))
            {
                isDive = true;
                CleanAM(2);
                movie = new QMovie(":/pic/gif/dino_dive");
                ui.label->setMovie(movie);
                ui.label->setGeometry(ui.label->x(), ui.label->y() + 34, 118, 60);
                ui.label->setScaledContents(true);
                movie->start();
            }/*
            else if (!isDive && flag == 0)
            {
                CleanAM(3);
                pScaleAnimation2 = new QPropertyAnimation(ui.label, "pos");
                pScaleAnimation2->setEasingCurve(QEasingCurve::InQuad);
                pScaleAnimation2->setDuration(100);
                pScaleAnimation2->setStartValue(QPoint(ui.label->x(), ui.label->y()));
                pScaleAnimation2->setEndValue(QPoint(ui.label->x(), 0.6 * this->frameGeometry().height() - ui.label->height()));
                pScaleAnimation2->start();
                QTimer::singleShot(100, this, SLOT(StartStep5()));
                flag = 1;
            }*/
            break;
        case (Qt::Key_D): //D
            if (!isDive && ui.label->x() < this->frameGeometry().width() - ui.label->width() && ui.label->y() == (int)(0.6 * this->frameGeometry().height() - ui.label->height()))
            {
                pScaleAnimation1 = new QPropertyAnimation(ui.label, "pos");
                pScaleAnimation1->setDuration((this->frameGeometry().width() - 1. * ui.label->x() - ui.label->width()) / 0.5);
                pScaleAnimation1->setStartValue(QPoint(ui.label->x(), ui.label->y()));
                pScaleAnimation1->setEndValue(QPoint(this->frameGeometry().width() - ui.label->width(), ui.label->y()));
                pScaleAnimation1->start();
            }
                break;
        default:

            break;
        }
    }
}

void DinoOL::keyReleaseEvent(QKeyEvent* e)
{
    if (!e->isAutoRepeat())
    {
        if (!isStarted || e->key() == Qt::Key_Space)
        {
            //StartGame();
            return;
        }
        switch (e->key())
        {
        case (Qt::Key_A): //A
            if (ui.label->y() == (int)(0.6 * this->frameGeometry().height() - ui.label->height()))
                //pScaleAnimation1->stop();
                CleanAM(2);
            break;
        case (Qt::Key_D): //D
            if (ui.label->y() == (int)(0.6 * this->frameGeometry().height() - ui.label->height()))
                //pScaleAnimation1->stop();
                CleanAM(2);
            break;
        case (Qt::Key_S): //S
            if (isDive)
            {
                isDive = false;
                movie = new QMovie(":/pic/gif/dino_run");
                ui.label->setGeometry(ui.label->x(), ui.label->y() - 34, 88, 98);
                ui.label->setMovie(movie);
                ui.label->setScaledContents(true);
                movie->start();
            }
            break;

        default:
            break;
        }
    }

}

void DinoOL::CleanAM(int id)
{
    switch (id)
    {
    case 1:
        if (movie != nullptr)
        {
            movie->stop();
            delete[] movie;
            movie = nullptr;
        }
        break;
    case 2:
        if (pScaleAnimation1 != nullptr)
        {
            pScaleAnimation1->stop();
            delete[] pScaleAnimation1;
            pScaleAnimation1 = nullptr;
        }
        break;
    case 3:
        if (pScaleAnimation2 != nullptr)
        {
            pScaleAnimation2->stop();
            delete[] pScaleAnimation2;
            pScaleAnimation2 = nullptr;
        }
        break;
    default:
        break;
    }
}
void DinoOL::StartGame(int step)
{
    int x = this->frameGeometry().width();
    int y = this->frameGeometry().height();
    switch (step)
    {
    case(0): 
        movie = new QMovie(":/pic/gif/dino_jump");
        ui.label->setGeometry(0.2 * x, 0.2 * y - 81, 44, 130);
        ui.label->setMovie(movie);
        ui.label->setScaledContents(true);
        movie->start();
        QTimer::singleShot(900, this, SLOT(StartStep1()));
        isStarted = 1;
        break;
    case(1):
        movie = new QMovie(":/pic/gif/dino_run");
        ui.label->setGeometry(0.2 * x, 0.2 * y, 44, 49);
        ui.label->setMovie(movie);
        ui.label->setScaledContents(true);
        movie->start();
        pScaleAnimation1 = new QPropertyAnimation(ui.labRoad, "geometry");
        pScaleAnimation1->setDuration(700);
        pScaleAnimation1->setStartValue(QRect(ui.labRoad->x(), ui.labRoad->y(), ui.labRoad->width(), ui.labRoad->height()));
        pScaleAnimation1->setEndValue(QRect(ui.labRoad->x(), ui.labRoad->y(), 0.6 * x, ui.labRoad->height()));
        pScaleAnimation1->start();
        QTimer::singleShot(700, this, SLOT(StartStep2()));
        isStarted = 2;
        break;
    case(2):
        ui.label_2->hide();
        movie = new QMovie(":/pic/png/road");
        //ui.label->setGeometry(0.2 * x, 0.2 * y, 44, 49);
        ui.labRoad->setMovie(movie);
        //ui.labRoad->setScaledContents(true);
        movie->start();
        pScaleAnimation1 = new QPropertyAnimation(ui.label, "geometry");
        pScaleAnimation1->setDuration(700);
        pScaleAnimation1->setStartValue(QRect(ui.label->x(), ui.label->y(), ui.label->width(), ui.label->height()));
        pScaleAnimation1->setEndValue(QRect(0, 0.6 * y - 2.0 * ui.label->height(), 2 * ui.label->width(), 2 * ui.label->height()));
        pScaleAnimation2 = new QPropertyAnimation(ui.labRoad, "geometry");
        pScaleAnimation2->setDuration(700);
        pScaleAnimation2->setStartValue(QRect(ui.labRoad->x(), ui.labRoad->y(), ui.labRoad->width(), ui.labRoad->height()));
        pScaleAnimation2->setEndValue(QRect(0, 0.6 * y - 2.0 * ui.label->height() + 73, 250 * ui.labRoad->height(), 1.0 * ui.labRoad->height()));
        pScaleAnimation1->start();
        pScaleAnimation2->start();
        ui.labRoad->setScaledContents(true);
        isStarted = 3;
        QTimer::singleShot(700, this, SLOT(roadloop()));
        QTimer::singleShot(700, this, SLOT(cloudloop()));
        break;
    case(3):
        pScaleAnimation2 = new QPropertyAnimation(ui.label, "pos");
        pScaleAnimation2->setEasingCurve(QEasingCurve::InQuad);
        pScaleAnimation2->setDuration(300);
        pScaleAnimation2->setStartValue(QPoint(ui.label->x(), ui.label->y()));
        pScaleAnimation2->setEndValue(QPoint(ui.label->x(), 0.6 * y - ui.label->height()));
        pScaleAnimation2->start();
        QTimer::singleShot(300, this, SLOT(StartStep4()));
        break;
    case(4):
        movie = new QMovie(":/pic/gif/dino_run");
        ui.label->setMovie(movie);
        movie->start();
        break;
    case(5):
        isDive = true;
        CleanAM(2);
        movie = new QMovie(":/pic/gif/dino_dive");
        ui.label->setMovie(movie);
        ui.label->setGeometry(ui.label->x(), ui.label->y() + 34, 118, 60);
        ui.label->setScaledContents(true);
        movie->start();
        break;
    }
}

void DinoOL::StartStep1() { StartGame(1); }
void DinoOL::StartStep2() { StartGame(2); }
void DinoOL::StartStep3() { StartGame(3); }
void DinoOL::StartStep4() { StartGame(4); }
void DinoOL::StartStep5() { StartGame(5); }

void DinoOL::roadloop()
{
    int t;
    int y = this->frameGeometry().height();
    pAnimationRoad = new QPropertyAnimation(ui.labRoad, "pos");
    if (isStarted >= 3)
    {
        if (ui.labRoad->x() >= -1200)
        {
            t = (1200.0 + ui.labRoad->x()) / v;
            pAnimationRoad->setDuration(t * 1000);
            pAnimationRoad->setStartValue(QPoint(ui.labRoad->x(), ui.labRoad->y()));
            pAnimationRoad->setEndValue(QPoint(0 - ui.labRoad->width() * 0.4, ui.labRoad->y()));
            pAnimationRoad->start();
            proad->setInterval(t * 1000);
            proad->start();
        }
        else
        {
            t = 1200 / v;
            ui.labRoad->move(0, ui.labRoad->y());
            pAnimationRoad->setDuration(t * 1000);
            pAnimationRoad->setStartValue(QPoint(ui.labRoad->x(), ui.labRoad->y()));
            pAnimationRoad->setEndValue(QPoint(0 - ui.labRoad->width() * 0.4, ui.labRoad->y()));
            pAnimationRoad->start();
            proad->setInterval(t * 1000);
            proad->start();
        }
    }
    else
    {
        pAnimationRoad->stop();
    }
}

void DinoOL::cloudloop()
{
    if (labCloud == nullptr)
    {
        labCloud = new QLabel[3];
        for (int i = 0; i < 3; i++)
        {
            labCloud[i].setParent(this);
        }
    }
    ui.label->raise();
    int x = this->frameGeometry().width();
    int y = this->frameGeometry().height();
    QPropertyAnimation* pAnimationCloud = new QPropertyAnimation(&labCloud[cloudid], "pos",this);
    int t1, t2;
    float cy;
    t1 = randNum(500) + 4000;
    t2 = randNum(500) + 1500;
    cy = 0.2 + randNum(3) / 10.0;
    QMovie *movie = new QMovie(":/pic/png/cloud");
    labCloud[cloudid].setGeometry(0, 0.2 * y, 46, 13);
    labCloud[cloudid].setMovie(movie);
    //labCloud[cloudid].adjustSize();
    labCloud[cloudid].setScaledContents(true);
    movie->start();
    labCloud[cloudid].setGeometry(x, 0.2 * y, 2 * labCloud[cloudid].width(), 2 * labCloud[cloudid].height());
    labCloud[cloudid].show();
    pAnimationCloud->setDuration(t1);
    pAnimationCloud->setStartValue(QPoint(x, cy * y));
    pAnimationCloud->setEndValue(QPoint(0 - labCloud[cloudid].width(), cy * y));
    pAnimationCloud->start();
    if (cloudid == 2)
    {
        cloudid = 0;
    }
    else
    {
        cloudid++;
    }
    ptcloud->setInterval(t2);
    ptcloud->start();
}

int randNum(int Max)
{
    QTime time = QTime::currentTime();
    qsrand(time.msec() + time.second() * 1000);
    int n = qrand() % Max;
    return n;
}
