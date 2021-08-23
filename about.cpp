/*
* 2021年6月27日17点45分
* By：Brownlzy
*/

#include "about.h"

About::About(QWidget* parent)
	: QDialog(parent)
{
	ui.setupUi(this);
#ifdef _DEBUG
	ui.label_2->setText(QString("DinoOLv") + VERSION + ".DEBUG");
#else
	ui.label_2->setText(QString("DinoOLv") + VERSION);
#endif // _DEBUG
	pUpdateFrm = new QPropertyAnimation(ui.frmUpdateInfo, "pos", this);
	connect(ui.btnShowUpdateInfo, SIGNAL(clicked()), this, SLOT(ShowUpdateInfo()));
	connect(ui.btnDoUpdate, SIGNAL(clicked()), this, SLOT(btnDoUpdate()));
}

About::~About()
{
}

int About::CheckUpdate()
{
	if (!Update::isChecked)
	{
		switch (pUpdate->CheckUpdate())
		{
		case Update::InfoError:
			ui.btnDoUpdate->setEnabled(true);
			ui.btnDoUpdate->setText("检查更新");
			break;
		case Update::MustUpdate:
			pUpdate->doUpdate();
		case Update::NotLasted:
			ui.btnDoUpdate->setEnabled(true);
			ui.btnDoUpdate->setText("立即更新");
		case Update::CantAuto:
			if (ui.btnShowUpdateInfo->text() == "U")
			{
				ShowUpdateInfo();
			}
			break;
		default:
			break;
			//case Update::DownFail:
			//	break;
			//case Update::Crc32Error:
			//	break;
			//case Update::FileOperateError:
			//	break;
		}
	}
	else
	{
		ui.btnDoUpdate->setEnabled(true);
		if (ui.btnShowUpdateInfo->text() == "U" && !pUpdate->isLatest && !pUpdate->isError)
		{
			ShowUpdateInfo();
		}
	}
	return 0;
}

void About::ShowUpdateInfo()
{
	if (ui.btnShowUpdateInfo->text() == "U")
	{
		ui.btnShowUpdateInfo->setText("D");
		pUpdateFrm->stop();
		pUpdateFrm->setDuration(700);
		pUpdateFrm->setStartValue(QPoint(ui.frmUpdateInfo->x(), ui.frmUpdateInfo->y()));
		pUpdateFrm->setEndValue(QPoint(10, 0));
		pUpdateFrm->setEasingCurve(QEasingCurve::InOutQuad);
		pUpdateFrm->start();
	}
	else
	{
		ui.btnShowUpdateInfo->setText("U");
		pUpdateFrm->stop();
		pUpdateFrm->setDuration(700);
		pUpdateFrm->setStartValue(QPoint(ui.frmUpdateInfo->x(), ui.frmUpdateInfo->y()));
		pUpdateFrm->setEndValue(QPoint(10, 112));
		pUpdateFrm->setEasingCurve(QEasingCurve::InOutQuad);
		pUpdateFrm->start();
	}
}

void About::btnDoUpdate()
{
	if (pUpdate->isChecked && pUpdate->isError)
	{
		pUpdate->isChecked = false;
		ui.btnDoUpdate->setEnabled(false);
		QTimer::singleShot(500, this, SLOT(CheckUpdate()));
	}
	else
	{
		ui.label_3->setToolTip("");
		//ui.btnDoUpdate->setEnabled(false);
		pUpdate->doUpdate();
	}
}

void About::closeEvent(QCloseEvent* event)
{
	event->ignore();
	this->hide();
}
