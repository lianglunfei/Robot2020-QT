/*
 * @Author: xingzhang.Wu 
 * @Date: 2019-09-29 10:05:06 
 * @Last Modified by:   xingzhang.Wu 
 * @Last Modified time: 2019-09-29 10:05:06 
 */
#include "singlejointcontrol.h"
#include "ui_singlejointcontrol.h"

#include <QKeyEvent>

#include "drivers.h"
#include "qdebug.h"

/**
 * @brief Construct a new Single Joint Control:: Single Joint Control object
 * 
 */
SingleJointControl::SingleJointControl() : ui(new Ui::SingleJointControl)
{
    ui->setupUi(this);
    this->setWindowTitle(tr("Single Joint Control"));

    initObject();
    if (initUiObject())
    {
        initConnection();
        controlWidgetInit();
    }
    setCurrentNode(0);
}

/**
 * @brief 初始化界面信息，字符串代表界面控件的object名称
 * 
 */
void SingleJointControl::initObject()
{
    setSpeedSlider(QList<QString>({"speedHorizontalSlider"}));
    setPositionSlider(QList<QString>({"positionHorizontalSlider"}));
    setSpeedSpinBox(QList<QString>({"speedDoubleSpinBox"}));
    setPositionSpinBox(QList<QString>({"positionDoubleSpinBox"}));
    setPositionSetButton(QList<QString>({"positionSetButton"}));
    setSpeedSetButton(QList<QString>({"speedSetButton"}));
    setForwardReversePushButton(QList<QString>({"posForwardPushButton", "posReversalPushButton"}));
    setRelativeSpinBox(QList<QString>({"relativePosDoubleSpinBox"}));
}

/**
 * @brief 初始化电机
 * 
 */
void SingleJointControl::on_initDriverPushButton_clicked()
{
    Drivers::initJoint(ui->comboBox->currentIndex());
}

/**
 * @brief 紧急停止
 * 
 */
void SingleJointControl::on_emergencyStopPushButton_clicked()
{
    Drivers::stopJoint(ui->comboBox->currentIndex());
}

/**
 * @brief 电机校准
 * 
 */
void SingleJointControl::on_caliPushButton_clicked()
{
    Drivers::calJoint(ui->comboBox->currentIndex());
}

/**
 * @brief 电机状态位清除
 *
 */
void SingleJointControl::on_faultPushButton_clicked()
{
    Drivers::faultJoint(ui->comboBox->currentIndex());
}


/**
 * @brief 打开关节电磁阀
 *
 */
void SingleJointControl::on_OpenValvePushButton_clicked()
{
    Drivers::initValve(ui->comboBox->currentIndex());
}


/**
 * @brief 关闭关节电磁阀
 *
 */
void SingleJointControl::on_CloseValvePushButton_clicked()
{
    Drivers::CloseValve(ui->comboBox->currentIndex());
}





/**
 * @brief 紧急停止快捷键ALT+A
 * 
 * @param e 
 */
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

/**
 * @brief 选择需要进行操控的CAN节点
 * 
 * @param index 节点ID
 */
void SingleJointControl::on_comboBox_currentIndexChanged(int index)
{
    setCurrentNode(index);
}

SingleJointControl::~SingleJointControl()
{
    delete ui;
}
