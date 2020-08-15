/*
 * @Author: lunfei.liang
 * @Date: 2020-07-24 10:09:55
 * @Last Modified by:   lunfei.liang
 * @Last Modified time:
 */
#include "reconfigcontrol.h"
#include "ui_reconfigcontrol.h"

#include <QKeyEvent>

//#include "mycustomslider.h"
#include "drivers.h"
#include "qdebug.h"
#include "package.h"
#include "datatransmission.h"
#include "debug.h"
#include "doubleslider.h"
#include "protocol.h"

#include <QDoubleSpinBox>
#include <QPushButton>
#include <QCheckBox>
#include <QMessageBox>

#include <QGuiApplication>

#include <QtWidgets/QApplication>
#include <QtWidgets/QWidget>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QCommandLinkButton>
#include <QtGui/QScreen>

#define Platform_SpeedMode  5
#define Platform_STOP  6
#define Platform_Forward 1
#define Platform_Backward 2
#define Reconfig_Open 3
#define Reconfig_Close 4
#define Reconfig_Stop  7
#define Platform_RePositionMode 8


unsigned char FlatformSpeedMode[8]      = {0x08,0x11,0x2A,0x00,0x00,0x00,0x00,0x00};
unsigned char FlatformRePositionMode[8] = {0x08,0x11,0x2A,0x00,0x00,0x01,0x00,0x00};
unsigned char ForwardCode[8]            = {0x08,0x11,0x90,0x00,0x88,0x13,0x00,0x00};
unsigned char BackwardCode[8]           = {0x08,0x11,0x90,0x00,0x78,0xEC,0xFF,0xFF};
unsigned char FlatformStopCode[8]       = {0x08,0x11,0x90,0x00,0x00,0x00,0x00,0x00};

unsigned char OpenCode[8]       = {0x9A,0x00,0x00,0xC8,0x00,0x00,0x00,0x01};
unsigned char CloseCode[8]      = {0x9A,0xFF,0x00,0xC8,0x00,0x00,0x00,0x01};
unsigned char ReStopCode[8]     = {0x9A,0x00,0x00,0x00,0x00,0x00,0x00,0x01};

/**
 * @brief Construct a new Reconfig Control:: Reconfig Control object
 *
 */
ReconfigControl::ReconfigControl(QWidget *parent) : QDialog(parent),ui(new Ui::ReconfigControl)
//ReconfigControl::ReconfigControl() : ui(new Ui::ReconfigControl)
{
    ui->setupUi(this);
    this->setWindowTitle(tr("Reconfig Control"));

   initObject();
    if (initUiObject())
    {
        initConnection();
        controlWidgetInit();
    }


}

/**
 * @brief 初始化界面信息，字符串代表界面控件的object名称
 *
 */
void ReconfigControl::initObject()
{
    setSpeedSlider(QList<QString>({"PlatformSpeedHorizontalSlider","ReconfigSpeedHorizontalSlider"}));
    setPositionSlider(QList<QString>({"PlatformAbsPosHorizontalSlider","ReconfigAbsPosHorizontalSlider"}));
    setSpeedSpinBox(QList<QString>({"PlatformSpeedDoubleSpinBox","ReconfigSpeedDoubleSpinBox"}));
    setPositionSpinBox(QList<QString>({"PlatformAbsPosDoubleSpinBox","ReconfigAbsPosDoubleSpinBox"}));
    setPositionSetButton(QList<QString>({"PlatformAbsPosSetButton","ReconfigAbsPosSetButton"}));
    setSpeedSetButton(QList<QString>({"PlatformSpeedSetButton","ReconfigSpeedSetButton"}));
    setForwardReversalPushButton(QList<QString>({"PlatformPosFWPushButton", "PlatformPosRPushButton",
                                                "ReconfigPosFWPushButton", "ReconfigPosRPushButton"}));
    setRelativeSpinBox(QList<QString>({"PlatfromRPosDoubleSpinBox","ReconfigRPositionDoubleSpinBox"}));

}



/**
*@projectName   RobotControlSystem
*@brief         判断对象数组是否有赋值，如果没有则返回false
*@parameter
*@author        Lunfei.Liang
*@date          20200728
**/
bool ReconfigControl::initUiObject()
{
    if (SpeedSlider.length() == 0 || PositionSlider.length() == 0 || SpeedSpinBox.length() == 0 ||
            PositionSpinBox.length() == 0 || PositionSetButton.length() == 0 || SpeedSetButton.length() == 0 ||
            ForwardReversalPushButton.length() == 0 || RelativeSpinBox.length() == 0 )
        return false;
    return true;
}


/**
*@projectName   RobotControlSystem
*@brief         信号与槽的初始化
*@parameter
*@author        Lunfei.Liang
*@date          20200728
**/
void ReconfigControl::initConnection()
{
    for (int i = 0; i < SpeedSpinBox.length(); i++)
    {
        connect(findChild<QDoubleSpinBox *>(SpeedSpinBox[i]), SIGNAL(valueChanged(double)),
                findChild<DoubleSlider *>(SpeedSlider[i]), SLOT(doubleSetValue(double)));
        connect(findChild<QDoubleSpinBox *>(SpeedSpinBox[i]), SIGNAL(editingFinished()),
                this, SLOT(speedValueChanged()));
    }
    for (int i = 0; i < PositionSpinBox.length(); i++)
    {
        connect(findChild<QDoubleSpinBox *>(PositionSpinBox[i]), SIGNAL(valueChanged(double)),
                findChild<DoubleSlider *>(PositionSlider[i]), SLOT(doubleSetValue(double)));
        connect(findChild<QDoubleSpinBox *>(PositionSpinBox[i]), SIGNAL(editingFinished()),
                this, SLOT(posValueChanged()));
    }
    for (int i = 0; i < SpeedSlider.length(); i++)
    {
        connect(findChild<DoubleSlider *>(SpeedSlider[i]), SIGNAL(doubleValueChanged(double)),
                findChild<QDoubleSpinBox *>(SpeedSpinBox[i]), SLOT(setValue(double)));
        connect(findChild<DoubleSlider *>(SpeedSlider[i]), SIGNAL(sliderReleased()),
                this, SLOT(speedValueChanged()));
    }
    for (int i = 0; i < PositionSlider.length(); i++)
    {
        connect(findChild<DoubleSlider *>(PositionSlider[i]), SIGNAL(doubleValueChanged(double)),
                findChild<QDoubleSpinBox *>(PositionSpinBox[i]), SLOT(setValue(double)));
        connect(findChild<DoubleSlider *>(PositionSlider[i]), SIGNAL(sliderReleased()),
                this, SLOT(posValueChanged()));
    }
    for (int i = 0; i < SpeedSetButton.length(); i++)
    {
        connect(findChild<QPushButton *>(SpeedSetButton[i]), SIGNAL(pressed()),
                this, SLOT(setSpeedButtonClicked()));
        connect(findChild<QPushButton *>(SpeedSetButton[i]), SIGNAL(released()),
                this, SLOT(setSpeedButtonReleased()));
    }
    for (int i = 0; i < PositionSetButton.length(); i++)
    {
        connect(findChild<QPushButton *>(PositionSetButton[i]), SIGNAL(pressed()),
                this, SLOT(setPosButtonClicked()));
    }

    for (int i = 0; i < ForwardReversalPushButton.length(); i++)
    {
        connect(findChild<QPushButton *>(ForwardReversalPushButton[i]), SIGNAL(pressed()),
                this, SLOT(ForwardReversalPushbtnClicked()));
    }


}

/**
*@projectName   RobotControlSystem
*@brief         界面滑条、方框设定最大最小值等
*@parameter
*@author        Lunfei.Liang
*@date          20200728
**/
void ReconfigControl::controlWidgetInit()
{

    for (int i = 0; i < SpeedSlider.length(); i++)
    {
        findChild<DoubleSlider *>(SpeedSlider[i])->setRange(RE_SPEED_MIN, RE_SPEED_MAX);
    }
    for (int i = 0; i < PositionSlider.length(); i++)
    {
        findChild<DoubleSlider *>(PositionSlider[i])->setRange(RE_POSITION_MIN, RE_POSITION_MAX);
    }
    for (int i = 0; i < SpeedSpinBox.length(); i++)
    {
        findChild<QDoubleSpinBox *>(SpeedSpinBox[i])->setRange(RE_SPEED_MIN, RE_SPEED_MAX);
    }
    for (int i = 0; i < PositionSpinBox.length(); i++)
    {
        findChild<QDoubleSpinBox *>(PositionSpinBox[i])->setRange(RE_POSITION_MIN, RE_POSITION_MAX);
    }
}




/**
*@projectName   RobotControlSystem
*@brief         读取输入位置和速度值函数
*@parameter
*@author        Lunfei.Liang
*@date          20200728
**/


void ReconfigControl::speedValueChanged()
{
    auto *Spin = qobject_cast<QDoubleSpinBox *>(this->sender());
    auto *Slider = qobject_cast<DoubleSlider *>(this->sender());
    if (Spin == findChild<QDoubleSpinBox *>(SpeedSpinBox[0]) || Slider == findChild<DoubleSlider *>(SpeedSlider[0]))
    {
        ReadPlatformSpeedData = findChild<QDoubleSpinBox *>(SpeedSpinBox[0])->text().toDouble();
    }
    else if (Spin == findChild<QDoubleSpinBox *>(SpeedSpinBox[1]) || Slider == findChild<DoubleSlider *>(SpeedSlider[1]))
    {
        ReadReconfigSpeedData = findChild<QDoubleSpinBox *>(SpeedSpinBox[1])->text().toDouble();
    }



}

void ReconfigControl::posValueChanged()
{
    auto *Spin = qobject_cast<QDoubleSpinBox *>(this->sender());
    auto *Slider = qobject_cast<DoubleSlider *>(this->sender());

    if (Spin == findChild<QDoubleSpinBox *>(PositionSpinBox[0]) || Slider == findChild<DoubleSlider *>(PositionSlider[0]))
    {
         ReadPlatformPositionData = findChild<QDoubleSpinBox *>(PositionSpinBox[0])->text().toDouble();
     }
    else if (Spin == findChild<QDoubleSpinBox *>(PositionSpinBox[1]) || Slider == findChild<DoubleSlider *>(PositionSlider[1]))
    {
         ReadReconfigPositionData = findChild<QDoubleSpinBox *>(PositionSpinBox[1])->text().toDouble();
     }

}

/**
*@projectName   RobotControlSystem
*@brief         设置按钮处理函数
*@parameter
*@author        Lunfei.Liang
*@date          20200728
**/
void ReconfigControl::setPosButtonClicked()
{
    auto *btn = qobject_cast<QPushButton *>(this->sender());
    if (btn == findChild<QPushButton *>(PositionSetButton[0]))
    {
        PlatformPositionControl(ReadPlatformPositionData);
     }
    else if (btn == findChild<QPushButton *>(PositionSetButton[1]))
    {
        ReconfigPositionControl(ReadReconfigPositionData);
     }
}

void ReconfigControl::setSpeedButtonClicked()
{
    auto *btn = qobject_cast<QPushButton *>(this->sender());
    if (btn == findChild<QPushButton *>(SpeedSetButton[0]))
    {
        PlatformSpeedControl(ReadPlatformSpeedData);
    }
    else if (btn == findChild<QPushButton *>(SpeedSetButton[1]))
    {
        ReconfigSpeedControl(ReadReconfigSpeedData);
    }
}


void ReconfigControl::setSpeedButtonReleased()
{
    auto *btn = qobject_cast<QPushButton *>(this->sender());
    if (btn == findChild<QPushButton *>(SpeedSetButton[0]))
    {
        PlatformSpeedControl(0);
    }
    else if (btn == findChild<QPushButton *>(SpeedSetButton[1]))
    {
        ReconfigAct(Reconfig_Stop);
    }
}



void ReconfigControl::ForwardReversalPushbtnClicked()
{

}



/**
 * @brief 速度模式
 *
 */
void ReconfigControl::on_PlatformReModepushButton_clicked()
{

    PlatformAct(Platform_SpeedMode);
}


/**
 * @brief 位置模式
 *
 */
void ReconfigControl::on_PlatformPosModepushButton_clicked()
{

    PlatformAct(Platform_RePositionMode);
}


/**
 * @brief 平台前进
 *
 */
void ReconfigControl::on_PlatformFWpushButton_pressed()
{
    PlatformAct(Platform_Forward);
}

void ReconfigControl::on_PlatformFWpushButton_released()
{
    PlatformAct(Platform_STOP);
}


/**
 * @brief 平台后退
 *
 */
void ReconfigControl::on_PlatformBWpushButton_pressed()
{
    PlatformAct(Platform_Backward);
}

void ReconfigControl::on_PlatformBWpushButton_released()
{
    PlatformAct(Platform_STOP);
}

/**
 * @brief 臂杆分离
 *
 */
void ReconfigControl::on_ReOpenpushButton_pressed()
{
    ReconfigAct(Reconfig_Open);
}

void ReconfigControl::on_ReOpenpushButton_released()
{
    ReconfigAct(Reconfig_Stop);
}


/**
 * @brief 臂杆重构
 *
 */
void ReconfigControl::on_ReClosepushButton_pressed()
{
    ReconfigAct(Reconfig_Close);
}

void ReconfigControl::on_ReClosepushButton_released()
{
    ReconfigAct(Reconfig_Stop);
}

/**
 * @brief 可重构平台控制 1前进  2后退
 *
 */
void ReconfigControl::PlatformAct(int action)
{
    unsigned int currentId = 17;
    int ret;
    switch (action)
    {
        case Platform_SpeedMode://平台速度模式
            ret = DataTransmission::CANTransmit(globalData->connectType, FlatformSpeedMode, currentId);
            break;
        case Platform_RePositionMode://平台位置模式
            ret = DataTransmission::CANTransmit(globalData->connectType, FlatformRePositionMode, currentId);
        break;
        case Platform_Forward://平台前进
            ret = DataTransmission::CANTransmit(globalData->connectType, ForwardCode, currentId);
            break;
        case Platform_Backward://平台后退
            ret = DataTransmission::CANTransmit(globalData->connectType, BackwardCode, currentId);
            break;
        case Platform_STOP://平台电机停止运动
            ret = DataTransmission::CANTransmit(globalData->connectType, FlatformStopCode, currentId);
            break;
    }

    if (ret == -1)
    {
        qDebug() << "failed- device not open"; //=-1表示USB-CAN设备不存在或USB掉线
        return;
    }
    else if (ret == 0)
    {
        qDebug() << "send error";
        return;
    }
    else
    {
        qDebug() << "send successful";
    }
}




/**
 * @brief 重构控制 3分离 4重构
 *
 */
void ReconfigControl::ReconfigAct(int action)
{
    unsigned int currentId = 18;
    int ret;
    switch (action)
    {
        case Reconfig_Open://臂杆分离
            ret = DataTransmission::CANTransmit(globalData->connectType, OpenCode, currentId);
            break;
        case Reconfig_Close://臂杆重构
            ret = DataTransmission::CANTransmit(globalData->connectType, CloseCode, currentId);
            break;
        case Reconfig_Stop://臂杆电机停止运动
            ret = DataTransmission::CANTransmit(globalData->connectType, ReStopCode, currentId);
            break;
    }

    if (ret == -1)
    {
        qDebug() << "failed- device not open"; //=-1表示USB-CAN设备不存在或USB掉线
        return;
    }
    else if (ret == 0)
    {
        qDebug() << "send error";
        return;
    }
    else
    {
        qDebug() << "send successful";
    }
}

/**
 * @brief 可重构平台速度控制
 *
 */

void ReconfigControl::PlatformSpeedControl(double SpeedData)
{
    unsigned int currentId = 17;
    int ret;
    unsigned char packData[8] = {0};
    Protocol::packRePlatformSpeed(packData,currentId,SpeedData);
    ret = DataTransmission::CANTransmit(globalData->connectType, packData, currentId);


    if (ret == -1)
    {
        qDebug() << "failed- device not open"; //=-1表示USB-CAN设备不存在或USB掉线
        return;
    }
    else if (ret == 0)
    {
        qDebug() << "send error";
        return;
    }
    else
    {
        qDebug() << "send successful";
    }
}

/**
 * @brief 可重构平台位置控制
 *
 */

void ReconfigControl::PlatformPositionControl(double RePositionData)
{
    unsigned int currentId = 17;
    int ret;
    unsigned char packData[8] = {0};
    Protocol::packRePlatformPos(packData,currentId,RePositionData);
    ret = DataTransmission::CANTransmit(globalData->connectType, packData, currentId);


    if (ret == -1)
    {
        qDebug() << "failed- device not open"; //=-1表示USB-CAN设备不存在或USB掉线
        return;
    }
    else if (ret == 0)
    {
        qDebug() << "send error";
        return;
    }
    else
    {
        qDebug() << "send successful";
    }
}




/**
 * @brief 可重构臂杆速度控制
 *
 */

void ReconfigControl::ReconfigSpeedControl(double SpeedData)
{
    unsigned int currentId = 18;
    int ret;
    unsigned char packData[8] = {0};
    Protocol::packReconfigSpeed(packData,SpeedData);
    ret = DataTransmission::CANTransmit(globalData->connectType, packData, currentId);


    if (ret == -1)
    {
        qDebug() << "failed- device not open"; //=-1表示USB-CAN设备不存在或USB掉线
        return;
    }
    else if (ret == 0)
    {
        qDebug() << "send error";
        return;
    }
    else
    {
        qDebug() << "send successful";
    }
}

/**
 * @brief 可重构臂杆位置控制
 *
 */
void ReconfigControl::ReconfigPositionControl(double AbsPositionData)
{
    unsigned int currentId = 18;
    int ret;
    unsigned char packData[8] = {0};
    Protocol::packReconfigPos(packData,AbsPositionData);
    ret = DataTransmission::CANTransmit(globalData->connectType, packData, currentId);


    if (ret == -1)
    {
        qDebug() << "failed- device not open"; //=-1表示USB-CAN设备不存在或USB掉线
        return;
    }
    else if (ret == 0)
    {
        qDebug() << "send error";
        return;
    }
    else
    {
        qDebug() << "send successful";
    }
}





ReconfigControl::~ReconfigControl()
{
    delete ui;
}

void ReconfigControl::setPositionSlider(const QList<QString> &l)
{
    PositionSlider = l;
}

void ReconfigControl::setPositionSpinBox(const QList<QString> &l)
{
    PositionSpinBox = l;
}

void ReconfigControl::setPositionSetButton(const QList<QString> &l)
{
    PositionSetButton = l;
}

void ReconfigControl::setSpeedSlider(const QList<QString> &l)
{
    SpeedSlider = l;
}

void ReconfigControl::setSpeedSpinBox(const QList<QString> &l)
{
    SpeedSpinBox = l;
}

void ReconfigControl::setSpeedSetButton(const QList<QString> &l)
{
    SpeedSetButton = l;
}

void ReconfigControl::setForwardReversalPushButton(const QList<QString> &l)
{
     ForwardReversalPushButton =l;
}
void ReconfigControl::setRelativeSpinBox(const QList<QString> &l)
{
      RelativeSpinBox = l;
}


