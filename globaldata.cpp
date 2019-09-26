#include "globaldata.h"

GlobalData *GlobalData::instance;
QMutex GlobalData::mutex;

GlobalData::GlobalData()
{
    for (int i = 0; i < NODE_NUM; i++)
    {
        sendId[i] = 0x01 + static_cast<unsigned int>(i);
    }
}
