/*
 * @Author: xingzhang.Wu 
 * @Date: 2019-09-29 10:02:14 
 * @Last Modified by:   xingzhang.Wu 
 * @Last Modified time: 2019-09-29 10:02:14 
 */
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
