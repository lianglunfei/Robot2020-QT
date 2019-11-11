#ifndef TCPREADTHREAD_H
#define TCPREADTHREAD_H

#include <QThread>
#include <QTcpSocket>

class TcpReadThread : public QThread
{
    Q_OBJECT
public:
    explicit TcpReadThread(QTcpSocket* client, QObject *parent);
    void run() override;

private:
    QTcpSocket *tcpSocket;
};

#endif // TCPREADTHREAD_H