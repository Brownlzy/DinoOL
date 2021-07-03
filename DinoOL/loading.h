#pragma once

#include <QDialog>
#include <QMovie>
#include <QIcon>
#include <QTimer>
#include "ui_appstart.h"

class Loading : public QDialog
{
	Q_OBJECT

public:
	Loading(QWidget* parent = Q_NULLPTR);
	~Loading();
	void Start();

private:
	Ui::Loading ui;
	QMovie Mdino, Mroad, Mbird;

};
