/*
 * @Author: xingzhang.Wu 
 * @Date: 2019-09-29 10:04:05 
 * @Last Modified by: xingzhang.Wu
 * @Last Modified time: 2019-09-29 10:58:04
 */
#include "protocol.h"
#include <QDebug>

#include "debug.h"
#include "globaldata.h"

#define ECODER_MAX (DRIVE_NEW > 0 ? 524288 : 262144) //2^19
#define POS_MAX (ECODER_MAX / 360.0)
#define SPEED_COEFFICIENT (65536.0 / 4000.0)

Protocol::Protocol()
{
}

/**
 * @brief 根据ID将某个关节的数据放到一维数组的某个位置
 * 
 * @param rawData 从CAN接收到的原始数据
 * @param outData 输出的一维数组
 * @param len 
 * @param id CAN ID
 */
void Protocol::getRawData(unsigned char rawData[], unsigned char outData[], int len, int id)
{
    (void)len;
    int k = 0;
    k = (id - globalData->sendId[0]) * 8;
    for (int j = 0; j < 8; j++)
    {
        outData[k + j] = rawData[j]; //Data received from CAN
    }
}

/**
 * @brief 将原始数据解析成位置信息,2^19/360=1456.36
 * 
 * @param data 原始数据
 * @param index 节点ID，根据该ID可以找到原始数组中对应的值
 * @return double 解析后的位置
 */
double Protocol::parsePos(unsigned char data[], int index)
{
//    if (index == 13)
//        return data[index * 8 + 1] > 127 ? data[index * 8 + 1] - 256 : data[index * 8 + 1];
//    else
        return (data[index * 8 + 1] * 65536 + data[index * 8 + 2] * 256 + data[index * 8 + 3]) / POS_MAX;
}

/**
 * @brief 将原始数据解析成速度信息
 * 
 * @param data 原始数据
 * @param index 节点ID，根据该ID可以找到原始数组中对应的值
 * @return double 解析后的速度
 */
double Protocol::parseSpeed(unsigned char data[], int index)
{
    double res = data[index * 8 + 6] * 256 + data[index * 8 + 7];
    if (res > 32768)
        res -= 65536;
    return res / 10.0;
}

/**
 * @brief 将原始数据解析成电流信息
 * 
 * @param data 原始数据
 * @param index 节点ID，根据该ID可以找到原始数组中对应的值
 * @return double 解析后的电流
 */
double Protocol::parseCurrent(unsigned char data[], int index)
{
    return (data[index * 8 + 4] * 256 + data[index * 8 + 5]) / 10.0;
}

int Protocol::parseStatus(unsigned char data[], int index)
{
    return data[index * 8];
}

/**
 * @brief 将位置信息封装成字节数组，用于CAN信息封装
 * 
 * @param data 原始数据
 * @param value 封装后的数据
 */
void Protocol::packPos(unsigned char data[], double value)
{
    //{0x8a,0x00,0x00,0x00,0x00,0x00,0x00,0x01}; //default position mode
    int readValue;
    if (value > 360)
        value -= 360;
    readValue = static_cast<int>(value * POS_MAX + 0.5);
    readValue = (readValue > ECODER_MAX) ? (readValue - ECODER_MAX) : readValue;
    readValue = readValue < 0 ? (readValue + ECODER_MAX) : readValue;
    data[0] = 0x8a;
    data[1] = readValue / 65536;
    data[2] = (readValue % 65536) / 256;
    data[3] = readValue % 256;
    data[7] = 0x01;
}

/**
 * @brief 将位置信息封装成字节数组，用于CAN信息封装
 * 
 * @param data 原始数据
 * @param value 封装后的数据
 */
void Protocol::packPosRF(unsigned char data[], double value)
{
    //{0x8a,0x00,0x00,0x00,0x00,0x00,0x00,0x01}; //default position mode
    int readValue;
    readValue = static_cast<int>(value + 0.5);
    readValue = readValue < 0 ? (readValue + 255) : readValue;
    readValue = readValue > 255 ? 255 : readValue;
    data[0] = 0x8a;
    data[1] = readValue;
    data[7] = 0x01;
}

/**
 * @brief 将速度信息封装成字节数组，用于CAN信息封装
 * 
 * @param data 原始数据
 * @param value 封装后的数据
 */
void Protocol::packSpeed(unsigned char data[], double value)
{
    //data:65536~4000RPM 65536/4000=16.384
    int readValue;
    readValue = static_cast<int>(value * SPEED_COEFFICIENT + 0.5);
    data[0] = 0x9a;
    if (readValue > 0)
        data[1] = 0x00;
    else
    {
        readValue = -readValue;
        data[1] = 0xff;
    }
    data[2] = readValue / 256;
    data[3] = readValue % 256;
    data[7] = 0x01;
}

/**
 * @brief 将速度信息封装成字节数组，用于CAN信息封装
 * 
 * @param data 原始数据
 * @param value 封装后的数据
 */
void Protocol::packSpeedSet(unsigned char data[], double value)
{
    //data:65536~4000RPM 65536/4000=16.384
    int readValue;
    readValue = static_cast<int>(value * SPEED_COEFFICIENT + 0.5);
    data[0] = 0xca;
    if (readValue > 0)
        data[1] = 0x00;
    else
    {
        readValue = -readValue;
        data[1] = 0xff;
    }
    data[2] = readValue / 256;
    data[3] = readValue % 256;
    data[7] = 0x01;
}

/**
 * @brief 封装校验信息
 * 
 * @param data 原始数据
 * @param value 封装后的数据
 */
void Protocol::packCal(unsigned char data[], double value)
{
    (void)value;
#ifdef DRIVE_NEW
    data[0] = 0xda;
    data[7] = 0x01;
#else
    data[7] = 0x06;
#endif
}

/**
 * @brief 封装启动电机命令
 * 
 * @param data 原始数据
 * @param value 封装后的数据
 */
void Protocol::packStart(unsigned char data[], double value)
{
    (void)value;
    data[0] = 0xaa;
    data[7] = 0x01;
}

/**
 * @brief 封装暂停电机命令
 * 
 * @param data 原始数据
 * @param value 封装后的数据
 */
void Protocol::packPause(unsigned char data[], double value)
{
    (void)value;
    data[0] = 0xaa;
    data[7] = 0x01;
}

/**
 * @brief 封装停止电机命令
 * 
 * @param data 原始数据
 * @param value 封装后的数据
 */
void Protocol::packShutDown(unsigned char data[], double value)
{
    (void)value;
    data[0] = 0xaa;
    data[7] = 0x02;
}


/**
 * @brief 封装打开阀门命令
 * 
 * @param data 原始数据
 * @param value 封装后的数据
 */
void Protocol::packOpenValve(unsigned char data[], double value)
{
    (void)value;
    data[0] = 0xea;
}


/**
 * @brief 封装关闭阀门命令
 * 
 * @param data 原始数据
 * @param value 封装后的数据
 */
void Protocol::packCloseValve(unsigned char data[], double value)
{
    (void)value;
    data[0] = 0xfa;
}

/**
 * @brief 轮子控制，暂时未启用
 * 
 * @param data 
 * @param value 
 */
void Protocol::packWheelReset(unsigned char data[], double value)
{
    (void)value;
    data[0] = 0x55;
    data[1] = 0x55;
    data[2] = 0x55;
    data[3] = 0x55;
    data[4] = 0x55;
    data[5] = 0x55;
    data[6] = 0x55;
    data[7] = 0x55;
}

/**
 * @brief 轮子控制，暂时未启用
 * 
 * @param data 
 * @param value 
 */
void Protocol::packWheelSelect(unsigned char data[], double value)
{
    (void)value;
    data[0] = 0x01;
    data[1] = 0x55;
    data[2] = 0x55;
    data[3] = 0x55;
    data[4] = 0x55;
    data[5] = 0x55;
    data[6] = 0x55;
    data[7] = 0x55;
}

/**
 * @brief 轮子控制，暂时未启用
 * 
 * @param data 
 * @param value 
 */
void Protocol::packWheelSpeed(unsigned char data[], double value)
{
    int readValue;
    readValue = static_cast<int>(value + 0.5);
    data[0] = (unsigned char)(readValue >> 8) & 0xff;
    data[1] = (unsigned char)readValue & 0xff;
    data[2] = 0x55;
    data[3] = 0x55;
    data[4] = 0x55;
    data[5] = 0x55;
    data[6] = 0x55;
    data[7] = 0x55;
}

/**
 * @brief 轮子控制，暂时未启用
 * 
 * @param data 
 * @param value 
 */
void Protocol::packWheelRef(unsigned char data[], double value)
{
    (void)value;
    data[0] = 0x8a;
    data[1] = 0x00;
    data[2] = 0x55;
    data[7] = 0x01;
}

/**
 * @brief 暂时未启用
 * 
 * @param data 
 * @param value 
 */
void Protocol::packSpdPaw(unsigned char data[], double value)
{
    data[0] = 0x9a;
    if (value > 0)
    {
        data[1] = 0x00;
        data[2] = 0x01;
    }
    else
    {
        data[1] = 0xFF;
        data[2] = 0x02;
    }
    data[7] = 0x01;
}

/**
 * @brief 可重构平台直流电机速度控制
 *
 * @param data
 * @param value
 */
void Protocol::packRePlatformSpeed(unsigned char data[],unsigned int currentId, double value)
{
    int readValue;
    readValue = static_cast<int>(value + 0.5);
    data[0] = 0x08;
    data[1] = currentId;
    data[2] = 0x90;
    data[3] = 0x00;
    data[4] = readValue % 256;
    data[5] = readValue / 256;
    data[6] = 0x00;
    data[7] = 0x00;
}

/**
 * @brief 可重构平台直流电机位置控制
 *
 * @param data
 * @param value
 */
void Protocol::packRePlatformPos(unsigned char data[],unsigned int currentId, double value)
{
    int readValue;
    readValue = static_cast<int>(value + 0.5);
    data[0] = 0x08;
    data[1] = currentId;
    data[2] = 0x90;
    data[3] = 0x00;
    data[4] = 0x00;
    data[5] = 0x00;
    data[6] = 0x00;
    data[7] = 0x00;
}

/**
 * @brief 可重构臂杆电机速度控制
 *
 * @param data
 * @param value
 */
void Protocol::packReconfigSpeed(unsigned char data[],double value)
{
    int readValue;
    readValue = static_cast<int>(value + 0.5);
    data[0] = 0x9A;
    if (readValue > 0)
        data[1] = 0x00;
    else
    {
        readValue = -readValue;
        data[1] = 0xff;
    }
    data[2] = readValue / 256;
    data[3] = readValue % 256;
    data[4] = 0x00;
    data[5] = 0x00;
    data[6] = 0x00;
    data[7] = 0x01;
}

/**
 * @brief 可重构臂杆电机位置控制
 *
 * @param data
 * @param value
 */
void Protocol::packReconfigPos(unsigned char data[],double value)
{
    int readValue;
    readValue = static_cast<int>(value + 0.5);
    data[0] = 0x8A;
    data[1] = 0x00;
    data[2] = readValue / 256;
    data[3] = readValue % 256;
    data[4] = 0x00;
    data[5] = 0x00;
    data[6] = 0x00;
    data[7] = 0x01;
}
