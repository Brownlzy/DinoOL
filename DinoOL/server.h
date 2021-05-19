#pragma once

#include <QDialog>
#include "ui_server.h"

class Server : public QDialog
{
	Q_OBJECT

public:
	Server(QWidget *parent = Q_NULLPTR);
	~Server();

private:
	Ui::Server ui;
};
