#include "drivers.h"
#include "globaldata.h"
#include "package.h"

Drivers::Drivers()
{

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
