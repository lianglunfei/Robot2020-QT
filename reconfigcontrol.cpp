/*
 * @Author: lunfei.liang
 * @Date: 2020-07-24 10:09:55
 * @Last Modified by:   lunfei.liang
 * @Last Modified time:
 */
#include "reconfigcontrol.h"
#include "ui_reconfigcontrol.h"

#include <QKeyEvent>

#include "mycustomslider.h"
#include "drivers.h"
#include "qdebug.h"
#include "package.h"
#include "datatransmission.h"
#include "debug.h"

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


#define Platform_Forward 1
#define Platform_Backward 2
#define Reconfig_Open 3
#define Reconfig_Close 4



unsigned char ForwardCode[8]    = {0x8A,0x00,0x10,0x00,0x00,0x00,0x00,0x01};
unsigned char BackwardCode[8]   = {0x8A,0xFF,0x10,0x00,0x00,0x00,0x00,0x01};
unsigned char OpenCode[8]       = {0x8A,0x00,0x00,0x00,0x00,0x00,0x00,0x01};
unsigned char CloseCode[8]      = {0x8A,0x00,0x01,0x67,0x00,0x00,0x00,0x01};


/**
 * @brief Construct a new Reconfig Control:: Reconfig Control object
 *
 */
ReconfigControl::ReconfigControl() : ui(new Ui::ReconfigControl)
{
    ui->setupUi(this);
    this->setWindowTitle(tr("Reconfig Control"));

   initObject();
    if (initUiObject())
    {
        initConnection();
        controlWidgetInit();
    }
//    setCurrentNode(0);

}

/**
 * @brief 初始化界面信息，字符串代表界面控件的object名称
 *
 */
void ReconfigControl::initObject()
{
    setSpeedSlider(QList<QString>({"PlatformSpeedHorizontalSlider","ReconfigSpeedHorizontalSlider"}));
    setPositionSlider(QList<QString>({"PlatformPosHorizontalSlider","ReconfigPosHorizontalSlider"}));
    setSpeedSpinBox(QList<QString>({"PlatformSpeedDoubleSpinBox","ReconfigSpeedDoubleSpinBox"}));
    setPositionSpinBox(QList<QString>({"PlatformPosDoubleSpinBox","ReconfigPosDoubleSpinBox"}));
    setPositionSetButton(QList<QString>({"PlatformPosSetButton","ReconfigPosSetButton"}));
    setSpeedSetButton(QList<QString>({"PlatformSpeedSetButton","ReconfigSpeedSetButton"}));
    setForwardReversePushButton(QList<QString>({"PlatformPosFWPushButton", "PlatformPosRPushButton","ReconfigPosFWPushButton", "ReconfigPosRPushButton"}));
    setRelativeSpinBox(QList<QString>({"PlatfromRPosDoubleSpinBox","ReconfigRPosDoubleSpinBox"}));
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
    if (SpeedSlider.length() == 0 || PositionSlider.length() == 0 || SpeedSpinBox.length() == 0 || PositionSpinBox.length() == 0 || PositionSetButton.length() == 0 || SpeedSetButton.length() == 0 || ForwardReversePushButton.length() == 0 || RelativeSpinBox.length() == 0 )
        return false;
    return true;
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
        findChild<DoubleSlider *>(SpeedSlider[i])->setRange(SPEED_MIN, SPEED_MAX);
    }
    for (int i = 0; i < PositionSlider.length(); i++)
    {
        findChild<DoubleSlider *>(PositionSlider[i])->setRange(POSITION_MIN, POSITION_MAX);
    }
    for (int i = 0; i < SpeedSpinBox.length(); i++)
    {
        findChild<QDoubleSpinBox *>(SpeedSpinBox[i])->setRange(SPEED_MIN, SPEED_MAX);
    }
    for (int i = 0; i < PositionSpinBox.length(); i++)
    {
        findChild<QDoubleSpinBox *>(PositionSpinBox[i])->setRange(POSITION_MIN, POSITION_MAX);
    }
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
                findChild<DoubleSlider *>(SpeedSpinBox[i]), SLOT(doubleSetValue(double)));
        connect(findChild<QDoubleSpinBox *>(SpeedSpinBox[i]), SIGNAL(editingFinished()),
                this, SLOT(speedValueChanged()));
    }
    for (int i = 0; i < PositionSpinBox.length(); i++)
    {
        connect(findChild<QDoubleSpinBox *>(PositionSpinBox[i]), SIGNAL(valueChanged(double)),
                findChild<DoubleSlider *>(PositionSpinBox[i]), SLOT(doubleSetValue(double)));
        connect(findChild<QDoubleSpinBox *>(PositionSpinBox[i]), SIGNAL(editingFinished()),
                this, SLOT(posValueChanged()));
    }
    for (int i = 0; i < PositionSpinBox.length(); i++)
    {
        connect(findChild<DoubleSlider *>(PositionSpinBox[i]), SIGNAL(doubleValueChanged(double)),
                findChild<QDoubleSpinBox *>(PositionSpinBox[i]), SLOT(setValue(double)));
        connect(findChild<DoubleSlider *>(PositionSpinBox[i]), SIGNAL(sliderReleased()),
                this, SLOT(posValueChanged()));
    }
    for (int i = 0; i < SpeedSlider.length(); i++)
    {
        connect(findChild<DoubleSlider *>(SpeedSlider[i]), SIGNAL(doubleValueChanged(double)),
                findChild<QDoubleSpinBox *>(SpeedSlider[i]), SLOT(setValue(double)));
        connect(findChild<DoubleSlider *>(SpeedSlider[i]), SIGNAL(sliderReleased()),
                this, SLOT(speedValueChanged()));
    }
    for (int i = 0; i < PositionSetButton.length(); i++)
    {
        connect(findChild<QPushButton *>(PositionSetButton[i]), SIGNAL(pressed()),
                this, SLOT(setPosButtonClicked()));
    }
    for (int i = 0; i < SpeedSetButton.length(); i++)
    {
        connect(findChild<QPushButton *>(SpeedSetButton[i]), SIGNAL(pressed()),
                this, SLOT(setSpeedButtonClicked()));
        connect(findChild<QPushButton *>(SpeedSetButton[i]), SIGNAL(released()),
                this, SLOT(setSpeedButtonReleased()));
    }
    for (int i = 0; i < ForwardReversePushButton.length(); i++)
    {
        connect(findChild<QPushButton *>(ForwardReversePushButton[i]), SIGNAL(pressed()),
                this, SLOT(forwardReversalPushbtnClicked()));
    }

//    connect(worker, &ArmSyncWorker::recvData, this, &ReconfigControl::comDataRecv);

}

//void ReconfigControl::setCurrentNode(int nodeNum)
//{
//    currentNode = nodeNum > ARM_NODE_NUM - 1 ? ARM_NODE_NUM - 1 : nodeNum;
//}


//void ReconfigControl::posValueChanged()
//{
////    auto *Spin = qobject_cast<QDoubleSpinBox *>(this->sender());
////    auto *Slider = qobject_cast<MyCustomSlider *>(this->sender());

//    for (int i = 0; i < ARM_NODE_NUM; i++)
//    {
////        if ((Spin == findChild<QDoubleSpinBox *>(positionSpinBox[i]) || Slider == findChild<MyCustomSlider *>(positionSlider[i])))
////        {
//            int motorID = motorIDs[i]-1;
//            readyToSendCanData[motorID]=
//                findChild<QDoubleSpinBox *>(positionSpinBox[i])->value();
//            if(i==2)
//                armAngle[i] =  360.0-entity2model(readyToSendCanData[motorID],cal[i]);
//            else
//                armAngle[i] =  entity2model(readyToSendCanData[motorID],cal[i]);
//            updateModel(1);
////        }

//    }

//}

/**
*@projectName   RobotControlSystem
*@brief         读取输入位置和速度值函数
*@parameter
*@author        Lunfei.Liang
*@date          20200728
**/


void ReconfigControl::speedValueChanged()
{
    for (int i = 0; i < RE_NODE_NUM; i++)
    {
//       if ((Spin == findChild<QDoubleSpinBox *>(positionSpinBox) || Slider == findChild<MyCustomSlider *>(positionSlider)))
//       {
           int ReconfigID = ReconfigIDs[i]-1;
            ReadyToSendCanData[ReconfigID] = findChild<QDoubleSpinBox *>(SpeedSpinBox[i])->value();
//            if(i==2)
//                armAngle[i] =  360.0-entity2model(readyToSendCanData[motorID],cal[i]);
//            else
//                armAngle[i] =  entity2model(readyToSendCanData[motorID],cal[i]);
//            updateModel(1);
//       }

    }
}

void ReconfigControl::posValueChanged()
{
    for (int i = 0; i < RE_NODE_NUM; i++)
    {
//       if ((Spin == findChild<QDoubleSpinBox *>(positionSpinBox) || Slider == findChild<MyCustomSlider *>(positionSlider)))
//       {
           int ReconfigID = ReconfigIDs[i]-1;
            ReadyToSendCanData[ReconfigID] = findChild<QDoubleSpinBox *>(PositionSpinBox[i])->value();
//            if(i==2)
//                armAngle[i] =  360.0-entity2model(readyToSendCanData[motorID],cal[i]);
//            else
//                armAngle[i] =  entity2model(readyToSendCanData[motorID],cal[i]);
//            updateModel(1);
//       }

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
//    auto *btn = qobject_cast<QPushButton *>(this->sender());

    Package::packOperateMulti(globalData->sendId, ReadyToSendCanData, 2, PROTOCOL_TYPE_POS);
}

void ReconfigControl::setSpeedButtonClicked()
{
    Package::packOperateMulti(globalData->sendId, ReadyToSendCanData, 2, PROTOCOL_TYPE_SPD);
}


void ReconfigControl::setSpeedButtonReleased()
{

}



void ReconfigControl::forwardReversalPushbtnClicked()
{

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

void ReconfigControl::setForwardReversePushButton(const QList<QString> &l)
{
     ForwardReversePushButton =l;
}
void ReconfigControl::setRelativeSpinBox(const QList<QString> &l)
{
      RelativeSpinBox = l;
}




//void ReconfigControl::syncPosition(){
//    for (int i = 0; i < ARM_NODE_NUM; i++)
//    {
//        int motorID = motorIDs[i]-1;
//        double position = globalData->currentCanAnalyticalData[motorID].position;
//        findChild<QDoubleSpinBox *>(positionSpinBox[i])->setValue(position);
//        findChild<MyCustomSlider *>(positionSlider[i])->doubleSetValue(position);
//        if(i==2)
//            armAngle[i] =  360.0-entity2model(position,cal[i]);
//        else
//            armAngle[i] =  entity2model(position,cal[i]);


//    }
//}

//void ReconfigControl::startSync()
//{
//    QTimer::singleShot(0, taskTimer, SLOT(start()));
//    taskThread->start();
//}

//void ReconfigControl::stopSync()
//{
////    taskTimer->stop();
//    emit stopThread();

//}


/**
 * @brief 平台前进
 *
 */
void ReconfigControl::on_PlatformFWpushButton_clicked()
{
    PlatformAct(Platform_Forward);
}

/**
 * @brief 平台后退
 *
 */
void ReconfigControl::on_PlatformBWpushButton_clicked()
{
    PlatformAct(Platform_Backward);
}



//void ReconfigControl::on_clawOpenPushButton_released()
//{
//    clawAct(CLAW_STOP);
//}


/**
 * @brief 臂杆分离
 *
 */
void ReconfigControl::on_ReOpenpushButton_clicked()
{
    ReconfigAct(Reconfig_Open);
}


/**
 * @brief 臂杆重构
 *
 */
void ReconfigControl::on_ReClosepushButton_clicked()
{
    ReconfigAct(Reconfig_Close);
}


//void ReconfigControl::on_clawClosePushButton_released()
//{
//    clawAct(CLAW_STOP);
//}


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
        case Platform_Forward://平台前进
            ret = DataTransmission::CANTransmit(globalData->connectType, ForwardCode, currentId);
            break;
        case Platform_Backward://平台后退
            ret = DataTransmission::CANTransmit(globalData->connectType, BackwardCode, currentId);
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
 * @brief 初始化电机
 *
 */
/*
void SingleJointControl::on_initDriverPushButton_clicked()
{
    Drivers::initJoint(ui->comboBox->currentIndex());
}
*/
/**
 * @brief 紧急停止
 *
 */
/*
void SingleJointControl::on_emergencyStopPushButton_clicked()
{
    Drivers::stopJoint(ui->comboBox->currentIndex());
}
*/
/**
 * @brief 电机校准
 *
 */
/*
void SingleJointControl::on_caliPushButton_clicked()
{
    Drivers::calJoint(ui->comboBox->currentIndex());
}
*/
/**
 * @brief 紧急停止快捷键ALT+A
 *
 * @param e
 */
/*
void SingleJointControl::keyPressEvent(QKeyEvent *e)
{
    switch (e->key())
    {
    case Qt::Key_A:
    {
        if (QApplication::keyboardModifiers() == Qt::AltModifier)
        {
            on_emergencyStopPushButton_clicked();
        }
        break;
    }
    }
}
*/
/**
 * @brief 选择需要进行操控的CAN节点
 *
 * @param index 节点ID
 */
/*
void SingleJointControl::on_comboBox_currentIndexChanged(int index)
{
    setCurrentNode(index);
}

*/
ReconfigControl::~ReconfigControl()
{
    delete ui;
}



