#include "protocol.h"
#include <QDebug>

#include "debug.h"
#include "globaldata.h"

#define ECODER_MAX (DRIVE_NEW>0?524288:262144)//2^19
#define POS_MAX (ECODER_MAX/360.0)
#define SPEED_COEFFICIENT (65536.0/4000.0)

Protocol::Protocol()
{

}

/**
*@projectName   RobotControlSystem
*@brief         根据ID将某个关节的数据放到一维数组的某个位置
*@parameter
*@author        XingZhang.Wu
*@date          20190724
**/
void Protocol::getRawData(unsigned char rawData[], unsigned char outData[], int len, int id)
{
    int k=0;
    k = (id-GlobalData::sendId[0])*8;
    for(int j=0;j<8;j++) {
        outData[k+j]=rawData[j]; //Data received from CAN
    }
}

/**
*@projectName   RobotControlSystem
*@brief         将原始数据解析成位置信息,2^19/360=1456.36
*@parameter
*@author        XingZhang.Wu
*@date          20190724
**/
double Protocol::parsePos(unsigned char data[], int index)
{
    if(index==13)
        return data[index*8+1]>127?data[index*8+1]-256:data[index*8+1];
    else
        return (data[index*8+1]*65536 + data[index*8+2]*256 + data[index*8+3])/POS_MAX;
}

double Protocol::parseSpeed(unsigned char data[], int index)
{
    double res = data[index*8+6]*256 + data[index*8+7];
    if(res > 32768)
        res -= 65536;
    return res/10.0;
}

double Protocol::parseCurrent(unsigned char data[], int index)
{
    return (data[index*8+4]*256 + data[index*8+5])/10.0;
}

int Protocol::parseStatus(unsigned char data[], int index)
{
    return data[index*8];
}

/**
*@projectName   RobotControlSystem
*@brief         将位置信息封装成字节数组，用于CAN信息封装
*@parameter
*@author        XingZhang.Wu
*@date          20190724
**/
void Protocol::packPos(unsigned char data[], double value)
{
    //{0x8a,0x00,0x00,0x00,0x00,0x00,0x00,0x01}; //default position mode
    if(sizeof(data)/sizeof(unsigned char)<8)
        return;
    int readValue;
    readValue = static_cast<int>(value*POS_MAX+0.5);
    readValue = (readValue>ECODER_MAX)?(readValue-ECODER_MAX):readValue;
    readValue = readValue<0?(readValue+ECODER_MAX):readValue;
    data[0] = 0x8a;
    data[1] = readValue/65536;
    data[2] = (readValue%65536)/256;
    data[3] = readValue%256;
    data[7] = 0x01;
}

void Protocol::packPosRF(unsigned char data[], double value)
{
    //{0x8a,0x00,0x00,0x00,0x00,0x00,0x00,0x01}; //default position mode
    if(sizeof(data)/sizeof(unsigned char)<8)
        return;
    int readValue;
    readValue = static_cast<int>(value+0.5);
    readValue = readValue<0?(readValue+255):readValue;
    readValue = readValue>255?255:readValue;
    data[0] = 0x8a;
    data[1] = readValue;
    data[7] = 0x01;
}

void Protocol::packSpeed(unsigned char data[], double value)
{
    //data:65536~4000RPM 65536/4000=16.384
    if(sizeof(data)/sizeof(unsigned char)<8)
        return;
    int readValue;
    readValue = static_cast<int>(value*SPEED_COEFFICIENT+0.5);
    data[0] = 0x9a;
    if(readValue > 0)
        data[1] = 0x00;
    else {
        readValue = -readValue;
        data[1] = 0xff;
    }
    data[2] = readValue/256;
    data[3] = readValue%256;
    data[7] = 0x01;
}

void Protocol::packSpeedSet(unsigned char data[], double value)
{
    //data:65536~4000RPM 65536/4000=16.384
    if(sizeof(data)/sizeof(unsigned char)<8)
        return;
    int readValue;
    readValue = static_cast<int>(value*SPEED_COEFFICIENT+0.5);
    data[0] = 0xca;
    if(readValue > 0)
        data[1] = 0x00;
    else {
        readValue = -readValue;
        data[1] = 0xff;
    }
    data[2] = readValue/256;
    data[3] = readValue%256;
    data[7] = 0x01;
}

void Protocol::packCal(unsigned char data[], double value)
{
    if(sizeof(data)/sizeof(unsigned char)<8)
        return;
#ifdef DRIVE_NEW
    data[0] = 0xda;
    data[7] = 0x01;
#else
    data[7] = 0x06;
#endif
}

void Protocol::packStart(unsigned char data[], double value)
{
    if(sizeof(data)/sizeof(unsigned char)<8)
        return;
    data[0] = 0xaa;
    data[7] = 0x01;
}

void Protocol::packPause(unsigned char data[], double value)
{
    if(sizeof(data)/sizeof(unsigned char)<8)
        return;
    data[0] = 0xaa;
    data[7] = 0x01;
}

void Protocol::packShutDown(unsigned char data[], double value)
{
    if(sizeof(data)/sizeof(unsigned char)<8)
        return;
    data[0] = 0xaa;
    data[7] = 0x02;
}

void Protocol::packOpenValve(unsigned char data[], double value)
{
    if(sizeof(data)/sizeof(unsigned char)<8)
        return;
    data[0] = 0xea;
}

void Protocol::packCloseValve(unsigned char data[], double value)
{
    if(sizeof(data)/sizeof(unsigned char)<8)
        return;
    data[0] = 0xfa;
}

void Protocol::packWheelReset(unsigned char data[], double value)
{
    if(sizeof(data)/sizeof(unsigned char)<8)
        return;
    data[0] = 0x55;
    data[1] = 0x55;
    data[2] = 0x55;
    data[3] = 0x55;
    data[4] = 0x55;
    data[5] = 0x55;
    data[6] = 0x55;
    data[7] = 0x55;
}

void Protocol::packWheelSelect(unsigned char data[], double value)
{
    if(sizeof(data)/sizeof(unsigned char)<8)
        return;
    data[0] = 0x01;
    data[1] = 0x55;
    data[2] = 0x55;
    data[3] = 0x55;
    data[4] = 0x55;
    data[5] = 0x55;
    data[6] = 0x55;
    data[7] = 0x55;
}

void Protocol::packWheelSpeed(unsigned char data[], double value)
{
    if(sizeof(data)/sizeof(unsigned char)<8)
        return;
    int readValue;
    readValue = static_cast<int>(value+0.5);
    data[0] = (unsigned char)(readValue>>8)&0xff;
    data[1] = (unsigned char)readValue&0xff;
    data[2] = 0x55;
    data[3] = 0x55;
    data[4] = 0x55;
    data[5] = 0x55;
    data[6] = 0x55;
    data[7] = 0x55;
}

void Protocol::packWheelRef(unsigned char data[], double value)
{
    if(sizeof(data)/sizeof(unsigned char)<8)
        return;
    data[0] = 0x8a;
    data[1] = 0x00;
    data[2] = 0x55;
    data[7] = 0x01;
}

void Protocol::packSpdPaw(unsigned char data[], double value)
{
    if(sizeof(data)/sizeof(unsigned char)<8)
        return;
    data[0] = 0x9a;
    if(value>0) {
        data[1] = 0x00;
        data[2] = 0x01;
    }
    else {
        data[1] = 0xFF;
        data[2] = 0x02;
    }
    data[7] = 0x01;
}
