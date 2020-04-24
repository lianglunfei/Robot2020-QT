#include "armsyncworker.h"

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
                            QByteArray b = tcpSocket->read(sizeof (rawData));
                            convertData((rawData*)b.data());
                        }
                        );

}

// 控制器-机械臂关节角转换函数
// data：控制器数据 
void ArmSyncWorker::convertData(rawData* data)
{

    // 不做变换
    double converted_data[6] = {
        data->x0,
        data->x1,
        data->x2,
        data->x3,
        data->x4,
        data->x5
    };

    // 转换完成，通过signal将数据发给控制类
    emit recvData(converted_data);
}


