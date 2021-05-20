#include "dinool.h"

DinoOL::DinoOL(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);
    QMovie* movie = new QMovie(":/pic/gif/dino_start");
    ui.label->setMovie(movie);
    connect(ui.label_2, SIGNAL(linkActivated(QString)),this,SLOT(NetworkChk(QString)));
    movie->start();
    ui.frmKey->hide();
    maxH = 1670.0 / G;
    pdtime = new QTimer(this);
    pdtime->setInterval(100);
    connect(pdtime, SIGNAL(timeout()), this, SLOT(printpos()));
    //pdtime->start();
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
            ui.labRoad->move(0, 0.6 * y - 0.2551 * ui.label->height());
        }
        else
        {
            ui.label->setGeometry(ui.label->x() > x - ui.label->width() ? x - ui.label->width() : ui.label->x(), 0.6 * y - ui.label->height(), 88 / 800.0 * x, 98 / 800.0 * x);
            ui.labRoad->setGeometry(0, (0.6 * y - 0.2551 * ui.label->height()), 2 * x, x / 100.0);
        }
    }
    ui.label_2->move(ui.label->x(), ui.label->y() + 70);
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
        //QPropertyAnimation* pScaleAnimation1;
        //QMovie* movie;
        switch (e->key())
        {
            /*
        case (Qt::Key_W): //W
            if ((ui.label->y() - (int)(0.6 * this->frameGeometry().height() - ui.label->height()) <= 1))
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
            break;
            */
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

            break;
        case (Qt::Key_D): //D
            if (ui.label->x() < this->frameGeometry().width() - ui.label->width() && ui.label->y() == (int)(0.6 * this->frameGeometry().height() - ui.label->height()))
            {
                pScaleAnimation1 = new QPropertyAnimation(ui.label, "pos");
                pScaleAnimation1->setDuration((this->frameGeometry().width() - 1. * (ui.label->x() - ui.label->width())) / 0.5);
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
    //QMovie* movie;
    //QPropertyAnimation* pScaleAnimation1, * pScaleAnimation2;
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
        break;
    case(3):
        //pScaleAnimation1->stop();
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
    }
}

void DinoOL::StartStep1() { StartGame(1); }
void DinoOL::StartStep2() { StartGame(2); }
void DinoOL::StartStep3() { StartGame(3); }
void DinoOL::StartStep4() { StartGame(4); }

void DinoOL::roadloop()
{
    int t;
    int y = this->frameGeometry().height();
    pScaleAnimationRoad = new QPropertyAnimation(ui.labRoad, "pos");
    if (isStarted >= 3)
    {
        t = 1200 / v;
        ui.labRoad->move(0, ui.label->y());
        pScaleAnimationRoad->setDuration(t * 1000);
        pScaleAnimationRoad->setStartValue(QPoint(ui.labRoad->x(), 0.6 * y - ui.label->height() + 73));
        pScaleAnimationRoad->setEndValue(QPoint(0 - ui.labRoad->width() * 0.4, 0.6 * y - ui.label->height() + 73));
        pScaleAnimationRoad->start();    
        QTimer::singleShot(t * 1000, this, SLOT(roadloop()));
    }
    else
    {
        pScaleAnimationRoad->stop();
    }
}