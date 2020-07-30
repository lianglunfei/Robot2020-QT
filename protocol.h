/*
 * @Author: xingzhang.Wu 
 * @Date: 2019-09-29 10:04:09 
 * @Last Modified by:   xingzhang.Wu 
 * @Last Modified time: 2019-09-29 10:04:09 
 */
#ifndef PROTOCOL_H
#define PROTOCOL_H

class Protocol
{
public:
    Protocol();

    static void getRawData(unsigned char rawData[], unsigned char outData[], int len, int id);
    static double parsePos(unsigned char data[], int index);
    static double parseSpeed(unsigned char data[], int index);
    static double parseCurrent(unsigned char data[], int index);
    static int parseStatus(unsigned char data[], int index);
    static void packPos(unsigned char data[], double value);
    static void packPosRF(unsigned char data[], double value);
    static void packSpeed(unsigned char data[], double value);
    static void packSpeedSet(unsigned char data[], double value);
    static void packCal(unsigned char data[], double value);
    static void packStart(unsigned char data[], double value);
    static void packPause(unsigned char data[], double value);
    static void packShutDown(unsigned char data[], double value);
    static void packOpenValve(unsigned char data[], double value);
    static void packCloseValve(unsigned char data[], double value);
    static void packWheelReset(unsigned char data[], double value);
    static void packWheelSelect(unsigned char data[], double value);
    static void packWheelSpeed(unsigned char data[], double value);
    static void packWheelRef(unsigned char data[], double value);
    static void packSpdPaw(unsigned char data[], double value);
    static void packRePlatformPos(unsigned char data[], unsigned int currentId,double value);  // 可重构平台位置数据包
    static void packRePlatformSpeed(unsigned char data[],unsigned int currentId, double value);    //可重构平台速度数据包
    static void packReconfigPos(unsigned char data[], double value);  // 可重构臂杆位置数据包
    static void packReconfigSpeed(unsigned char data[], double value);    //可重构臂杆速度数据包
};

#endif // PROTOCOL_H
