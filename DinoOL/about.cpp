#include "about.h"

void About::Update()
{
	std::ofstream fbat;
	fbat.open("update.bat");
	fbat << "@echo off\n";
	fbat << "taskkill /pid " << qApp->applicationPid() << " -t -f\n";
	fbat << "del DinoOL.exe\n";
	fbat << "ren DinoOLNew.exe DinoOL.exe\n";
	fbat << "start DinoOL.exe\n";
	fbat << "del update.bat\n";
	fbat.close();
	QProcess::startDetached("update.bat");
	//qApp->exit(-3);
}

About::About(QWidget* parent)
	: QDialog(parent)
{
	ui.setupUi(this);
	manager = new QNetworkAccessManager(this);
	ui.progressBar->hide();
}

About::~About()
{
}

void About::on_pushButton_2_clicked()
{
	url = "https://brownlzy.github.io/DinoOL.exe";
	//测试url;
	//url = "http://dl.360safe.com/drvmgr/guanwang__360DrvMgrInstaller_beta.exe";
	//获取在界面中输入的url地址
	QFileInfo info(url.path());
	QString fileName("DinoOLNew.exe");//(info.fileName());
	//获取文件名
	//if (fileName.isEmpty()) fileName = "index.html";
	//如果文件名为空，则使用“index.html”，
	file = new QFile(fileName);

	if (!file->open(QIODevice::WriteOnly))
	{   //如果打开文件失败，则删除file，并使file指针为0，然后返回
		qDebug() << "file open error";
		delete file;
		file = 0;
		return;
	}
	startRequest(url);  //进行链接请求
	ui.progressBar->setValue(0);  //进度条的值设为0
	ui.progressBar->show();  //显示进度条
	ui.pushButton_2->hide();
}

void About::startRequest(QUrl url)  //链接请求
{
	QNetworkRequest m_request(url);
	QSslConfiguration config;
	QSslConfiguration conf = m_request.sslConfiguration();
	conf.setPeerVerifyMode(QSslSocket::VerifyNone);
	conf.setProtocol(QSsl::TlsV1SslV3);
	m_request.setSslConfiguration(conf);
	reply = manager->get(m_request);
	//
	connect(reply, SIGNAL(finished()), this, SLOT(httpFinished()));//当reply执行完毕后激发httpFinished方法
	//下载完成后
	connect(reply, SIGNAL(readyRead()), this, SLOT(httpReadyRead()));//当读取replay准备ok后就调用httpReadyRead方法读取内容并写入文件
	//并且根据读取进度更新进度条
	connect(reply, SIGNAL(downloadProgress(qint64, qint64)),
		this, SLOT(updateDataReadProgress(qint64, qint64)));

}
void About::httpReadyRead()   //有可用数据
{
	if (file) file->write(reply->readAll());  //如果文件存在，则写入文件
}
void About::updateDataReadProgress(qint64 bytesRead, qint64 totalBytes)
{
	ui.progressBar->setMaximum(totalBytes); //最大值
	ui.progressBar->setValue(bytesRead);  //当前值
}
void About::httpFinished()  //完成下载
{
	ui.progressBar->hide();
	ui.pushButton_2->show();
	file->flush();
	file->close();
	reply->deleteLater();
	reply = 0;
	delete file;
	file = 0;
	Update();
}
