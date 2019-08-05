#include "receiveframebox.h"
#include "ui_receiveframebox.h"

#include <QTimer>
#include "qdebug.h"
#include "globaldata.h"

ReceiveFrameBox::ReceiveFrameBox(QWidget *parent) :
    QGroupBox(parent),
    ui(new Ui::ReceiveFrameBox)
{
    ui->setupUi(this);
    initEvent();
}

void ReceiveFrameBox::initEvent()
{
    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(update()));
    timer->start(100);
}

/**
*@projectName   RobotControlSystem
*@brief         如果已连接CAN设备并且显示调试信息变量已打开，则将收到的CAN命令在接收框中定时显示出来
*@parameter
*@author        XingZhang.Wu
*@date          20190801
**/
void ReceiveFrameBox::update()
{
    if(GlobalData::connectType && GlobalData::showDebugInfo) {
        for(int i=0;i<GlobalData::currentCanData.length();i++) {
            ui->receivedMessagesEdit->append(GlobalData::currentCanData[i]);
        }
        GlobalData::currentCanData.clear();
    }
}

ReceiveFrameBox::~ReceiveFrameBox()
{
    delete ui;
}
