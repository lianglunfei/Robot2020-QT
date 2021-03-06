/*
 * @Author: xingzhang.Wu 
 * @Date: 2019-10-21 16:07:52 
 * @Last Modified by: xingzhang.Wu
 * @Last Modified time: 2019-11-11 21:43:51
 */
#include "remotecontrol.h"
#include "debug.h"

RemoteControl::RemoteControl(QDialog *parent) : QDialog(parent)
{
    ac = new AutoControl();
}

RemoteControl::~RemoteControl()
{
    delete qmlControl1;
    delete qmlControl2;
    delete ac;
}

/**
 * @brief 显示遥控器界面
 * 
 */
void RemoteControl::show()
{
    engine.load(QUrl(QStringLiteral("qrc:/qml/main.qml")));
    QObject *item = engine.rootObjects().first(); //component->create();
    QObject *qmlLeftObject = item->findChild<QObject *>("qmlLeftJoySignal");
    QObject *qmlRightObject = item->findChild<QObject *>("qmlRightJoySignal");
    QObject *qmlGamepadObject = item->findChild<QObject *>("qmlGamepadSignal");

    QObject::connect(qmlLeftObject, SIGNAL(qmlLeftJoySignal(double)), this, SLOT(leftJoySlot(double)));
    QObject::connect(qmlRightObject, SIGNAL(qmlRightJoySignal(double)), this, SLOT(rightJoySlot(double)));
    QObject::connect(qmlGamepadObject, SIGNAL(qmlStartSignal(void)), this, SLOT(qmlStartSlot(void)));
    QObject::connect(qmlGamepadObject, SIGNAL(qmlL1Signal(void)), this, SLOT(qmlL1Slot(void)));
    QObject::connect(qmlGamepadObject, SIGNAL(qmlASignal(bool)), this, SLOT(qmlASlot(bool)));
    QObject::connect(qmlGamepadObject, SIGNAL(qmlBSignal(bool)), this, SLOT(qmlBSlot(bool)));

    qmlControl2 = item->findChild<QObject *>("qmlcontrol2");
    qmlControl1 = item->findChild<QObject *>("qmlcontrol1");
}

/**
 * @brief 左边摇杆事件
 * 
 * @param speed 
 */
void RemoteControl::leftJoySlot(const double &speed)
{
    //测试案例
    // double changePos[3] = {0.0, 0.0, -0.05};
    // ac->moveLegSet(0, changePos, 1, 1);
    // ac->start();
    // if (speed < 1000)
    //     ac->stop();
}

/**
 * @brief 右边摇杆事件
 * 
 * @param angle 
 */
void RemoteControl::rightJoySlot(const double &angle)
{
    //测试案例
    // double changePos[3] = {0.0, 0.0, 0.05};
    // ac->moveLegSet(0, changePos, 1, 1);
    // ac->start();
    // if (angle < 1000)
    //     ac->stop();
}

/**
 * @brief 按键Start事件函数
 * 
 */
void RemoteControl::qmlStartSlot()
{
    //测试案例
    ac->autoRunSet();
}

/**
 * @brief L1事件函数
 * 
 */
void RemoteControl::qmlL1Slot()
{
}

/**
 * @brief 按键A事件函数
 * 
 * @param press 
 */
void RemoteControl::qmlASlot(bool press)
{
    //测试案例
    // double changePos[3] = {0.0, 0.0, -0.05};
    // ac->moveLegSet(0, changePos, 1, 0);
    // ac->start();
    double changePos[3] = {0.0, -0.025, 0.0};
    ac->moveBodySet(changePos, 1, 0);
    ac->start();
}

/**
 * @brief 按键B事件函数
 * 
 * @param press 
 */
void RemoteControl::qmlBSlot(bool press)
{
    //测试案例
    // double changePos[3] = {0.0, 0.0, 0.05};
    // ac->moveLegSet(0, changePos, 1, 0);
    // ac->start();
    double changePos[3] = {0.0, 0.025, 0.0};
    ac->moveBodySet(changePos, 1, 0);
    ac->start();
}
