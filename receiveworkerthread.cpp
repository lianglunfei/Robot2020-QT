/*
 * @Author: xingzhang.Wu 
 * @Date: 2019-09-29 10:04:43 
 * @Last Modified by: xingzhang.Wu
 * @Last Modified time: 2019-09-29 10:28:51
 */
#include "receiveworkerthread.h"
#include "globaldata.h"
#include "package.h"

#include "debug.h"

/**
 * @brief Construct a new Receive Worker Thread:: Receive Worker Thread object
 * 
 * @param parent 
 */
ReceiveWorkerThread::ReceiveWorkerThread(QObject *parent)
    : QThread(parent),
      period(100)
{
}

/**
 * @brief Destroy the Receive Worker Thread:: Receive Worker Thread object
 * 
 */
ReceiveWorkerThread::~ReceiveWorkerThread()
{
    // Request termination
    requestInterruption();
    quit();
    wait();
}

/**
 * @brief 线程循环函数
 * 
 */
void ReceiveWorkerThread::run()
{
    while (!isInterruptionRequested())
    {
        QThread::msleep(20); //Time is too long, data will be delayed
        if (global->connectType != NONE_CONNECT || SIMULATE_CONNECT != NONE_CONNECT)
            Package::unpackOperate();
    }
}
