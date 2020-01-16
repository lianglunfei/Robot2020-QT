/*
 * @Author: xingzhang.Wu 
 * @Date: 2019-11-11 16:49:30 
 * @Last Modified by: xingzhang.Wu
 * @Last Modified time: 2019-11-11 19:06:57
 */
#include "tcpreadthread.h"
#include "globaldata.h"

TcpReadThread::TcpReadThread(QTcpSocket* client,  QObject *parent) :
    QThread(parent)
{
    tcpSocket=client;
    connect(this,&TcpReadThread::finished,[=](){
        qDebug()<<"thread finished";
    });
}

/**
 * @brief 解析导航模块等外设发送过来的网络数据
 * TODO: 接收到的数据需要进行下一步处理
 */
void TcpReadThread::run()
{
    qDebug()<<"thread runs";
    qDebug()<<"rthread read data";
    QByteArray s;

    //More processing can be done here.
    s.append(tcpSocket->readAll());

    //更新导航目标数据
    //TODO: 待完善协议
    globalData->navigateData.x = 0;
    globalData->navigateData.y = 0;
    globalData->navigateData.z = 0;
}
