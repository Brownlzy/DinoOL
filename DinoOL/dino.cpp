#include "dino.h"

Dino::Dino(int x, int y, QObject* parent, QWidget* dinop)
	: fx(x), fy(y), QObject(parent), labDino(dinop), labP(dinop)
{
	connect(&ptdino, SIGNAL(timeout()), this, SLOT(printDino()));
}

Dino::~Dino() {}

bool Dino::isOnGround(double horline)
{
	if (horline < 0) horline = 0.6 * fy;
	double ly = labDino.y();
	double lh = labDino.height();
	double tmp = ly + lh - horline;
	if (tmp <= vy0 * tms / 1000.0 && tmp >= 0 - vy0 * tms / 1000.0)
	{
		return true;
	}
	return false;
}

void Dino::setDinoState(QString pic)
{
	if (isShining)
	{
		if (pic.startsWith(":/pic/gif/"))
			pic += "_shine";
		else
			pic = ":/pic/gif/dino_run_shine";
	}
	movie.stop();
	movie.setFileName(pic);
	labDino.setMovie(&movie);
	movie.start();
}
void Dino::setfxy(int x, int y)
{
	fx = x;
	fy = y;
}

void Dino::move(int x, int y)
{
	labDino.move(x, y);
	moveP();
}

void Dino::moveP()
{
	if (!isDive)
		labP.move(labDino.x() + 0.5 * labDino.width() - 10, labDino.y() - 30);
	else
		labP.move(labDino.x() + 0.5 * labDino.width() - 25, labDino.y() - 68);
}

void Dino::setSize(int width, int height)
{
	labDino.setGeometry(labDino.x(), labDino.y(), width, height);
}

void Dino::setGeometry(int x, int y, int width, int height)
{
	labDino.setGeometry(x, y, width, height);
	moveP();
}

void Dino::start()
{
	ptdino.setInterval(tms);
	ptdino.start();
}

void Dino::stop()
{
	ptdino.stop();
}

void Dino::Pause()
{
	ptdino.stop();
	//setDinoState(":/pic/png/dino_fail");
	if (isDive)
	{
		isDive = false;
		setGeometry(labDino.x(), labDino.y() - 38, 88, 98);
		setScaledContents(true);
	}
}

void Dino::shining(int state)
{
	isShining = 1;
	if (!state)
	{
		if (isDive)
		{
			setDinoState(":/pic/gif/dino_dive");
		}
		else
		{
			setDinoState(":/pic/gif/dino_run");
		}
	}
	else if (state == 1)
	{
		setDinoState(":/pic/gif/dino_run");
	}
	else if (state == -1)
	{
		setDinoState(":/pic/gif/dino_dive");
	}
	QTimer::singleShot(3000, this, SLOT(CancleShine()));
}

void Dino::changeP(QString Ptxt)
{
	labP.setText(Ptxt);
	labP.setStyleSheet("font: 14pt ""黑体"";");
	labP.setFrameShape(QFrame::Box);
	labP.adjustSize();
	labP.setGeometry(labP.x(), labP.y(), labP.width() + 20, labP.height());
	labP.setAlignment(Qt::AlignCenter);
}

void Dino::debug()
{
	if (labP.text() != "") labDino.setFrameShape(QFrame::Box);
}

void Dino::startAnima()
{
	if (pAnima == NULL) pAnima = new QPropertyAnimation(this);
	pAnima->setTargetObject(&labDino);
	pAnima->setPropertyName("geometry");
	pAnima->setDuration(700);
	pAnima->setStartValue(QRect(labDino.x(), labDino.y(), labDino.width(), labDino.height()));
	pAnima->setEndValue(QRect(0, 0.6 * fy - 2.0 * labDino.height(), 2 * labDino.width(), 2 * labDino.height()));
	pAnima->start();
}
int Dino::x() const
{
	return labDino.x();
}

int Dino::y() const
{
	return labDino.y();
}

int Dino::width() const
{
	return labDino.width();
}

int Dino::height() const
{
	return labDino.height();
}

void Dino::adjustSize()
{
	labDino.adjustSize();
}

void Dino::setScaledContents(bool isTrue)
{
	labDino.setScaledContents(isTrue);
}

void Dino::show(bool Dino, int Ptag)
{
	if (Dino)
	{
		labDino.show();
	}
	else
	{
		labDino.hide();
		return;
	}
	if (Ptag > 0)
	{
		labP.show();
	}
	else if (Ptag < 0)
	{
		labP.hide();
	}
}
int Dino::getP()
{
	return labP.text().toInt();
}

void Dino::keyPR(int key, int isPress)
{
	bool onG = isOnGround();
	if (isPress)
	{
		if (key == Qt::Key_W && onG)
		{
			start();
			vy += vy0;
			isJump = 1;
		}
		if (key == Qt::Key_A || key == Qt::Key_Left) //A
		{
			start();
			if (onG)
			{
				isMove = -1;
				vx = 0 - vx0;
			}
			else
			{
				isMove = -2;
			}
		}
		if (key == Qt::Key_S || key == Qt::Key_Down)  //S
		{
			if (!isDive)// && !isJump)
			{
				setDinoState(":/pic/gif/dino_dive");
				isDive = true;
				setGeometry(labDino.x(), labDino.y() + 38, 118, 60);
				setScaledContents(true);
			}
		}
		if (key == Qt::Key_D || key == Qt::Key_Right)  //D
		{
			start();
			if (onG)
			{
				isMove = 1;
				vx = vx0;
			}
			else
			{
				isMove = 2;
			}
		}
	}
	else
	{
		if (key == Qt::Key_A || key == Qt::Key_Left) //A
		{
			if (onG)
			{
				vx = 0;
				stop();
			}
			isMove = 0;
		}
		if (key == Qt::Key_D || key == Qt::Key_Right) //D
		{
			if (onG)
			{
				vx = 0;
				stop();
			}
			isMove = 0;
		}
		if (key == Qt::Key_S || key == Qt::Key_Down)  //S
		{
			if (isDive)
			{
				setDinoState(":/pic/gif/dino_run");
				isDive = false;
				setGeometry(labDino.x(), labDino.y() - 38, 88, 98);
				setScaledContents(true);
			}
		}
	}
}

void Dino::CancleShine()
{
	isShining = 0;
	if (isDive)
	{
		setDinoState(":/pic/gif/dino_dive");
	}
	else
	{
		setDinoState(":/pic/gif/dino_run");
	}
}

void Dino::showP()
{
	show(1, 1);
	moveP();
	changeP("P1");
}

void Dino::printDino()
{
	if (labDino.x() < 0) { vx = fabs(vx); isMove = fabs(isMove); }
	if (!WebGame && labDino.x() > fx - labDino.width()) { vx = 0 - fabs(vx); isMove = 0 - fabs(isMove); }
	if (vy > 0 - vy0 * tms / 2000 || vy < vy0 * tms / 2000 && !isOnGround())
		vy -= G * tms / 1000;
	else if (isOnGround())
	{
		vy = 0;
		if (!isMove)
		{
			vx = 0;
			ptdino.stop();
		}
		labDino.move(labDino.x(), 0.6 * fy - labDino.height());
	}
	if ((isMove >= -1 && isMove <= 1 && !isOnGround()) || isOnGround())
	{
		if (isOnGround() && isMove) vx = isMove / fabs(isMove) * vx0;
		labDino.move(labDino.x() + vx / 1000 * tms, labDino.y() - vy / 1000 * tms);
	}
	else
		labDino.move(labDino.x() + vx / 1000 * tms, labDino.y() - vy / 1000 * tms);
	if (isOnGround() && isJump == 1)
	{
		isJump = 0;
		if (!isDive)
			setDinoState(":/pic/gif/dino_run");
	}
	if (!isOnGround() && isJump == 0)
		isJump = 1;

	if (isOnGround() && isJump == 0)
	{
	}
	else if (!isOnGround() && isJump == 1)
	{
		if (!isDive && !isShining)
			setDinoState(":/pic/png/dino");
	}
	labP.setVisible(true);
	moveP();
}
