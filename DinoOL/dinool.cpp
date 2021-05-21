#include "dinool.h"

DinoOL::DinoOL(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);
    setDinoState(":/pic/gif/dino_start");
    ui.label_3->hide();
    ui.frame->hide();
    ui.frame_2->hide();
    ui.frame_3->hide();
    connect(ui.label_2, SIGNAL(linkActivated(QString)),this,SLOT(NetworkChk(QString)));
    maxH = vy * vy / (2 * G);
    pdtime = new QTimer(this);
    proad = new QTimer(this);
    ptcloud = new QTimer(this);
    ptdino = new QTimer(this);
    ptdino->setInterval(tms);
    pdtime->setInterval(100);
    connect(proad, SIGNAL(timeout()), this, SLOT(roadloop()));
    connect(ptcloud, SIGNAL(timeout()), this, SLOT(cloudloop()));
    connect(pdtime, SIGNAL(timeout()), this, SLOT(printpos()));
    connect(ptdino, SIGNAL(timeout()), this, SLOT(printDino()));
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
    tmp += ",vx=" + QString::number(vx) + ",vy=" + QString::number(vy) + ",onG=" + QString::number(isOnGround());
    ui.lab->setText(tmp);
    ui.lab->adjustSize();
    pdtime->start();
}
void DinoOL::resizeEvent(QResizeEvent* event)
{
    //获取ui形成窗口宽度
    int x = this->frameGeometry().width();
    int y = this->frameGeometry().height();
    horline = 0.6 * y;
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
            ui.label->setGeometry(ui.label->x() > x - ui.label->width() ? x - ui.label->width() : ui.label->x(), horline - ui.label->height(),88,98);
            ui.labRoad->move(ui.labRoad->x(), horline - 0.2551 * ui.label->height());
        }
        else
        {
            ui.label->setGeometry(ui.label->x() > x - ui.label->width() ? x - ui.label->width() : ui.label->x(), horline - ui.label->height(), 88 / 800.0 * x, 98 / 800.0 * x);
            ui.labRoad->setGeometry(0, (horline - 0.2551 * ui.label->height()), 2 * x, x / 100.0);
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
    if (!isStarted && e->key() == Qt::Key_Space)
    {
        StartGame();
        return;
    }
    if (isStarted < 4) return;
    bool onG = isOnGround();
    if (e->key() == Qt::Key_W && onG)
    {
        ptdino->start();
        vy += vy0;
        isJump = 1;
    }
    if (!e->isAutoRepeat() ) //&& onG)
    {
        switch (e->key())
        {
        case (Qt::Key_A): //A
            ptdino->start();
            if (onG)
            {
                isMove = -1;
                vx = 0 - vx0;
            }
            else
            {
                isMove = -2;
                //vx = 0;
            }
            break;
        case (Qt::Key_S): //S
            if (!isDive)// && !isJump)
            {
                setDinoState(":/pic/gif/dino_dive");
                isDive = true;
                ui.label->setGeometry(ui.label->x(), ui.label->y() + 34, 118, 60);
                ui.label->setScaledContents(true);
            }
            break;
        case (Qt::Key_D): //D
            ptdino->start();
            if (onG)
            {
                isMove = 1;
                vx = vx0;
            }
            else
            {
                isMove = 2;
                //vx = 0;
            }
            break;
        default:

            break;
        }
    }
}

void DinoOL::keyReleaseEvent(QKeyEvent* e)
{
    if (isStarted < 4) return;
    bool onG = isOnGround();
    if (!e->isAutoRepeat())
    {
        switch (e->key())
        {
        case (Qt::Key_A): //A
            if (isOnGround())
            {
                vx = 0;
                ptdino->stop();
            }
            isMove = 0;
            break;
        case (Qt::Key_D): //D
            if (isOnGround())
            {
                vx = 0;
                ptdino->stop();
            }
            isMove = 0;
            break;
        case (Qt::Key_S): //S
            if (isDive)
            {
                setDinoState(":/pic/gif/dino_run");
                ui.label->setGeometry(ui.label->x(), ui.label->y() - 34, 88, 98);
                isDive = false;
                ui.label->setScaledContents(true);
            }
            break;

        default:
            break;
        }
    }

}
void DinoOL::CleanAM(QPropertyAnimation* pitem)
{
    if (pitem !=NULL)
    {
        pitem->stop();
    }
}
void DinoOL::CleanAM(QMovie* pitem)
{
    if (pitem !=NULL)
    {
        pitem->stop();
    }
}

void DinoOL::StartGame(int step)
{
    int x = this->frameGeometry().width();
    int y = this->frameGeometry().height();
    switch (step)
     {
     case(0): 
         setDinoState(":/pic/gif/dino_jump");
         ui.label->setGeometry(0.2 * x, 0.2 * y - 81, 44, 130);
         ui.label->setScaledContents(true);
         QTimer::singleShot(900, this, SLOT(StartStep1()));
         isStarted = 1;
         break;
     case(1):
         setDinoState(":/pic/gif/dino_run");
         ui.label->setGeometry(0.2 * x, 0.2 * y, 44, 49);
         ui.label->setScaledContents(true);
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
         ui.labRoad->setMovie(movie_road);
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

bool DinoOL::isOnGround()
{
    double ly = ui.label->y();
    double lh = ui.label->height();
    double tmp = ly + lh - horline;
    if (tmp <= vy0 * tms / 1000.0 && tmp >= 0 - vy0 * tms / 1000.0)
    {
        return true;
    }
    return false;
}

void DinoOL::setDinoState(QString pic)
{
    if (movie_dino == NULL) movie_dino = new QMovie(this);
    movie_dino->stop();
    movie_dino->setFileName(pic);
    ui.label->setMovie(movie_dino);
    movie_dino->start();
}

void DinoOL::printDino()
{
    if (vy > 0 - vy0 * tms / 2000 || vy < vy0 * tms / 2000 && !isOnGround())
        vy -= G * tms / 1000;
    else if (isOnGround())
    {
        vy = 0;
        if (!isMove)
        {
            vx = 0;
            ptdino->stop();
        }
        ui.label->move(ui.label->x(), horline - ui.label->height());
    }
    if ((isMove >= -1 && isMove <= 1 && !isOnGround()) || isOnGround())
    {
        if (isOnGround() && isMove) vx = isMove / fabs(isMove) * vx0;
        ui.label->move(ui.label->x() + vx / 1000 * tms, ui.label->y() - vy / 1000 * tms);
    }
    //else if(isMove == 2 || isMove ==-2)
        //ui.label->move(ui.label->x(), ui.label->y() - vy / 1000 * tms);
    else
        ui.label->move(ui.label->x() + vx / 1000 * tms, ui.label->y() - vy / 1000 * tms);
    if (isOnGround() && isJump == 1)
    {
        isJump = 0;
        setDinoState(":/pic/gif/dino_run");
    }
    if (!isOnGround() && isJump == 0)
        isJump = 1;

    if (isOnGround() && isJump == 0)
    {
        if (!isDive)
        {
        }
        else
            setDinoState(":/pic/gif/dino_dive");
    }
    else if (!isOnGround() && isJump == 1) 
    {
        if (!isDive)
            setDinoState(":/pic/png/dino");
        else
            setDinoState(":/pic/gif/dino_dive");
    }
}

void DinoOL::StartStep1() { StartGame(1); }
void DinoOL::StartStep2() { StartGame(2); }

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
            t = (1200.0 + ui.labRoad->x()) / vx0;
            pAnimationRoad->setDuration(t * 1000);
            pAnimationRoad->setStartValue(QPoint(ui.labRoad->x(), ui.labRoad->y()));
            pAnimationRoad->setEndValue(QPoint(0 - ui.labRoad->width() * 0.4, ui.labRoad->y()));
            pAnimationRoad->start();
            proad->setInterval(t * 1000);
            proad->start();
        }
        else
        {
            t = 1200 / vx0;
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
    if (isStarted == 3) isStarted++;
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
    if (isStarted == 3) isStarted++;
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
