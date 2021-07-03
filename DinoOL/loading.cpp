#include "loading.h"

Loading::Loading(QWidget* parent)
	: QDialog(parent)
{
	ui.setupUi(this);
	Start();
	this->setWindowTitle("Loading...");
	this->setModal(true);
	//this->setWindowFlags(Qt::WindowStaysOnTopHint | Qt::WindowCloseButtonHint);
	this->setWindowFlags(Qt::WindowTitleHint);
	this->setWindowIcon(QIcon(":pic/icon/DinoOL"));
	this->setFixedSize(203, 79);
	//this->setWindowFlag(Qt::FramelessWindowHint); /* 注意：如果单纯开启窗口透明层效果，在Windows系统中必须设置, 其他系统可忽略。*/
	//this->setAttribute(Qt::WA_TranslucentBackground);

}

Loading::~Loading()
{

}

void Loading::Start()
{
	Mdino.setFileName(":pic/gif/dino_run");
	Mroad.setFileName(":pic/gif/roadloop");
	Mbird.setFileName(":pic/gif/tbird");
	ui.dino->setMovie(&Mdino);
	ui.road->setMovie(&Mroad);
	ui.bird->setMovie(&Mbird);
	Mdino.start();
	Mroad.start();
	Mbird.start();
	//QTimer::singleShot(600, this, SLOT(close()));
}
