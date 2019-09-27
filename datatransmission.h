/*
 * @Author: wuxingzhang
 * @Date: 2019-09-17 19:39:25
 * @LastEditors: wuxingzhang
 * @LastEditTime: 2019-09-27 15:33:15
 * @Description: file content
 */
#ifndef DATATRANSMISSION_H
#define DATATRANSMISSION_H

#include "globaldata.h"

#ifdef USE_ZLG
#include "ControlCANZLG.h"
#else
#include "controlcan.h"
#endif
#include "ecanvci.h"
#include <string.h>
#include <QStringList>

class DataTransmission
{
private:
    DataTransmission();

    static int CANOpenDevice();
    static int InitCANHelper(int connectType, int devIndex, int baud);
    static int StartCANHelper(int connectType, int devIndex);

public:
    static int connectToCan(int &connectType, int baute);
    static int CANCloseDevice(int connectType);
    static int CANTransmit(int connectType, unsigned char *data, int id);
    static int CANTransmitMulti(int connectType, unsigned char data[][8], int id[], int len);
    static int CANReceive(int connectType, QStringList &list, int dataLen[], int id[], unsigned char data[][8]);
};

#endif // DATATRANSMISSION_H
