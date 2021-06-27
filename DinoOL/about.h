#pragma once

#include <QDialog>
#include "ui_about.h"

class About : public QDialog
{
	Q_OBJECT

private:
public:
	About(QWidget* parent = Q_NULLPTR);
	~About();

	Ui::About ui;
};
