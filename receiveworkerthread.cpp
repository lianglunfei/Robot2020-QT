#include "receiveworkerthread.h"
#include "globaldata.h"
#include "package.h"

#include "debug.h"

ReceiveWorkerThread::ReceiveWorkerThread(QObject *parent)
    : QThread(parent),
      period(100)
{
}

ReceiveWorkerThread::~ReceiveWorkerThread()
{
    // Request termination
    requestInterruption();
    quit();
    wait();
}

void ReceiveWorkerThread::run()
{
    while (!isInterruptionRequested())
    {
        QThread::msleep(50); //Time is too long, data will be delayed
        if (global->connectType != NONE_CONNECT || SIMULATE_CONNECT != NONE_CONNECT)
            Package::unpackOperate();
    }
}
