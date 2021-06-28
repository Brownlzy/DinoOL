/*
* 2021年6月27日17点45分
* By：Brownlzy
*/

#pragma once

#include <QDialog>
#include <fstream>
#include "ui_about.h"
#include <QtNetwork>
#include <QProcess>

class About : public QDialog
{
	Q_OBJECT

private:
	QNetworkAccessManager* manager;//请求网络的对象
	QNetworkReply* reply;//出来返回结果的对象
	QUrl url;   //存储网络地址
	QFile* file;  //文件指针
	void Update();
public:
	About(QWidget* parent = Q_NULLPTR);
	~About();
	void startRequest(QUrl url); //请求链接
	Ui::About ui;
	QString strCRC32 = "";

private slots:
	void on_pushButton_2_clicked();
	void httpFinished();  //完成下载后的处理
	void httpReadyRead();  //接收到数据时的处理
	void updateDataReadProgress(qint64, qint64); //更新进度条

};

QString calcFileCRC(QString fileName);
quint32 calcCRC32(const QByteArray& data);
