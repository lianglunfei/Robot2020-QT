#include "globaldata.h"
// 直接初始化静态成员变量
CanAnalysis GlobalData::currentCanAnalyticalData[NODE_NUM];
int GlobalData::connectType=NONE_CONNECT;
QStringList GlobalData::currentCanData;
bool GlobalData::showDebugInfo=false;
int GlobalData::runningId[NODE_NUM]={0};
unsigned int GlobalData::sendId[NODE_NUM] = {0x01, 0x02, 0x03, 0x04,
                                             0x05, 0x06, 0x07, 0x08,
                                             0x09, 0x0A, 0x0B, 0x0C}; //joint module
//                                    0x0D,0x0F};//car wheel

GlobalData::GlobalData()
{

}
