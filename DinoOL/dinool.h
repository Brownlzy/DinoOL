#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_dinool.h"

class DinoOL : public QMainWindow
{
    Q_OBJECT

public:
    DinoOL(QWidget *parent = Q_NULLPTR);

public slots:
    void NetworkChk(QString);

protected:
    virtual void resizeEvent(QResizeEvent* event) override;

private:
    Ui::DinoOLClass ui;

};
