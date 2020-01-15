/*
 * @Author: xingzhang.Wu
 * @Date: 2019-10-21 16:07:31
 * @Last Modified by: xingzhang.Wu
 * @Last Modified time: 2019-11-11 21:37:47
 */
#include "autocontrol.h"
#include "globaldata.h"
#include "package.h"
#include "debug.h"
#include <cmath>
#include <QElapsedTimer>
#include <QTimer>

#define PI 57.3
#define DEBUG_SIM 0

/**
 * @brief Construct a new Auto Control:: Auto Control object
 * 使用流程：
 * 1. 调用moveBodySet或者moveLegSet函数设置好目标位置、关节Id、运动速度，或者调用autoRunSet启动自动运行任务。
 *    !: moveBodySet或者moveLegSet函数一般用来进行遥控控制，autoRunSet用来自动控制。
 * 2. 调用start启动运行
 * 3. 调用stop停止运行
 * 
 * @param parent 
 */
AutoControl::AutoControl(QObject *parent) : QObject(parent),
                                            period(20), l1(0), l2(0.304), l3(0.277),
                                            flag(0), leg(0), v(0), mode(0), useBody(0), isSetting(0)
{
    timer = new QTimer();
    timer->setTimerType(Qt::PreciseTimer);
    connect(timer, &QTimer::timeout, this, &AutoControl::run); //定时运行运动函数，该函数可以处理一些自动运行的逻辑
    timer->start(period);
}

/**
 * @brief 设置完成后，调用该接口开始运行
 * 
 */
void AutoControl::start()
{
    flag = 0;
}

/**
 * @brief 将连续运动改为点动模式
 * 
 */
void AutoControl::stop()
{
    mode = 0;
}

/**
 * @brief 运动定时函数
 * 
 */
void AutoControl::run()
{
    if (!isSetting) //正在修改设置时不启动运行
    {
        if (this->useBody == 1)
            moveBody();
        else if(this->useBody == 0)
            moveLeg();
        else
            autoRunFunc();
    }
}

/**
 * @brief 启动自动运行任务
 * 
 */
void AutoControl::autoRunSet()
{
    useBody = 2;
}

/**
 * @brief 单独移动一条腿设置
 *
 * @param leg 第几条腿
 * @param changePos 腿部末端位置
 * @param v 运动速度
 * @return void
 */
void AutoControl::moveLegSet(int leg, double changePos[], double v, int mode)
{
    this->isSetting = 1; //正在修改设置时不启动运行
    this->leg = leg;
    for (int i = 0; i < 3; i++)
    {
        this->changePos[i] = changePos[i];
    }
    this->v = v;
    this->mode = mode;
    this->useBody = 0;
    this->isSetting = 0; //修改完成
}

/**
 * @brief 单条腿运动
 * 
 */
void AutoControl::moveLeg()
{
    static double c1 = 0;
    static double c2 = 0;
    static double c3 = 0;
    static double x = 0;
    static double y = 0;
    static double z = 0;
    static double tp = 0;
    static QElapsedTimer t;
    static double currentPos[NODE_NUM];

    if (flag == 0)
    {
        if (NODE_NUM != 12) //目前只对12个关节做处理
            return;
        flag = 1;
        c1 = (globalData->currentCanAnalyticalData[leg].position - globalData->refValue[leg]) / PI;
        c2 = (globalData->currentCanAnalyticalData[leg + 1].position - globalData->refValue[leg + 1]) / PI;
        c3 = (globalData->currentCanAnalyticalData[leg + 2].position - globalData->refValue[leg + 2]) / PI;

        for (int i = 0; i < NODE_NUM; i++)
            currentPos[i] = globalData->currentCanAnalyticalData[i].position;

        x = l2 * sin(c2) + l3 * sin(c2 + c3);
        y = l2 * sin(c1) * cos(c2) + l3 * sin(c1) * cos(c2 + c3);
        z = -l2 * cos(c1) * cos(c2) - l3 * cos(c1) * cos(c2 + c3);
        tp = 0;

        t.start();
    }

    double d = sqrt((changePos[0] * changePos[0]) + (changePos[1] * changePos[1]) + (changePos[2] * changePos[2]));
    double T1 = d / (v * 0.1);
    tp += (t.elapsed() / 1000.0) * 1 / T1;
    if (tp < 1)
    {
        double px = 0, py = 0, pz = 0;
        if (leg == 0 || leg == 1)
            changePos[0] = -changePos[0];

        if (leg == 1 || leg == 3)
            changePos[1] = -changePos[1];

        if (leg == 0 || leg == 3)
            px = x + changePos[0] * tp;
        else
            px = -x + changePos[0] * tp;

        py = y + changePos[1] * tp;
        pz = z + changePos[2] * tp;
        c1 = atan(py / pz);
        double f = acos((l2 * l2 + px * px + (py - l1 * sin(c1)) * (py - l1 * sin(c1)) +
                         (pz + l1 * cos(c1)) * (pz + l1 * cos(c1)) - l3 * l3) / (2 * l2 * sqrt(px * px +
                                                                                               (py - l1 * sin(c1)) *
                                                                                               (py - l1 * sin(c1)) +
                                                                                               (pz + l1 * cos(c1)) *
                                                                                               (pz + l1 * cos(c1)))));
        c2 = atan(px / (sqrt(py * py + pz * pz)) - l1) - f;
        c3 = (l2 + l3) / l3 * f;

        double cc1 = 0, cc2 = 0, cc3 = 0;

        if (leg == 0 || leg == 3) {
            cc1 = c1 * PI + globalData->refValue[leg];
            cc2 = c2 * PI + globalData->refValue[leg + 1];
            cc3 = c3 * PI + globalData->refValue[leg + 2];
        } else {
            cc1 = -c1 * PI + globalData->refValue[leg];
            cc2 = -c2 * PI + globalData->refValue[leg + 1];
            cc3 = -c3 * PI + globalData->refValue[leg + 2];
        }
        //isnanf代表检查数据是否为NAN

#if DEBUG_SIM
        if (!isnanf(static_cast<long double>(cc1)))
            global->currentCanAnalyticalData[leg].position = cc1;
        if (!isnanf(static_cast<long double>(cc2)))
            global->currentCanAnalyticalData[leg + 1].position = cc2;
        if (!isnanf(static_cast<long double>(cc3)))
            global->currentCanAnalyticalData[leg + 2].position = cc3;

        qDebug() << global->currentCanAnalyticalData[leg].position << global->currentCanAnalyticalData[leg + 1].position
                 << global->currentCanAnalyticalData[leg + 2].position;
#else
        if (!isnanf(static_cast<long double>(cc1)))
            currentPos[leg] = cc1;
        if (!isnanf(static_cast<long double>(cc2)))
            currentPos[leg + 1] = cc2;
        if (!isnanf(static_cast<long double>(cc3)))
            currentPos[leg + 2] = cc3;

        qDebug() << currentPos[leg] << currentPos[leg + 1]
                 << currentPos[leg + 2];

        //逆运算完成后，将计算出的目标角度输出给关节，启动四足运动
        Package::packOperateMulti(global->sendId, currentPos, NODE_NUM, PROTOCOL_TYPE_POS);
#endif
    }
    else
    {
        if (mode)
            flag = 0;
    }
}

/**
 * @brief 四足身体移动设置
 *
 * @param changePos 身体的位置 c a d b
 * @param v 运动速度
 * @return void
 */
void AutoControl::moveBodySet(double changePos[], double v, int mode)
{
    this->isSetting = 1; //正在修改设置时不启动运行
    for (int i = 0; i < 3; i++)
    {
        this->changePos[i] = changePos[i];
    }
    this->v = v;
    this->mode = mode;
    this->useBody = 1;
    this->isSetting = 0; //修改完成
}

/**
 * @brief 移动机器人本体
 * 
 */
void AutoControl::moveBody()
{
    static double ca1 = 0;
    static double ca2 = 0;
    static double ca3 = 0;
    static double cb1 = 0;
    static double cb2 = 0;
    static double cb3 = 0;
    static double cc1 = 0;
    static double cc2 = 0;
    static double cc3 = 0;
    static double cd1 = 0;
    static double cd2 = 0;
    static double cd3 = 0;

    static double xa = 0;
    static double ya = 0;
    static double za = 0;
    static double xb = 0;
    static double yb = 0;
    static double zb = 0;
    static double xc = 0;
    static double yc = 0;
    static double zc = 0;
    static double xd = 0;
    static double yd = 0;
    static double zd = 0;
    static double tp = 0;

    double f = 0;

    static QElapsedTimer t;
    static double currentPos[NODE_NUM];

    if (flag == 0)
    {
        if (NODE_NUM != 12) //目前只对12个关节做处理
            return;
        flag = 1;
        ca1 = (globalData->currentCanAnalyticalData[3].position - globalData->refValue[3]) / PI;
        ca2 = (globalData->currentCanAnalyticalData[4].position - globalData->refValue[4]) / PI;
        ca3 = (globalData->currentCanAnalyticalData[5].position - globalData->refValue[5]) / PI;
        cb1 = (globalData->currentCanAnalyticalData[9].position - globalData->refValue[9]) / PI;
        cb2 = (globalData->currentCanAnalyticalData[10].position - globalData->refValue[10]) / PI;
        cb3 = (globalData->currentCanAnalyticalData[11].position - globalData->refValue[11]) / PI;
        cc1 = (globalData->currentCanAnalyticalData[0].position - globalData->refValue[0]) / PI;
        cc2 = (globalData->currentCanAnalyticalData[1].position - globalData->refValue[1]) / PI;
        cc3 = (globalData->currentCanAnalyticalData[2].position - globalData->refValue[2]) / PI;
        cd1 = (globalData->currentCanAnalyticalData[6].position - globalData->refValue[6]) / PI;
        cd2 = (globalData->currentCanAnalyticalData[7].position - globalData->refValue[7]) / PI;
        cd3 = (globalData->currentCanAnalyticalData[8].position - globalData->refValue[8]) / PI;

        for (int i = 0; i < NODE_NUM; i++)
            currentPos[i] = globalData->currentCanAnalyticalData[i].position;

        xa = l2 * sin(ca2) + l3 * sin(ca2 + ca3);
        ya = l2 * sin(ca1) * cos(ca2) + l3 * sin(ca1) * cos(ca2 + ca3);
        za = -l2 * cos(ca1) * cos(ca2) - l3 * cos(ca1) * cos(ca2 + ca3);
        xb = l2 * sin(cb2) + l3 * sin(cb2 + cb3);
        yb = l2 * sin(cb1) * cos(cb2) + l3 * sin(cb1) * cos(cb2 + cb3);
        zb = -l2 * cos(cb1) * cos(cb2) - l3 * cos(cb1) * cos(cb2 + cb3);
        xc = l2 * sin(cc2) + l3 * sin(cc2 + cc3);
        yc = l2 * sin(cc1) * cos(cc2) + l3 * sin(cc1) * cos(cc2 + cc3);
        zc = -l2 * cos(cc1) * cos(cc2) - l3 * cos(cc1) * cos(cc2 + cc3);
        xd = l2 * sin(cd2) + l3 * sin(cd2 + cd3);
        yd = l2 * sin(cd1) * cos(cd2) + l3 * sin(cd1) * cos(cd2 + cd3);
        zd = -l2 * cos(cd1) * cos(cd2) - l3 * cos(cd1) * cos(cd2 + cd3);
        tp = 0;

        t.start();
    }

    double d = sqrt((changePos[0] * changePos[0]) + (changePos[1] * changePos[1]) + (changePos[2] * changePos[2]));
    double T1 = d / (v * 0.1);
    tp += (t.elapsed() / 1000.0) * 1 / T1;
    if (tp < 1)
    {
        double pxa = -xa + changePos[0] * tp;
        double pya = ya + changePos[1] * tp;
        double pza = za + changePos[2] * tp;
        ca1 = atan(pya / pza);
        f = acos((l2 * l2 + pxa * pxa + (pya - l1 * sin(ca1)) * (pya - l1 * sin(ca1)) + (pza + l1 * cos(ca1)) * (pza + l1 * cos(ca1)) - l3 * l3) / (2 * l2 * sqrt(pxa * pxa + (pya - l1 * sin(ca1)) * (pya - l1 * sin(ca1)) + (pza + l1 * cos(ca1)) * (pza + l1 * cos(ca1)))));
        ca2 = atan(pxa / (sqrt(pya * pya + pza * pza)) - l1) - f;
        ca3 = (l2 + l3) / l3 * f;

        double pxb = xb - changePos[0] * tp;
        double pyb = yb + changePos[1] * tp;
        double pzb = zb + changePos[2] * tp;
        cb1 = atan(pyb / pzb);
        f = acos((l2 * l2 + pxb * pxb + (pyb - l1 * sin(cb1)) * (pyb - l1 * sin(cb1)) + (pzb + l1 * cos(cb1)) * (pzb + l1 * cos(cb1)) - l3 * l3) / (2 * l2 * sqrt(pxb * pxb + (pyb - l1 * sin(cb1)) * (pyb - l1 * sin(cb1)) + (pzb + l1 * cos(cb1)) * (pzb + l1 * cos(cb1)))));
        cb2 = atan(pxb / (sqrt(pyb * pyb + pzb * pzb)) - l1) - f;
        cb3 = (l2 + l3) / l3 * f;

        double pxc = xc + changePos[0] * tp;
        double pyc = yc - changePos[1] * tp;
        double pzc = zc + changePos[2] * tp;
        cc1 = atan(pyc / pzc);
        f = acos((l2 * l2 + pxc * pxc + (pyc - l1 * sin(cc1)) * (pyc - l1 * sin(cc1)) + (pzc + l1 * cos(cc1)) * (pzc + l1 * cos(cc1)) - l3 * l3) / (2 * l2 * sqrt(pxc * pxc + (pyc - l1 * sin(cc1)) * (pyc - l1 * sin(cc1)) + (pzc + l1 * cos(cc1)) * (pzc + l1 * cos(cc1)))));
        cc2 = atan(pxc / (sqrt(pyc * pyc + pzc * pzc)) - l1) - f;
        cc3 = (l2 + l3) / l3 * f;

        double pxd = -xd - changePos[0] * tp;
        double pyd = yd - changePos[1] * tp;
        double pzd = zd + changePos[2] * tp;
        cd1 = atan(pyd / pzd);
        f = acos((l2 * l2 + pxd * pxd + (pyd - l1 * sin(cd1)) * (pyd - l1 * sin(cd1)) +
                  (pzd + l1 * cos(cd1)) * (pzd + l1 * cos(cd1)) - l3 * l3) /
                 (2 * l2 * sqrt(pxd * pxd + (pyd - l1 * sin(cd1)) * (pyd - l1 * sin(cd1)) +
                                (pzd + l1 * cos(cd1)) * (pzd + l1 * cos(cd1)))));
        cd2 = atan(pxd / (sqrt(pyd * pyd + pzd * pzd)) - l1) - f;
        cd3 = (l2 + l3) / l3 * f;

        double cca1 = -ca1 * PI + globalData->refValue[3];
        double cca2 = -ca2 * PI + globalData->refValue[4];
        double cca3 = -ca3 * PI + globalData->refValue[5];

        double ccb1 = cb1 * PI + globalData->refValue[9];
        double ccb2 = cb2 * PI + globalData->refValue[10];
        double ccb3 = cb3 * PI + globalData->refValue[11];

        double ccc1 = cc1 * PI + globalData->refValue[0];
        double ccc2 = cc2 * PI + globalData->refValue[1];
        double ccc3 = cc3 * PI + globalData->refValue[2];

        double ccd1 = -cd1 * PI + globalData->refValue[6];
        double ccd2 = -cd2 * PI + globalData->refValue[7];
        double ccd3 = -cd3 * PI + globalData->refValue[8];

        //isnanf代表检查数据是否为NAN
#if DEBUG_SIM
        if (!isnanf(static_cast<long double>(cca1)))
            global->currentCanAnalyticalData[3].position = cca1;
        if (!isnanf(static_cast<long double>(cca2)))
            global->currentCanAnalyticalData[4].position = cca2;
        if (!isnanf(static_cast<long double>(cca3)))
            global->currentCanAnalyticalData[5].position = cca3;

        if (!isnanf(static_cast<long double>(ccb1)))
            global->currentCanAnalyticalData[9].position = ccb1;
        if (!isnanf(static_cast<long double>(ccb2)))
            global->currentCanAnalyticalData[10].position = ccb2;
        if (!isnanf(static_cast<long double>(ccb3)))
            global->currentCanAnalyticalData[11].position = ccb3;

        if (!isnanf(static_cast<long double>(ccc1)))
            global->currentCanAnalyticalData[0].position = ccc1;
        if (!isnanf(static_cast<long double>(ccc2)))
            global->currentCanAnalyticalData[1].position = ccc2;
        if (!isnanf(static_cast<long double>(ccc3)))
            global->currentCanAnalyticalData[2].position = ccc3;

        if (!isnanf(static_cast<long double>(ccd1)))
            global->currentCanAnalyticalData[6].position = ccd1;
        if (!isnanf(static_cast<long double>(ccd2)))
            global->currentCanAnalyticalData[7].position = ccd2;
        if (!isnanf(static_cast<long double>(ccd3)))
            global->currentCanAnalyticalData[8].position = ccd3;
#else
        if (!isnanf(static_cast<long double>(cca1)))
            currentPos[3] = cca1;
        if (!isnanf(static_cast<long double>(cca2)))
            currentPos[4] = cca2;
        if (!isnanf(static_cast<long double>(cca3)))
            currentPos[5] = cca3;

        if (!isnanf(static_cast<long double>(ccb1)))
            currentPos[9] = ccb1;
        if (!isnanf(static_cast<long double>(ccb2)))
            currentPos[10] = ccb2;
        if (!isnanf(static_cast<long double>(ccb3)))
            currentPos[11] = ccb3;

        if (!isnanf(static_cast<long double>(ccc1)))
            currentPos[0] = ccc1;
        if (!isnanf(static_cast<long double>(ccc2)))
            currentPos[1] = ccc2;
        if (!isnanf(static_cast<long double>(ccc3)))
            currentPos[2] = ccc3;

        if (!isnanf(static_cast<long double>(ccd1)))
            currentPos[6] = ccd1;
        if (!isnanf(static_cast<long double>(ccd2)))
            currentPos[7] = ccd2;
        if (!isnanf(static_cast<long double>(ccd3)))
            currentPos[8] = ccd3;

        qDebug() << currentPos[0] << currentPos[1] << currentPos[2] << currentPos[3] << currentPos[4]
                 << currentPos[5] << currentPos[6] << currentPos[7] << currentPos[8] << currentPos[9]
                 << currentPos[10] << currentPos[11];

        //逆运算完成后，将计算出的目标角度输出给关节，启动四足运动
        Package::packOperateMulti(global->sendId, currentPos, NODE_NUM, PROTOCOL_TYPE_POS);
#endif
    }
}

/**
 * @brief 自动运行函数，用来自动进行路径规划，从导航、相机等外设获取控制信号，
 * 来计算控制数据，最后输出给四足运行，无需人工进行干预
 * TODO: 待完善
 * 建议数据获取：global->navigateData/global->CanAnalysis
 */
void AutoControl::autoRunFunc()
{
    
}
