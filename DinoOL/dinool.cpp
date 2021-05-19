#include "dinool.h"
#include <QtGui/QMovie>
#include <QtCore/QProcess>

DinoOL::DinoOL(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);
    QMovie* movie = new QMovie(":/pic/gif/dino_start");
    ui.label->setMovie(movie);
    connect(ui.label_2, SIGNAL(linkActivated(QString)),this,SLOT(NetworkChk(QString)));
    movie->start();
}

void DinoOL::resizeEvent(QResizeEvent* event)
{
    //获取ui形成窗口宽度
    int x = this->frameGeometry().width(); 
    int y = this->frameGeometry().height(); 
    ui.label->move(0.3 * x, 0.2 * y);
    ui.label_2->move(ui.label->x(), ui.label->y() + 70);
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