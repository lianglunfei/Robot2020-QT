/*
 * @Author: xingzhang.Wu 
 * @Date: 2019-09-29 10:04:32 
 * @Last Modified by:   xingzhang.Wu 
 * @Last Modified time: 2019-09-29 10:04:32 
 */
#include "receiveframebox.h"
#include "ui_receiveframebox.h"

#include <QTimer>
#include "qdebug.h"
#include "globaldata.h"

ReceiveFrameBox::ReceiveFrameBox(QWidget *parent) : QGroupBox(parent),
                                                    ui(new Ui::ReceiveFrameBox)
{
    ui->setupUi(this);
    ui->receivedMessagesEdit->document()->setMaximumBlockCount(1000);
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
    if ((global->connectType || SIMULATE_CONNECT != NONE_CONNECT) && global->showDebugInfo)
    {
        QStringList tmp = global->currentCanData;
        for (int i = 0; i < tmp.length(); i++)
        {
            ui->receivedMessagesEdit->append(tmp[i]);
        }
    }
}

ReceiveFrameBox::~ReceiveFrameBox()
{
    delete ui;
}
