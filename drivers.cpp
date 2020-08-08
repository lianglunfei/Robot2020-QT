/*
 * @Author: xingzhang.Wu 
 * @Date: 2019-09-29 10:02:07 
 * @Last Modified by: wuzhang.Wu
 * @Last Modified time: 2019-10-09 14:29:30
 */
#include "drivers.h"
#include "globaldata.h"
#include "package.h"

#include "debug.h"
#include <QEventLoop>
#include <QTimer>
#include <QElapsedTimer>

Drivers::Drivers()
{
}

void Delay_MSec(unsigned int msec)
{
    QEventLoop loop;//定义一个新的事件循环
    QTimer::singleShot(msec, &loop, SLOT(quit()));//创建单次定时器，槽函数为事件循环的退出函数
    loop.exec();//事件循环开始执行，程序会卡在这里，直到定时时间到，本循环被退出
}

/**
 * @brief 初始化单个电机
 * 
 * @param nodeNum 
 */
void Drivers::initJoint(int nodeNum)
{
    if (nodeNum < 0 || nodeNum > NODE_NUM - 1)
        return;
    double value = 0; //数据位为0
    Package::packOperate(globalData->sendId[nodeNum], value, PROTOCOL_TYPE_START);
#if DRIVE_NEW
    Package::packOperate(globalData->sendId[nodeNum], value, PROTOCOL_TYPE_OPENVALVE);
#endif
}

/**
 * @brief 停止单个电机
 * 
 * @param nodeNum 
 */
void Drivers::stopJoint(int nodeNum)
{
    if (nodeNum < 0 || nodeNum > NODE_NUM - 1)
        return;
    double value = 0; //数据位为0
    Package::packOperate(globalData->sendId[nodeNum], value, PROTOCOL_TYPE_SHUTDOWN);
#if DRIVE_NEW
    Package::packOperate(globalData->sendId[nodeNum], value, PROTOCOL_TYPE_CLOSEVALVE);
#endif
}

/**
 * @brief 校准单个电机
 * 
 * @param nodeNum 
 */
void Drivers::calJoint(int nodeNum)
{
    if (nodeNum < 0 || nodeNum > NODE_NUM - 1)
        return;
    double value = 0; //数据位为0
    Package::packOperate(globalData->sendId[nodeNum], value, PROTOCOL_TYPE_CAL);
}

/**
 * @brief 清除单个电机状态信息
 *
 * @param nodeNum
 */
void Drivers::faultJoint(int nodeNum)
{
    if (nodeNum < 0 || nodeNum > NODE_NUM - 1)
        return;
    double value = 0; //数据位为0
    Package::packOperate(globalData->sendId[nodeNum], value, PROTOCOL_TYPE_FAULT);
}

/**
 * @brief 打开单个电机电磁阀
 *
 * @param nodeNum
 */
void Drivers::initValve(int nodeNum)
{
    if (nodeNum < 0 || nodeNum > NODE_NUM - 1)
        return;
    double value = 0; //数据位为0
    Package::packOperate(globalData->sendId[nodeNum], value, PROTOCOL_TYPE_OPENVALVE);
}


/**
 * @brief 关闭单个电机电磁阀
 *
 * @param nodeNum
 */
void Drivers::CloseValve(int nodeNum)
{
    if (nodeNum < 0 || nodeNum > NODE_NUM - 1)
        return;
    double value = 0; //数据位为0
    Package::packOperate(globalData->sendId[nodeNum], value, PROTOCOL_TYPE_CLOSEVALVE);
}




/**
 * @brief 初始化所有电机
 * ! 如果是旧的关节，需要进行校准操作
 */
void Drivers::initJoint()
{
    double value[NODE_NUM] = {0}; //数据位为0
#ifndef DELAY_INIT
    Package::packOperateMulti(globalData->sendId, value, NODE_NUM, PROTOCOL_TYPE_START);
    // Delay_MSec(200);
#ifdef DRIVE_NEW
    Package::packOperateMulti(globalData->sendId, value, NODE_NUM, PROTOCOL_TYPE_OPENVALVE);
#endif
#else
    for (int i = 0; i < NODE_NUM; i++)
    {
        Package::packOperate(globalData->sendId[i], value[i], PROTOCOL_TYPE_START);
        Package::packOperate(globalData->sendId[i], value[i], PROTOCOL_TYPE_OPENVALVE);
        QElapsedTimer t;
        t.start();
        Delay_MSec(DELAY_INIT);
        qDebug() << "elapsed: " << t.elapsed();
    }
#endif
}

/**
 * @brief 初始化所有电机
 * ! 如果是旧的关节，需要进行校准操作
 */
void Drivers::initMotor()
{
    double value[NODE_NUM] = {0}; //数据位为0
    Package::packOperateMulti(globalData->sendId, value, NODE_NUM, PROTOCOL_TYPE_START);
}

/**
 * @brief 打开所有电磁阀
 *
 */
void Drivers::initValve()
{
    double value[NODE_NUM] = {0}; //数据位为0
    Package::packOperateMulti(globalData->sendId, value, NODE_NUM, PROTOCOL_TYPE_OPENVALVE);
}

/**
 * @brief 关闭所有电磁阀
 *
 */
void Drivers::CloseValve()
{
    double value[NODE_NUM] = {0}; //数据位为0
    Package::packOperateMulti(globalData->sendId, value, NODE_NUM, PROTOCOL_TYPE_CLOSEVALVE);
}


/**
 * @brief 停止所有电机
 * 
 */
void Drivers::stopJoint()
{
    double value[NODE_NUM] = {0}; //数据位为0
    Package::packOperateMulti(globalData->sendId, value, NODE_NUM, PROTOCOL_TYPE_SHUTDOWN);
#if DRIVE_NEW
    Package::packOperateMulti(globalData->sendId, value, NODE_NUM, PROTOCOL_TYPE_CLOSEVALVE);
#endif
}

/**
 * @brief 校准所有电机
 * 
 */
void Drivers::calJoint()
{
    double value[NODE_NUM] = {0}; //数据位为0
    Package::packOperateMulti(globalData->sendId, value, NODE_NUM, PROTOCOL_TYPE_CAL);
}
