/*
 * @Author: xingzhang.Wu 
 * @Date: 2019-09-29 10:02:07 
 * @Last Modified by:   xingzhang.Wu 
 * @Last Modified time: 2019-09-29 10:02:07 
 */
#include "drivers.h"
#include "globaldata.h"
#include "package.h"

Drivers::Drivers()
{
}

void Drivers::initJoint(int nodeNum)
{
    if (nodeNum < 0 || nodeNum > NODE_NUM - 1)
        return;
    double value = 0;
    Package::packOperate(global->sendId[nodeNum], value, PROTOCOL_TYPE_START);
#if DRIVE_NEW
    Package::packOperate(global->sendId[nodeNum], value, PROTOCOL_TYPE_OPENVALVE);
#endif
}

void Drivers::stopJoint(int nodeNum)
{
    if (nodeNum < 0 || nodeNum > NODE_NUM - 1)
        return;
    double value = 0;
    Package::packOperate(global->sendId[nodeNum], value, PROTOCOL_TYPE_SHUTDOWN);
#if DRIVE_NEW
    Package::packOperate(global->sendId[nodeNum], value, PROTOCOL_TYPE_CLOSEVALVE);
#endif
}

void Drivers::calJoint(int nodeNum)
{
    if (nodeNum < 0 || nodeNum > NODE_NUM - 1)
        return;
    double value = 0;
    Package::packOperate(global->sendId[nodeNum], value, PROTOCOL_TYPE_CAL);
}

void Drivers::initJoint()
{
    double value[NODE_NUM] = {0};
    Package::packOperateMulti(global->sendId, value, NODE_NUM, PROTOCOL_TYPE_START);
#if DRIVE_NEW
    Package::packOperateMulti(global->sendId, value, NODE_NUM, PROTOCOL_TYPE_OPENVALVE);
#endif
}

void Drivers::stopJoint()
{
    double value[NODE_NUM] = {0};
    Package::packOperateMulti(global->sendId, value, NODE_NUM, PROTOCOL_TYPE_SHUTDOWN);
#if DRIVE_NEW
    Package::packOperateMulti(global->sendId, value, NODE_NUM, PROTOCOL_TYPE_CLOSEVALVE);
#endif
}

void Drivers::calJoint()
{
    double value[NODE_NUM] = {0};
    Package::packOperateMulti(global->sendId, value, NODE_NUM, PROTOCOL_TYPE_CAL);
}
