#include "drivers.h"
#include "globaldata.h"
#include "package.h"

Drivers::Drivers()
{

}

void Drivers::initJoint(int nodeNum)
{
    if(nodeNum<0 || nodeNum>NODE_NUM-1)
        return;
    double value=0;
#if DRIVE_NEW
    Package::packOperate(GlobalData::sendId[nodeNum], value, PROTOCOL_TYPE_OPENVALVE);
#endif
    Package::packOperate(GlobalData::sendId[nodeNum], value, PROTOCOL_TYPE_START);
}

void Drivers::stopJoint(int nodeNum)
{
    if(nodeNum<0 || nodeNum>NODE_NUM-1)
        return;
    double value=0;
    Package::packOperate(GlobalData::sendId[nodeNum], value, PROTOCOL_TYPE_SHUTDOWN);
#if DRIVE_NEW
    Package::packOperate(GlobalData::sendId[nodeNum], value, PROTOCOL_TYPE_CLOSEVALVE);
#endif
}

void Drivers::initJoint()
{
    double value[NODE_NUM]={0};
#if DRIVE_NEW
    Package::packOperateMulti(GlobalData::sendId, value, NODE_NUM, PROTOCOL_TYPE_OPENVALVE);
#endif
    Package::packOperateMulti(GlobalData::sendId, value, NODE_NUM, PROTOCOL_TYPE_START);
}

void Drivers::stopJoint()
{
    double value[NODE_NUM]={0};
    Package::packOperateMulti(GlobalData::sendId, value, NODE_NUM, PROTOCOL_TYPE_SHUTDOWN);
#if DRIVE_NEW
    Package::packOperateMulti(GlobalData::sendId, value, NODE_NUM, PROTOCOL_TYPE_CLOSEVALVE);
#endif
}
