#pragma once

#include <QObject>
#include <QtGui/QMovie>
#include <QtWidgets/QLabel>
#include <QtCore/QTimer>
#include <QtCore/QPropertyAnimation>

class Dino : public QObject
{
	Q_OBJECT

private:
public:
	QLabel labDino;
	QLabel labP;
	QTimer ptdino;
	QMovie movie;
	QPropertyAnimation* pAnima = NULL;
	double vy0 = 542.2;
	double vx0 = 305.6;
	double G = 980.0;
	int tms = 10;
	int isOn = 0;
	int fx;
	int fy;
	int WebGame = 0;
	double vy = 0;
	double vx = 0;
	bool isDive = 0;
	int isMove = 0;
	bool isJump = 0;
	int isFail = 0;
	int isShining = 0;

	Dino(int x, int y, QObject* parent, QWidget* dinop);
	~Dino();
	void startAnima();
	bool isOnGround(double horline = -1);
	void setDinoState(QString);
	void move(int x, int y);
	void moveP();
	void setSize(int width, int height);
	void setGeometry(int x, int y, int width, int height);
	void setfxy(int x, int y);
	void start();
	void shining(int state = 0);
	void stop();
	void Pause();
	void adjustSize();
	void setScaledContents(bool);
	void changeP(QString);
	void debug();
	void show(bool Dino, int Ptag = 0);
	int x() const;
	int y() const;
	int width() const;
	int height() const;
	int getP();
	void keyPR(int key, int isPress);

public slots:
	void printDino();
	void showP();
	void CancleShine();

};
