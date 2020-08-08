/*
 * @Author: xingzhang.Wu 
 * @Date: 2019-09-29 10:02:11 
 * @Last Modified by:   xingzhang.Wu 
 * @Last Modified time: 2019-09-29 10:02:11 
 */
#ifndef DRIVERS_H
#define DRIVERS_H

class Drivers
{
private:
    Drivers();
    friend void Delay_MSec(unsigned int msec);

public:
    static void initJoint();
    static void initMotor();
    static void initValve();
    static void CloseValve();
    static void stopJoint();
    static void calJoint();
    static void faultJoint();
    static void initJoint(int nodeNum);
    static void stopJoint(int nodeNum);
    static void calJoint(int nodeNum);
    static void faultJoint(int nodeNum);
    static void initValve(int nodeNum);
    static void CloseValve(int nodeNum);

};

#endif // DRIVERS_H
