#include "singlejointcontrol.h"
#include "ui_singlejointcontrol.h"

#include <QKeyEvent>

#include "drivers.h"
#include "qdebug.h"

SingleJointControl::SingleJointControl() :
    ui(new Ui::SingleJointControl)
{
    ui->setupUi(this);
    this->setWindowTitle(tr("Single Joint Control"));

    initObject();
    if(initUiObject()) {
        initConnection();
        controlWidgetInit();
    }
    setCurrentNode(0);
}

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

void SingleJointControl::on_initDriverPushButton_clicked()
{
    Drivers::initJoint(ui->comboBox->currentIndex());
}

void SingleJointControl::on_emergencyStopPushButton_clicked()
{
    Drivers::stopJoint(ui->comboBox->currentIndex());
}

void SingleJointControl::on_caliPushButton_clicked()
{
    Drivers::calJoint(ui->comboBox->currentIndex());
}

void SingleJointControl::keyPressEvent(QKeyEvent *e)
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

void SingleJointControl::on_comboBox_currentIndexChanged(int index)
{
    setCurrentNode(index);
}

SingleJointControl::~SingleJointControl()
{
    delete ui;
}
