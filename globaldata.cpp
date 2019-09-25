#include "globaldata.h"
// 直接初始化静态成员变量
CanAnalysis GlobalData::currentCanAnalyticalData[NODE_NUM];
int GlobalData::connectType=NONE_CONNECT;
QStringList GlobalData::currentCanData;
bool GlobalData::showDebugInfo=false;
int GlobalData::runningId[NODE_NUM]={0};
unsigned int GlobalData::sendId[NODE_NUM] = {0};
int GlobalData::statusId[NODE_NUM]={0};

GlobalData::GlobalData()
{
}

void GlobalData::init()
{
    for(int i=0;i<NODE_NUM;i++) {
        sendId[i] = 0x01+static_cast<unsigned int>(i);
    }
}
