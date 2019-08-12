#ifndef DATATRANSMISSION_H
#define DATATRANSMISSION_H

#include "controlcan.h"
#include "controlcanwin.h"
#include <string.h>
#include <QStringList>

class DataTransmission
{
private:
    DataTransmission();

public:
    static int CANOpenDevice(int connectType);
    static int InitCAN(int connectType, int devIndex, int baud);
    static int StartCAN(int connectType, int devIndex);
    static int CANTransmit(int connectType, unsigned char *data, int id);
    static int CANTransmitMulti(int connectType, unsigned char data[][8], int id[], int len);
    static void CANReceive(int connectType, QStringList &list, int dataLen[], int id[], unsigned char data[][8]);
};

#endif // DATATRANSMISSION_H
