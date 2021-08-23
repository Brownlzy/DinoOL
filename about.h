/*
* 2021年6月27日17点45分
* By：Brownlzy
*/

#pragma once

#include <QDialog>
#include "ui_about.h"
#include "update.h"
#include <QCloseEvent>
#include <QPropertyAnimation>
#include <QTimer>

class About : public QDialog
{
	Q_OBJECT

private:
	QPropertyAnimation* pUpdateFrm = nullptr;
public:
	About(QWidget* parent = Q_NULLPTR);
	~About();
	Ui::About ui;
	Update* pUpdate = nullptr;
	void closeEvent(QCloseEvent* event);

public slots:
	int CheckUpdate();
	void ShowUpdateInfo();
	void btnDoUpdate();

};
