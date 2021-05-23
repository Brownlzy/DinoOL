#pragma once

#include <QMainWindow>
#include "ui_chklcs.h"

class ChkLcs : public QMainWindow
{
	Q_OBJECT

public:
	ChkLcs(QWidget* parent = Q_NULLPTR);
	~ChkLcs();

private:
	Ui::ChkLcs ui;
};
