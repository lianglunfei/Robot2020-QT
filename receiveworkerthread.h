#ifndef RECEIVEWORKERTHREAD_H
#define RECEIVEWORKERTHREAD_H

#include <QThread>

class ReceiveWorkerThread : public QThread
{
    Q_OBJECT
public:
    explicit ReceiveWorkerThread(QObject *parent = 0);
    ~ReceiveWorkerThread();

protected:
    virtual void run() Q_DECL_OVERRIDE;

private:
    unsigned char period;
};

#endif // RECEIVEWORKERTHREAD_H
