#include "loading.h"

Loading::Loading(QWidget* parent)
	: QDialog(parent)
{
	ui.setupUi(this);
	Start();
	this->setWindowTitle("chrome://dino");
	this->setModal(true);
	this->setWindowFlags(Qt::WindowStaysOnTopHint | Qt::WindowCloseButtonHint);
	this->setWindowIcon(QIcon(":pic/icon/DinoOL"));
	this->setFixedSize(203, 79);
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
}
