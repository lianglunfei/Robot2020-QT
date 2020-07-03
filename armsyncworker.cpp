#include "armsyncworker.h"
#include "kinemetics.h"
#include "Eigen/Core"
#include "Eigen/Dense"

ArmSyncWorker::ArmSyncWorker(QObject *parent) : QObject(parent)
{
    tcpServer = new QTcpServer(this);
    //断开连接消息
    if(!tcpServer->listen(QHostAddress::Any, 22345))
    {
        qDebug()<<"TCP listen failed.";
         return;
    }
    connect(tcpServer, SIGNAL(newConnection()), this, SLOT(ServerNewConnection()));

}

ArmSyncWorker::~ArmSyncWorker()
{
}

void ArmSyncWorker::ServerNewConnection()
{
    tcpSocket = tcpServer->nextPendingConnection();
    connect(tcpSocket,&QTcpSocket::readyRead,
                           [=]()
                           {
                            //从通讯套接字中取出内容
                            QByteArray b = tcpSocket->readAll();
                            // QByteArray b = tcpSocket->read(sizeof (rawData)+256);
                            memcpy(&recv, b.data(), sizeof (rawData));
//                            rawData* rd = ((rawData*)b.data());
//                            qDebug()<<rd->p[0]<<' '<<rd->p[1] <<' '<< rd->p[2];
                            emit recvData(recv);

                        }
                        );

}

// data：控制器数据 
void ArmSyncWorker::convertData(rawData* data)
{

}


