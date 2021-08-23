#include "dinool.h"

DinoOL::DinoOL(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);
    movie_dino = new QMovie(":/pic/gif/dino_start");
    ui.label->setMovie(movie_dino);
    movie_dino->start();
    ui.label_3->hide();
    ui.frame->hide();
    ui.frame_2->hide();
    ui.frame_3->hide();
    connect(ui.label_2, SIGNAL(linkActivated(QString)),this,SLOT(NetworkChk(QString)));
    maxH = vy * vy / (2 * G);
    pdtime = new QTimer(this);
    proad = new QTimer(this);
    ptcloud = new QTimer(this);
    pdtime->setInterval(100);
    connect(proad, SIGNAL(timeout()), this, SLOT(roadloop()));
    connect(ptcloud, SIGNAL(timeout()), this, SLOT(cloudloop()));
    connect(pdtime, SIGNAL(timeout()), this, SLOT(printpos()));
    pdtime->start();
}
DinoOL::~DinoOL()
{
    if (labCloud != NULL)
    {
        delete[] labCloud;
    }
    if (movie_cloud != NULL)
    {
        delete[] movie_cloud;
    }
    if (pAnimationCloud != NULL)
    {
        delete[] pAnimationCloud;
    }
}
void DinoOL::printpos()
{
    QString tmp;
    tmp = "(" + QString::number(ui.label->x()) + "," + QString::number(ui.label->y()) + ")";
    tmp = tmp + ",isDive:" + QString::number(isDive) + ",isJump:" + QString::number(isJump) + ",isMove:" + QString::number(isMove);
    ui.lab->setText(tmp);
    ui.lab->adjustSize();
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
    if (e->key() == Qt::Key_W && !isJump)
    {
        switch (isMove)
        {
        case -1:
            CleanAM(movie_dino);
            movie_dino->setFileName(":/pic/png/dino");
            ui.label->setMovie(movie_dino);
            movie_dino->start();
            CleanAM(pLeftRight);
            if (pJumpDive == NULL) 
                pJumpDive = new QPropertyAnimation(ui.label, "pos");
            //pScaleAnimation2->setEasingCurve(QEasingCurve::OutInQuad);
            pJumpDive->setDuration(vy / G * 2 * 1000);
            pJumpDive->setStartValue(QPoint(ui.label->x(), ui.label->y()));
            pJumpDive->setKeyValueAt(0.1, QPoint(ui.label->x() - 0.2 * vy / G * vx, ui.label->y() - 0.36 * maxH));
            pJumpDive->setKeyValueAt(0.2, QPoint(ui.label->x() - 0.4 * vy / G * vx, ui.label->y() - 0.64 * maxH));
            pJumpDive->setKeyValueAt(0.3, QPoint(ui.label->x() - 0.6 * vy / G * vx, ui.label->y() - 0.84 * maxH));
            pJumpDive->setKeyValueAt(0.4, QPoint(ui.label->x() - 0.8 * vy / G * vx, ui.label->y() - 0.96 * maxH));
            pJumpDive->setKeyValueAt(0.5, QPoint(ui.label->x() - 1.0 * vy / G * vx, ui.label->y() - maxH));
            pJumpDive->setKeyValueAt(0.6, QPoint(ui.label->x() - 1.2 * vy / G * vx, ui.label->y() - 0.96 * maxH));
            pJumpDive->setKeyValueAt(0.7, QPoint(ui.label->x() - 1.4 * vy / G * vx, ui.label->y() - 0.84 * maxH));
            pJumpDive->setKeyValueAt(0.8, QPoint(ui.label->x() - 1.6 * vy / G * vx, ui.label->y() - 0.64 * maxH));
            pJumpDive->setKeyValueAt(0.9, QPoint(ui.label->x() - 1.8 * vy / G * vx, ui.label->y() - 0.36 * maxH));
            pJumpDive->setEndValue(QPoint(ui.label->x() - 2 * vy / G * vx, ui.label->y()));
            pJumpDive->start();
            isJump = true;
            isStarted = 4;
            QTimer::singleShot(vy / G * 2 * 1000, this, SLOT(Keyr()));
            break;
        case 1:
            CleanAM(movie_dino);
            movie_dino->setFileName(":/pic/png/dino");
            ui.label->setMovie(movie_dino);
            movie_dino->start();
            CleanAM(pLeftRight);
            if (pJumpDive == NULL) 
                pJumpDive = new QPropertyAnimation(ui.label, "pos");
            //pScaleAnimation2->setEasingCurve(QEasingCurve::OutInQuad);
            pJumpDive->setDuration(vy / G * 2 * 1000);
            pJumpDive->setStartValue(QPoint(ui.label->x(), ui.label->y()));
            pJumpDive->setKeyValueAt(0.1, QPoint(ui.label->x() + 0.2 * vy / G * vx, ui.label->y() - 0.36 * maxH));
            pJumpDive->setKeyValueAt(0.2, QPoint(ui.label->x() + 0.4 * vy / G * vx, ui.label->y() - 0.64 * maxH));
            pJumpDive->setKeyValueAt(0.3, QPoint(ui.label->x() + 0.6 * vy / G * vx, ui.label->y() - 0.84 * maxH));
            pJumpDive->setKeyValueAt(0.4, QPoint(ui.label->x() + 0.8 * vy / G * vx, ui.label->y() - 0.96 * maxH));
            pJumpDive->setKeyValueAt(0.5, QPoint(ui.label->x() + 1.0 * vy / G * vx, ui.label->y() - maxH));
            pJumpDive->setKeyValueAt(0.6, QPoint(ui.label->x() + 1.2 * vy / G * vx, ui.label->y() - 0.96 * maxH));
            pJumpDive->setKeyValueAt(0.7, QPoint(ui.label->x() + 1.4 * vy / G * vx, ui.label->y() - 0.84 * maxH));
            pJumpDive->setKeyValueAt(0.8, QPoint(ui.label->x() + 1.6 * vy / G * vx, ui.label->y() - 0.64 * maxH));
            pJumpDive->setKeyValueAt(0.9, QPoint(ui.label->x() + 1.8 * vy / G * vx, ui.label->y() - 0.36 * maxH));
            pJumpDive->setEndValue(QPoint(ui.label->x() + 2 * vy / G * vx, ui.label->y()));
            pJumpDive->start();
            isJump = true;
            isStarted = 4;
            QTimer::singleShot(vy / G * 2 * 1000, this, SLOT(Keyr()));
            break;
        default:
            CleanAM(movie_dino);
            movie_dino->setFileName(":/pic/png/dino");
            ui.label->setMovie(movie_dino);
            movie_dino->start();
            if (pJumpDive == NULL) 
                pJumpDive = new QPropertyAnimation(ui.label, "pos");
            pJumpDive->setEasingCurve(QEasingCurve::OutQuad);
            pJumpDive->setDuration(vy / G * 1000);
            pJumpDive->setStartValue(QPoint(ui.label->x(), ui.label->y()));
            pJumpDive->setEndValue(QPoint(ui.label->x(), ui.label->y() - maxH));
            pJumpDive->start();
            isJump = true;
            isStarted = 4;
            QTimer::singleShot(vy / G * 1000, this, SLOT(KeyW()));
            break;
        }
    }
    if (e->key() == Qt::Key_E && !isJump && ui.label->x() + ui.label->width() +180 <= this->frameGeometry().width())
    {
        CleanAM(movie_dino);
        movie_dino->setFileName(":/pic/png/dino");
        ui.label->setMovie(movie_dino);
        movie_dino->start();
        CleanAM(pJumpDive);
        if ( pJumpDive == NULL ) 
            pJumpDive = new QPropertyAnimation(ui.label, "pos");
        //pScaleAnimation2->setEasingCurve(QEasingCurve::OutInQuad);
        pJumpDive->setDuration(vy / G * 2 * 1000);
        pJumpDive->setStartValue(QPoint(ui.label->x(), ui.label->y()));
        pJumpDive->setKeyValueAt(0.1, QPoint(ui.label->x() + 0.2 * vy / G * vx, ui.label->y() - 0.36 * maxH));
        pJumpDive->setKeyValueAt(0.2, QPoint(ui.label->x() + 0.4 * vy / G * vx, ui.label->y() - 0.64 * maxH));
        pJumpDive->setKeyValueAt(0.3, QPoint(ui.label->x() + 0.6 * vy / G * vx, ui.label->y() - 0.84 * maxH));
        pJumpDive->setKeyValueAt(0.4, QPoint(ui.label->x() + 0.8 * vy / G * vx, ui.label->y() - 0.96 * maxH));
        pJumpDive->setKeyValueAt(0.5, QPoint(ui.label->x() + 1.0 * vy / G * vx, ui.label->y() - maxH));
        pJumpDive->setKeyValueAt(0.6, QPoint(ui.label->x() + 1.2 * vy / G * vx, ui.label->y() - 0.96 * maxH));
        pJumpDive->setKeyValueAt(0.7, QPoint(ui.label->x() + 1.4 * vy / G * vx, ui.label->y() - 0.84 * maxH));
        pJumpDive->setKeyValueAt(0.8, QPoint(ui.label->x() + 1.6 * vy / G * vx, ui.label->y() - 0.64 * maxH));
        pJumpDive->setKeyValueAt(0.9, QPoint(ui.label->x() + 1.8 * vy / G * vx, ui.label->y() - 0.36 * maxH));
        pJumpDive->setEndValue(QPoint(ui.label->x() + 2 * vy / G * vx, ui.label->y()));
        pJumpDive->start();
        isJump = true;
        isStarted = 4;
        QTimer::singleShot(vy / G * 2 * 1000, this, SLOT(Keyr()));
    }
    if (e->key() == Qt::Key_Q && !isJump && ui.label->x() >= 180)
    {
        CleanAM(movie_dino);
        movie_dino->setFileName(":/pic/png/dino");
        ui.label->setMovie(movie_dino);
        movie_dino->start();
        CleanAM(pJumpDive);
        if (pJumpDive == NULL) 
            pJumpDive = new QPropertyAnimation(ui.label, "pos");
        //pScaleAnimation2->setEasingCurve(QEasingCurve::OutInQuad);
        pJumpDive->setDuration(vy / G * 2 * 1000);
        pJumpDive->setStartValue(QPoint(ui.label->x(), ui.label->y()));
        pJumpDive->setKeyValueAt(0.1, QPoint(ui.label->x() - 0.2 * vy / G * vx, ui.label->y() - 0.36 * maxH));
        pJumpDive->setKeyValueAt(0.2, QPoint(ui.label->x() - 0.4 * vy / G * vx, ui.label->y() - 0.64 * maxH));
        pJumpDive->setKeyValueAt(0.3, QPoint(ui.label->x() - 0.6 * vy / G * vx, ui.label->y() - 0.84 * maxH));
        pJumpDive->setKeyValueAt(0.4, QPoint(ui.label->x() - 0.8 * vy / G * vx, ui.label->y() - 0.96 * maxH));
        pJumpDive->setKeyValueAt(0.5, QPoint(ui.label->x() - 1.0 * vy / G * vx, ui.label->y() - maxH));
        pJumpDive->setKeyValueAt(0.6, QPoint(ui.label->x() - 1.2 * vy / G * vx, ui.label->y() - 0.96 * maxH));
        pJumpDive->setKeyValueAt(0.7, QPoint(ui.label->x() - 1.4 * vy / G * vx, ui.label->y() - 0.84 * maxH));
        pJumpDive->setKeyValueAt(0.8, QPoint(ui.label->x() - 1.6 * vy / G * vx, ui.label->y() - 0.64 * maxH));
        pJumpDive->setKeyValueAt(0.9, QPoint(ui.label->x() - 1.8 * vy / G * vx, ui.label->y() - 0.36 * maxH));
        pJumpDive->setEndValue(QPoint(ui.label->x() - 2 * vy / G * vx, ui.label->y()));
        pJumpDive->start();
        isJump = true;
        isStarted = 4;
        QTimer::singleShot(vy / G * 2 * 1000, this, SLOT(Keyr()));
    }
    if (!e->isAutoRepeat())
    {
        if (!isStarted && e->key() == Qt::Key_Space)
        {
            StartGame();
            return;
        }
        switch (e->key())
        {
        case (Qt::Key_A): //A
            if (ui.label->x() >= 0 && !isJump)
            {
                CleanAM(pLeftRight);
                if ( pLeftRight == NULL ) pLeftRight = new QPropertyAnimation(ui.label, "pos");
                pLeftRight->setDuration(ui.label->x() / vx * 1000);
                pLeftRight->setStartValue(QPoint(ui.label->x(), ui.label->y()));
                pLeftRight->setEndValue(QPoint(0, ui.label->y()));
                pLeftRight->start();
                isMove = -1;
            }
            break;
        case (Qt::Key_S): //S
            if (!isDive && !isJump)
            {
                CleanAM(movie_dino);
                isDive = true;
                CleanAM(pLeftRight);
                pLeftRight->stop();
                movie_dino->setFileName(":/pic/gif/dino_dive");
                ui.label->setMovie(movie_dino);
                ui.label->setGeometry(ui.label->x(), ui.label->y() + 34, 118, 60);
                ui.label->setScaledContents(true);
                movie_dino->start();
            }
            break;
        case (Qt::Key_D): //D
            if (!isDive && ui.label->x() < this->frameGeometry().width() - ui.label->width() && !isJump)
            {
                CleanAM(pLeftRight);
                if (pLeftRight == NULL) pLeftRight = new QPropertyAnimation(ui.label, "pos");
                pLeftRight->setDuration((this->frameGeometry().width() - 1. * ui.label->x() - ui.label->width()) / vx * 1000);
                pLeftRight->setStartValue(QPoint(ui.label->x(), ui.label->y()));
                pLeftRight->setEndValue(QPoint(this->frameGeometry().width() - ui.label->width(), ui.label->y()));
                pLeftRight->start();
                isMove = 1;
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
            //if (ui.label->y() == (int)(0.6 * this->frameGeometry().height() - ui.label->height()))
            //{
                CleanAM(pLeftRight);
                //pLeftRight->stop();
                isMove = 0;
            //}
            break;
        case (Qt::Key_D): //D
            //if (ui.label->y() == (int)(0.6 * this->frameGeometry().height() - ui.label->height()))
            //{
                CleanAM(pLeftRight);
                //pLeftRight->stop();
                isMove = 0;
            //}
            break;
        case (Qt::Key_S): //S
            if (isDive)
            {
                isDive = false;
                CleanAM(movie_dino);
                movie_dino->setFileName(":/pic/gif/dino_run");
                ui.label->setGeometry(ui.label->x(), ui.label->y() - 34, 88, 98);
                ui.label->setMovie(movie_dino);
                ui.label->setScaledContents(true);
                movie_dino->start();
            }
            break;

        default:
            break;
        }
    }

}
void DinoOL::CleanAM(QObject* pitem)
{
    if (pitem !=NULL)
    {
        delete pitem;
        pitem =NULL;
    }

}
void DinoOL::CleanAM(QPropertyAnimation* pitem)
{
    if (pitem !=NULL)
    {
        pitem->stop();
        //delete pitem;
        //pitem =NULL;
    }

}
void DinoOL::CleanAM(QMovie* pitem)
{
    if (pitem !=NULL)
    {
        pitem->stop();
        //delete pitem;
        //pitem =NULL;
    }
}

void DinoOL::StartGame(int step)
{
    int x = this->frameGeometry().width();
    int y = this->frameGeometry().height();
    switch (step)
     {
     case(0): 
         CleanAM(movie_dino);
         movie_dino->setFileName(":/pic/gif/dino_jump");
         ui.label->setGeometry(0.2 * x, 0.2 * y - 81, 44, 130);
         ui.label->setMovie(movie_dino);
         ui.label->setScaledContents(true);
         movie_dino->start();
         QTimer::singleShot(900, this, SLOT(StartStep1()));
         isStarted = 1;
         break;
     case(1):
         CleanAM(movie_dino);
         movie_dino->setFileName(":/pic/gif/dino_run");
         ui.label->setGeometry(0.2 * x, 0.2 * y, 44, 49);
         ui.label->setMovie(movie_dino);
         ui.label->setScaledContents(true);
         movie_dino->start();
         CleanAM(pAnimation1);
         pAnimation1 = new QPropertyAnimation(ui.labRoad, "geometry");
         pAnimation1->setDuration(700);
         pAnimation1->setStartValue(QRect(ui.labRoad->x(), ui.labRoad->y(), ui.labRoad->width(), ui.labRoad->height()));
         pAnimation1->setEndValue(QRect(ui.labRoad->x(), ui.labRoad->y(), 0.6 * x, ui.labRoad->height()));
         pAnimation1->start();
         QTimer::singleShot(700, this, SLOT(StartStep2()));
         isStarted = 2;
         break;
     case(2):
         ui.label_2->hide();
         CleanAM(movie_road);
         movie_road = new QMovie(":/pic/png/road");
         //ui.label->setGeometry(0.2 * x, 0.2 * y, 44, 49);
         ui.labRoad->setMovie(movie_road);
         //ui.labRoad->setScaledContents(true);
         movie_road->start();
         CleanAM(pAnimation1);
         CleanAM(pAnimation2);
         pAnimation1 = new QPropertyAnimation(ui.label, "geometry");
         pAnimation1->setDuration(700);
         pAnimation1->setStartValue(QRect(ui.label->x(), ui.label->y(), ui.label->width(), ui.label->height()));
         pAnimation1->setEndValue(QRect(0, 0.6 * y - 2.0 * ui.label->height(), 2 * ui.label->width(), 2 * ui.label->height()));
         pAnimation2 = new QPropertyAnimation(ui.labRoad, "geometry");
         pAnimation2->setDuration(700);
         pAnimation2->setStartValue(QRect(ui.labRoad->x(), ui.labRoad->y(), ui.labRoad->width(), ui.labRoad->height()));
         pAnimation2->setEndValue(QRect(0, 0.6 * y - 2.0 * ui.label->height() + 73, 250 * ui.labRoad->height(), 1.0 * ui.labRoad->height()));
         pAnimation1->start();
         pAnimation2->start();
         ui.labRoad->setScaledContents(true);
         isStarted = 3;
         QTimer::singleShot(700, this, SLOT(roadloop()));
         QTimer::singleShot(700, this, SLOT(cloudloop()));
         break;
    }
}

void DinoOL::KeyRec(char key)
{
    int x = this->frameGeometry().width();
    int y = this->frameGeometry().height();
    switch (key)
    {
    case('W'):
        CleanAM(pJumpDive);
        pJumpDive = new QPropertyAnimation(ui.label, "pos");
        pJumpDive->setEasingCurve(QEasingCurve::InQuad);
        pJumpDive->setDuration(vy / G * 1000);
        pJumpDive->setStartValue(QPoint(ui.label->x(), ui.label->y()));
        pJumpDive->setEndValue(QPoint(ui.label->x(), 0.6 * y - ui.label->height()));
        pJumpDive->start();
        QTimer::singleShot(vy / G * 1000, this, SLOT(Keyr()));
        break;
    case('r'):
        CleanAM(movie_dino);
        movie_dino->setFileName(":/pic/gif/dino_run");
        ui.label->setMovie(movie_dino);
        movie_dino->start();
        switch (isMove)
        {
        case -1:
            CleanAM(pLeftRight);
            pLeftRight = new QPropertyAnimation(ui.label, "pos");
            pLeftRight->setDuration(ui.label->x() / vx * 1000);
            pLeftRight->setStartValue(QPoint(ui.label->x(), ui.label->y()));
            pLeftRight->setEndValue(QPoint(0, ui.label->y()));
            pLeftRight->start();
            isJump = false;
            break;
        case 1:
            CleanAM(pLeftRight);
            pLeftRight = new QPropertyAnimation(ui.label, "pos");
            pLeftRight->setDuration((this->frameGeometry().width() - 1. * ui.label->x() - ui.label->width()) / vx * 1000);
            pLeftRight->setStartValue(QPoint(ui.label->x(), ui.label->y()));
            pLeftRight->setEndValue(QPoint(this->frameGeometry().width() - ui.label->width(), ui.label->y()));
            pLeftRight->start();
            isJump = false;
            break;
        default:
            isJump = false;
            break;
        }
        break;
    case('S'):
        isDive = true;
        CleanAM(pLeftRight);
        CleanAM(movie_dino);
        movie_dino->setFileName(":/pic/gif/dino_dive");
        ui.label->setMovie(movie_dino);
        ui.label->setGeometry(ui.label->x(), ui.label->y() + 34, 118, 60);
        ui.label->setScaledContents(true);
        movie_dino->start();
        break;
    }
}

void DinoOL::StartStep1() { StartGame(1); }
void DinoOL::StartStep2() { StartGame(2); }
void DinoOL::KeyW() { KeyRec('W'); }
void DinoOL::Keyr() { KeyRec('r'); }
void DinoOL::KeyS() { KeyRec('S'); }
void DinoOL::KeyE() { KeyRec('E'); }
void DinoOL::KeyQ() { KeyRec('Q'); }

void DinoOL::roadloop()
{
    int t;
    int y = this->frameGeometry().height();
    CleanAM(pAnimationRoad);
    if(pAnimationRoad == NULL) 
        pAnimationRoad = new QPropertyAnimation(ui.labRoad, "pos");
    if (isStarted >= 3)
    {
        if (ui.labRoad->x() >= -1200)
        {
            t = (1200.0 + ui.labRoad->x()) / vx;
            pAnimationRoad->setDuration(t * 1000);
            pAnimationRoad->setStartValue(QPoint(ui.labRoad->x(), ui.labRoad->y()));
            pAnimationRoad->setEndValue(QPoint(0 - ui.labRoad->width() * 0.4, ui.labRoad->y()));
            pAnimationRoad->start();
            proad->setInterval(t * 1000);
            proad->start();
        }
        else
        {
            t = 1200 / vx;
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
    if (labCloud ==NULL)
    {
        labCloud = new QLabel[3];
        for (int i = 0; i < 3; i++)
        {
            labCloud[i].setParent(this);
        }
    }
    if (movie_cloud ==NULL)
    {
        movie_cloud = new QMovie[3];
        for (int i = 0; i < 3; i++)
        {
            movie_cloud[i].setParent(this);
        }
    }
    if (pAnimationCloud ==NULL)
    {
        pAnimationCloud = new QPropertyAnimation[3];
        for (int i = 0; i < 3; i++)
        {
            pAnimationCloud[i].setParent(this);
        }
    }
    ui.label->raise();
    int x = this->frameGeometry().width();
    int y = this->frameGeometry().height();
    pAnimationCloud[cloudid].setTargetObject(&labCloud[cloudid]);
    pAnimationCloud[cloudid].setPropertyName("pos");
    int t1, t2;
    float cy;
    t1 = randNum(500) + 4000;
    t2 = randNum(500) + 1500;
    cy = 0.2 + randNum(3) / 10.0;
    movie_cloud->setFileName(":/pic/png/cloud");
    labCloud[cloudid].setGeometry(0, 0.2 * y, 46, 13);
    labCloud[cloudid].setMovie(movie_cloud);
    labCloud[cloudid].setScaledContents(true);
    movie_cloud->start();
    labCloud[cloudid].setGeometry(x, 0.2 * y, 2 * labCloud[cloudid].width(), 2 * labCloud[cloudid].height());
    labCloud[cloudid].show();
    pAnimationCloud[cloudid].setDuration(t1);
    pAnimationCloud[cloudid].setStartValue(QPoint(x, cy * y));
    pAnimationCloud[cloudid].setEndValue(QPoint(0 - labCloud[cloudid].width(), cy * y));
    pAnimationCloud[cloudid].start();
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

void DinoOL::on_actionRun_as_a_server_triggered()
{
    qApp->exit(-1);
}

void DinoOL::on_actionExit_triggered()
{
    qApp->exit(0);
}

void DinoOL::on_actionConnect_a_server_triggered()
{
    ui.frame->setVisible(true);
    if (ui.btnCon->text() == "已连接")
        QTimer::singleShot(3000, ui.frame, SLOT(hide()));
}

void DinoOL::on_actionCreate_a_room_triggered()
{
    ui.frame_2->setVisible(true);

}

void DinoOL::on_actionJoin_a_room_triggered()
{
    ui.frame_3->setVisible(true);

}

int randNum(int Max)
{
    QTime time = QTime::currentTime();
    qsrand(time.msec() + time.second() * 1000);
    int n = qrand() % Max;
    return n;
}
