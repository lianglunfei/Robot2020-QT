#ifndef DATASERVER_H
#define DATASERVER_H

#include <QObject>
#include <QTcpSocket>
#include <QTcpServer>
#include <QTimer>
#include "tcpreadthread.h"

class DataServer : public QTcpServer
{
    Q_OBJECT
public:
    explicit DataServer(QObject *parent = nullptr);
    ~DataServer();
    void start(); //可以供外界实例化后调用

signals:

private slots:
    void onTcpReadThread();
    void tcpWriteFunc();

protected:
    void incomingConnection(qintptr handle);

private:
    QTcpSocket *client;
    qintptr descriptor;
    TcpReadThread *thread;
    QTimer *tcpWriteTimer;
    bool serverConnectFlag;
};

#endif // DATASERVER_H
