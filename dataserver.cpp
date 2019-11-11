/*
 * @Author: xingzhang.Wu 
 * @Date: 2019-11-11 16:49:35 
 * @Last Modified by: xingzhang.Wu
 * @Last Modified time: 2019-11-11 18:20:15
 */
#include "dataserver.h"
#include "globaldata.h"
#include "debug.h"

#define PERIOD 20
#define SERVER_IP "192.168.137.134"
#define SERVER_PORT 2019

DataServer::DataServer(QObject *parent) : QTcpServer(parent),
                                          serverConnectFlag(false)
{
    client = new QTcpSocket();
    //断开连接消息
    connect(client, &QTcpSocket::disconnected, [=]() {
        serverConnectFlag = false;
        qDebug() << "clientDisconnected";
    });
    //有新的连接
    connect(this, &DataServer::newConnection, [=]() {
        serverConnectFlag = true;
        qDebug() << "new connection";
    });

    tcpWriteTimer = new QTimer();
    tcpWriteTimer->setInterval(PERIOD);
    connect(tcpWriteTimer, SIGNAL(timeout()), this, SLOT(tcpWriteFunc()));

    tcpWriteTimer->start();

    start();
}

/**
 * @brief 启动服务器监听
 * 
 */
void DataServer::start()
{
    if (listen(QHostAddress(SERVER_IP), SERVER_PORT))
    {
        qDebug() << "DataServer listened ok";
    }
    else
    {
        qDebug() << "DataServer linstened failed";
    }
}

/**
 * @brief 当有客户端建立连接时，获取此时的socket，建立信号槽处理机制
 * 
 * @param handle 
 */
void DataServer::incomingConnection(qintptr handle)
{
    client->setSocketDescriptor(handle);
    descriptor = handle;
    qDebug() << handle;
    connect(client, SIGNAL(readyRead()), this, SLOT(onTcpReadThread()));
}

/**
 * @brief 网络接收处理函数
 * 
 */
void DataServer::onTcpReadThread()
{
    thread = new TcpReadThread(client, this);
    qDebug() << descriptor;
    // connect(thread,SIGNAL(readyRead(QByteArray)),this,SLOT(emitReadyRead(QByteArray)));
    connect(thread, SIGNAL(finished()), thread, SLOT(deleteLater()));
    thread->start();
}

/**
 * @brief TCP写函数，默认不断往客户端写姿态数据
 * 协议以start开头，end结尾，用于区分网络流
 * 
 */
void DataServer::tcpWriteFunc()
{
    if (serverConnectFlag)
    {
        QByteArray sendData;
        sendData.append("start");
#if 0
        // sendData.append(QString::number(DataReceive::instance().fLegPos.left_fore_waist));
        // sendData.append(QString::number(DataReceive::instance().fLegPos.left_fore_hip));
        // sendData.append(QString::number(DataReceive::instance().fLegPos.left_fore_knee));

        // sendData.append(QString::number(DataReceive::instance().fLegPos.left_hind_waist));
        // sendData.append(QString::number(DataReceive::instance().fLegPos.left_hind_hip));
        // sendData.append(QString::number(DataReceive::instance().fLegPos.left_hind_knee));

        // sendData.append(QString::number(DataReceive::instance().fLegPos.right_fore_waist));
        // sendData.append(QString::number(DataReceive::instance().fLegPos.right_fore_hip));
        // sendData.append(QString::number(DataReceive::instance().fLegPos.right_fore_knee));

        // sendData.append(QString::number(DataReceive::instance().fLegPos.right_hind_waist));
        // sendData.append(QString::number(DataReceive::instance().fLegPos.right_hind_hip));
        // sendData.append(QString::number(DataReceive::instance().fLegPos.right_hind_knee));
        sendData.append(QString::number(global->currentCanAnalyticalData[0].position));
        sendData.append(QString::number(global->currentCanAnalyticalData[1].position));
        sendData.append(QString::number(global->currentCanAnalyticalData[2].position));

        sendData.append(QString::number(global->currentCanAnalyticalData[3].position));
        sendData.append(QString::number(global->currentCanAnalyticalData[4].position));
        sendData.append(QString::number(global->currentCanAnalyticalData[5].position));

        sendData.append(QString::number(global->currentCanAnalyticalData[6].position));
        sendData.append(QString::number(global->currentCanAnalyticalData[7].position));
        sendData.append(QString::number(global->currentCanAnalyticalData[8].position));

        sendData.append(QString::number(global->currentCanAnalyticalData[9].position));
        sendData.append(QString::number(global->currentCanAnalyticalData[10].position));
        sendData.append(QString::number(global->currentCanAnalyticalData[11].position));
#else
        sendData.append("10,");
        sendData.append("20,");
        sendData.append("30,");

        sendData.append("10,");
        sendData.append("20,");
        sendData.append("30,");

        sendData.append("10,");
        sendData.append("20,");
        sendData.append("30,");

        sendData.append("10,");
        sendData.append("20,");
        sendData.append("30,");
#endif
        sendData.append("end");
        client->write(sendData);
    }
}

/**
 * @brief Destroy the Data Server:: Data Server object
 * 
 */
DataServer::~DataServer()
{
    delete client;
    delete tcpWriteTimer;
}
