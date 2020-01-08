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

/**
 * @brief Construct a new Receive Frame Box:: Receive Frame Box object
 * 
 * @param parent 
 */
ReceiveFrameBox::ReceiveFrameBox(QWidget *parent) : QGroupBox(parent),
                                                    ui(new Ui::ReceiveFrameBox)
{
    ui->setupUi(this);
    ui->receivedMessagesEdit->document()->setMaximumBlockCount(1000);
    initEvent();
}

/**
 * @brief 接收控件初始化定时器
 * 
 */
void ReceiveFrameBox::initEvent()
{
    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(update()));
    timer->start(100);
}

/**
 * @brief 如果已连接CAN设备并且显示调试信息变量已打开，则将收到的CAN命令在接收框中定时显示出来
 * 
 */
void ReceiveFrameBox::update()
{
    if ((globalData->connectType || SIMULATE_CONNECT != NONE_CONNECT) && globalData->showDebugInfo) {
        QStringList tmp = globalData->currentCanData;
        for (int i = 0; i < tmp.length(); i++) {
            ui->receivedMessagesEdit->append(tmp[i]);
        }
    }
}

/**
 * @brief Destroy the Receive Frame Box:: Receive Frame Box object
 * 
 */
ReceiveFrameBox::~ReceiveFrameBox()
{
    delete ui;
}
