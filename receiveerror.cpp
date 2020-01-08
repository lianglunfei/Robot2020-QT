/*
 * @Author: xingzhang.Wu 
 * @Date: 2019-09-29 09:41:24 
 * @Last Modified by: xingzhang.Wu
 * @Last Modified time: 2019-09-29 10:19:28
 */
#include "receiveerror.h"
#include "ui_receiveerror.h"

#include "qdebug.h"
#include <QDateTime>

/**
 * @brief Construct a new Receive Error:: Receive Error object
 * 
 * @param parent 
 */
ReceiveError::ReceiveError(QWidget *parent) : QDialog(parent),
                                              ui(new Ui::ReceiveError)
{
    ui->setupUi(this);
    this->setWindowTitle(tr("Receive Error"));
    init();
}

/**
 * @brief 可以根据NODE_NUM自动生成初始化界面
 * 
 */
void ReceiveError::init()
{
    for (int i = 0; i < NODE_NUM; i++)
    {
        ui->comboBox->addItem(QString::number(i + 1));
    }
    ui->comboBox->addItem(tr("all"));
    ui->comboBox->setCurrentIndex(NODE_NUM);

    vLayout = new QVBoxLayout();
    hLayout1 = new QHBoxLayout();
    for (int i = 0; i < NODE_NUM; i++)
    {
        nodeName[i] = new QLabel();
        nodeName[i]->setText("J" + QString::number(i + 1));
    }
    nodeName[NODE_NUM] = new QLabel();
    nodeName[NODE_NUM]->setText("");

    hLayout2 = new QHBoxLayout();
    for (int i = 0; i < NODE_NUM; i++)
    {
        nodeNum[i] = new QLabel();
        nodeNum[i]->setNum(0);
    }
    nodeNum[NODE_NUM] = new QLabel();
    nodeNum[NODE_NUM]->setText("丢失数");

    hLayout3 = new QHBoxLayout();
    for (int i = 0; i < NODE_NUM; i++)
    {
        nodeMaxTime[i] = new QLabel();
        nodeMaxTime[i]->setNum(0);
    }
    nodeMaxTime[NODE_NUM] = new QLabel();
    nodeMaxTime[NODE_NUM]->setText("最大时");

    hLayout4 = new QHBoxLayout();
    for (int i = 0; i < NODE_NUM; i++)
    {
        nodeStatus[i] = new QLabel();
        nodeStatus[i]->setNum(0);
    }
    nodeStatus[NODE_NUM] = new QLabel();
    nodeStatus[NODE_NUM]->setText("状态值");

    hLayout1->addWidget(nodeName[NODE_NUM]);
    hLayout2->addWidget(nodeNum[NODE_NUM]);
    hLayout3->addWidget(nodeMaxTime[NODE_NUM]);
    hLayout4->addWidget(nodeStatus[NODE_NUM]);
    for (int i = 0; i < NODE_NUM; i++)
    {
        hLayout1->addWidget(nodeName[i]);
        hLayout2->addWidget(nodeNum[i]);
        hLayout3->addWidget(nodeMaxTime[i]);
        hLayout4->addWidget(nodeStatus[i]);
    }

    vLayout->addLayout(hLayout1);
    vLayout->addLayout(hLayout2);
    vLayout->addLayout(hLayout3);
    vLayout->addLayout(hLayout4);
    ui->frame->setLayout(vLayout);

    ui->pushButton->setText("重置@" + QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss"));

    time = new QTimer(this);
    connect(time, SIGNAL(timeout()), this, SLOT(update()));
    time->start(0);

    for (int i = 0; i < NODE_NUM; i++)
        start[i].start();
}

/**
 * @brief 从接收到的CAN信息里面判断错误信息，更新到界面显示
 * 
 * @param i 第i个关节
 */
void ReceiveError::errorHandle(int i)
{
    if (lastRunningJoint[i] && !globalData->runningId[i]) {
        ui->lcdNumber->display(++countLostAll);
        nodeNum[i]->setNum(++countLostId[i]);
        start[i].start();
    } else if (!lastRunningJoint[i] && globalData->runningId[i] && countLostId[i]) {
        int elapsedTime = start[i].elapsed();
        maxTime[i] = elapsedTime > maxTime[i] ? elapsedTime : maxTime[i];
        nodeMaxTime[i]->setNum(maxTime[i]);
    }
    if (globalData->runningId[i]) {
        nodeMaxTime[i]->setStyleSheet("color:black;");
    } else {
        nodeMaxTime[i]->setStyleSheet("color:red;");
    }
    if ((lastStatusId[i] == 0x06) &&
        (globalData->statusId[i] != -1 && globalData->statusId[i] != 0x06)) //不等于-1代表接收到了数据，不等于0x06代表出现了异常
    {
        countErrorId[i]++;
        qDebug() << "error: " << globalData->statusId[i];
        emit jointError();
    }
    lastRunningJoint[i] = globalData->runningId[i];
    lastStatusId[i] = globalData->statusId[i];
    nodeStatus[i]->setText(QString::number(globalData->statusId[i]) + "(" + QString::number(countErrorId[i]) + ")");
    if (globalData->statusId[i] == 0x0b) {
        nodeStatus[i]->setStyleSheet("color:red;");
    } else {
        nodeStatus[i]->setStyleSheet("color:black;");
    }
}

/**
 * @brief 定时处理错误信息，并更新界面
 * 
 */
void ReceiveError::update()
{
    if (ui->comboBox->currentIndex() < NODE_NUM)
    {
        int j = ui->comboBox->currentIndex();
        errorHandle(j);
    }
    else
    {
        for (int i = 0; i < NODE_NUM; i++)
        {
            errorHandle(i);
        }
    }
}

/**
 * @brief Destroy the Receive Error:: Receive Error object
 * 
 */
ReceiveError::~ReceiveError()
{
    delete time;
    for (int i = 0; i < NODE_NUM + 1; i++)
    {
        delete nodeNum[i];
        delete nodeName[i];
        delete nodeMaxTime[i];
        delete nodeStatus[i];
    }
    delete hLayout1;
    delete hLayout2;
    delete hLayout3;
    delete hLayout4;
    delete ui;
}

/**
 * @brief 重置按钮
 * 
 */
void ReceiveError::on_pushButton_clicked()
{
    countLostAll = 0;
    for (int i = 0; i < NODE_NUM; i++)
    {
        countLostId[i] = 0;
        countErrorId[i] = 0;
        maxTime[i] = 0;
        nodeNum[i]->setNum(0);
        nodeMaxTime[i]->setNum(0);
    }
    ui->lcdNumber->display(countLostAll);
    ui->pushButton->setText("重置@" + QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss"));
}
