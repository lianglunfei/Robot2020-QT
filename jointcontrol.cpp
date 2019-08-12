#include "jointcontrol.h"
#include "ui_jointcontrol.h"

#include <QKeyEvent>

#include "drivers.h"
#include "debug.h"

/**
*@projectName   RobotControlSystem
*@brief         继承类，继承于Controls，只需要完成界面对象的赋值即可，
*               然后调用基类的一系列初始化方法就能够完成界面的搭建，
*               比如initConnection、controlWidgetInit
*@parameter
*@author        XingZhang.Wu
*@date          20190731
**/
JointControl::JointControl() :
    ui(new Ui::JointControl)
{
    ui->setupUi(this);
    this->setWindowTitle(tr("Joint Control"));

    initObject();
    if(initUiObject()) {
        initConnection();
        controlWidgetInit();
    }
}

/**
*@projectName   RobotControlSystem
*@brief         完成界面对象的赋值，该值从UI中获取
*@parameter
*@author        XingZhang.Wu
*@date          20190731
**/
void JointControl::initObject()
{
    setSpeedSlider(QList<QString>({"speed11HorizontalSlider", "speed12HorizontalSlider", "speed13HorizontalSlider",
                                   "speed21HorizontalSlider", "speed22HorizontalSlider", "speed23HorizontalSlider",
                                   "speed31HorizontalSlider", "speed32HorizontalSlider", "speed33HorizontalSlider",
                                   "speed41HorizontalSlider", "speed42HorizontalSlider", "speed43HorizontalSlider"}));
    setPositionSlider(QList<QString>({"position11HorizontalSlider", "position12HorizontalSlider", "position13HorizontalSlider",
                                      "position21HorizontalSlider", "position22HorizontalSlider", "position23HorizontalSlider",
                                      "position31HorizontalSlider", "position32HorizontalSlider", "position33HorizontalSlider",
                                      "position41HorizontalSlider", "position42HorizontalSlider", "position43HorizontalSlider"}));
    setSpeedSpinBox(QList<QString>({"speed11DoubleSpinBox", "speed12DoubleSpinBox", "speed13DoubleSpinBox",
                                    "speed21DoubleSpinBox", "speed22DoubleSpinBox", "speed23DoubleSpinBox",
                                    "speed31DoubleSpinBox", "speed32DoubleSpinBox", "speed33DoubleSpinBox",
                                    "speed41DoubleSpinBox", "speed42DoubleSpinBox", "speed43DoubleSpinBox"}));
    setPositionSpinBox(QList<QString>({"position11DoubleSpinBox", "position12DoubleSpinBox", "position13DoubleSpinBox",
                                       "position21DoubleSpinBox", "position22DoubleSpinBox", "position23DoubleSpinBox",
                                       "position31DoubleSpinBox", "position32DoubleSpinBox", "position33DoubleSpinBox",
                                       "position41DoubleSpinBox", "position42DoubleSpinBox", "position43DoubleSpinBox"}));
    setPositionSetButton(QList<QString>({"position11SetButton", "position12SetButton", "position13SetButton",
                                         "position21SetButton", "position22SetButton", "position23SetButton",
                                         "position31SetButton", "position32SetButton", "position33SetButton",
                                         "position41SetButton", "position42SetButton", "position43SetButton"}));
    setSpeedSetButton(QList<QString>({"speed11SetButton", "speed12SetButton", "speed13SetButton",
                                      "speed21SetButton", "speed22SetButton", "speed23SetButton",
                                      "speed31SetButton", "speed32SetButton", "speed33SetButton",
                                      "speed41SetButton", "speed42SetButton", "speed43SetButton"}));
    setForwardReversePushButton(QList<QString>({"pos11ForwardPushButton", "pos12ForwardPushButton", "pos13ForwardPushButton",
                                                "pos21ForwardPushButton", "pos22ForwardPushButton", "pos23ForwardPushButton",
                                                "pos31ForwardPushButton", "pos32ForwardPushButton", "pos33ForwardPushButton",
                                                "pos41ForwardPushButton", "pos42ForwardPushButton", "pos43ForwardPushButton",
                                                "pos11ReversalPushButton", "pos12ReversalPushButton", "pos13ReversalPushButton",
                                                "pos21ReversalPushButton", "pos22ReversalPushButton", "pos23ReversalPushButton",
                                                "pos31ReversalPushButton", "pos32ReversalPushButton", "pos33ReversalPushButton",
                                                "pos41ReversalPushButton", "pos42ReversalPushButton", "pos43ReversalPushButton"}));
    setRelativeSpinBox(QList<QString>({"relativePos11DoubleSpinBox", "relativePos12DoubleSpinBox", "relativePos13DoubleSpinBox",
                                       "relativePos21DoubleSpinBox", "relativePos22DoubleSpinBox", "relativePos23DoubleSpinBox",
                                       "relativePos31DoubleSpinBox", "relativePos32DoubleSpinBox", "relativePos33DoubleSpinBox",
                                       "relativePos41DoubleSpinBox", "relativePos42DoubleSpinBox", "relativePos43DoubleSpinBox"}));
}

JointControl::~JointControl()
{
    delete ui;
}

void JointControl::on_initDriverPushButton_clicked()
{
    Drivers::initJoint();
}

void JointControl::on_emergencyStopPushButton_clicked()
{
    Drivers::stopJoint();
}

void JointControl::keyPressEvent(QKeyEvent *e)
{
    switch(e->key())
    {
    case Qt::Key_A:
    {
        if (QApplication::keyboardModifiers() == Qt::AltModifier) {
            on_emergencyStopPushButton_clicked();
        }
        break;
    }
    }
}
