#include "armcontrol.h"
#include "ui_armcontrol.h"

#include "mycustomslider.h"
#include "package.h"
#include "debug.h"
#include "drivers.h"

#include <QDoubleSpinBox>
#include <QPushButton>
#include <QCheckBox>
#include <QMessageBox>


ArmControl::ArmControl(QWidget *parent) : QDialog(parent),
                                          ui(new Ui::ArmControl)
{
    ui->setupUi(this);
    taskTimer = new QTimer;
    taskThread = new QThread;
    worker = new ArmSyncWorker();
    taskTimer->setTimerType(Qt::PreciseTimer);
    taskTimer->setInterval(100);
    taskTimer->moveToThread(taskThread);
    connect(taskTimer, SIGNAL(timeout()), this, SLOT(syncPosition()), Qt::DirectConnection);
    connect(this, SIGNAL(stopThread()), taskThread, SLOT(quit()));


    this->setWindowTitle(tr("Arm Control"));

    initObject();
    if (initUiObject())
    {
        initConnection();
        controlWidgetInit();
    }
    connect(ui->syncCheckBox, &QCheckBox::toggled, [this](bool set) {
             set? startSync():stopSync();
        });


}

ArmControl::~ArmControl()
{
    delete ui;
    delete taskTimer;
    delete taskThread;
    delete worker;
}

/**
*@projectName   RobotControlSystem
*@brief         判断对象数组是否有赋值，如果没有则返回false
*@parameter
*@author        XingZhang.Wu
*@date          20190731
**/
bool ArmControl::initUiObject()
{
    if (positionSlider.length() == 0 || positionSpinBox.length() == 0 )
        return false;
    return true;
}

/**
*@projectName   RobotControlSystem
*@brief         界面滑条、方框设定最大最小值等
*@parameter
*@author        XingZhang.Wu
*@date          20190731
**/
void ArmControl::controlWidgetInit()
{

    for (int i = 0; i < positionSlider.length(); i++)
    {
        findChild<MyCustomSlider *>(positionSlider[i])->setRange(POSITION_MIN, POSITION_MAX);
    }

    for (int i = 0; i < positionSpinBox.length(); i++)
    {
        findChild<QDoubleSpinBox *>(positionSpinBox[i])->setRange(POSITION_MIN, POSITION_MAX);
    }
}

/**
*@projectName   RobotControlSystem
*@brief         信号与槽的初始化
*@parameter
*@author        XingZhang.Wu
*@date          20190731
**/
void ArmControl::initConnection()
{

    for (int i = 0; i < positionSpinBox.length(); i++)
    {
        connect(findChild<QDoubleSpinBox *>(positionSpinBox[i]), SIGNAL(valueChanged(double)),
                findChild<MyCustomSlider *>(positionSlider[i]), SLOT(doubleSetValue(double)));
        connect(findChild<QDoubleSpinBox *>(positionSpinBox[i]), SIGNAL(editingFinished()),
                this, SLOT(posValueChanged()));
        connect(findChild<QDoubleSpinBox *>(positionSpinBox[i]), SIGNAL(editingFinished()),
                this, SLOT(setPosButtonClicked()));
    }
    for (int i = 0; i < positionSlider.length(); i++)
    {
        connect(findChild<MyCustomSlider *>(positionSlider[i]), SIGNAL(doubleValueChanged(double)),
                findChild<QDoubleSpinBox *>(positionSpinBox[i]), SLOT(setValue(double)));
        connect(findChild<MyCustomSlider *>(positionSlider[i]), SIGNAL(sliderReleased()),
                this, SLOT(posValueChanged()));
        connect(findChild<MyCustomSlider *>(positionSlider[i]), SIGNAL(sliderReleased()),
                this, SLOT(setPosButtonClicked()));
    }
    connect(worker, &ArmSyncWorker::recvData, this, &ArmControl::comDataRecv);

}

void ArmControl::setCurrentNode(int nodeNum)
{
    currentNode = nodeNum > ARM_NODE_NUM - 1 ? ARM_NODE_NUM - 1 : nodeNum;
}


void ArmControl::posValueChanged()
{
    auto *Spin = qobject_cast<QDoubleSpinBox *>(this->sender());
    auto *Slider = qobject_cast<MyCustomSlider *>(this->sender());

    for (int i = 0; i < ARM_NODE_NUM; i++)
    { //4 parts
        if ((Spin == findChild<QDoubleSpinBox *>(positionSpinBox[i]) || Slider == findChild<MyCustomSlider *>(positionSlider[i])))
        {
            int motorID = motorIDs[i]-1;
            readyToSendCanData[motorID]=
                findChild<QDoubleSpinBox *>(positionSpinBox[i])->text().toDouble();
        }

    }

}

/**
*@projectName   RobotControlSystem
*@brief         设置按钮处理函数
*@parameter
*@author        XingZhang.Wu
*@date          20190731
**/
void ArmControl::setPosButtonClicked()
{
//    auto *btn = qobject_cast<QPushButton *>(this->sender());

    Package::packOperateMulti(globalData->sendId, readyToSendCanData,
                         NODE_NUM, PROTOCOL_TYPE_POS);

}

void ArmControl::setPositionSlider(const QList<QString> &l)
{
    positionSlider = l;
}

void ArmControl::setPositionSpinBox(const QList<QString> &l)
{
    positionSpinBox = l;
}

void ArmControl::setPositionSetButton(const QList<QString> &l)
{
    positionSetButton = l;
}

void ArmControl::syncPosition(){
    for (int i = 0; i < ARM_NODE_NUM; i++)
    {
        int motorID = motorIDs[i]-1;
        double position = globalData->currentCanAnalyticalData[motorID].position;
        findChild<QDoubleSpinBox *>(positionSpinBox[i])->setValue(position);
        findChild<MyCustomSlider *>(positionSlider[i])->doubleSetValue(position);
    }
}

void ArmControl::startSync()
{
    QTimer::singleShot(0, taskTimer, SLOT(start()));
    taskThread->start();
}

void ArmControl::stopSync()
{
//    taskTimer->stop();
    emit stopThread();

}

void ArmControl::comDataRecv(double data[])
{
    ui->statusLabel->setText(tr("收到数据"));

    if(ui->comPreviewCheckBox->isChecked())
    {
        for (int i = 0; i < ARM_NODE_NUM; i++)
        {
            findChild<QDoubleSpinBox *>(positionSpinBox[i])->setValue(data[i]);
            findChild<MyCustomSlider *>(positionSlider[i])->doubleSetValue(data[i]);
        }

    }

    if(ui->comCheckBox->isChecked())
    {
        for (int i = 0; i < ARM_NODE_NUM; i++)
        {
            int motorID = motorIDs[i]-1;
            readyToSendCanData[motorID]=data[i];
        }
        setPosButtonClicked();
    }


}

/**
 * @brief 完成界面对象的赋值，该值从UI中获取
 *
 */
void ArmControl::initObject()
{

    setPositionSlider(QList<QString>({"positionHorizontalSlider", "positionHorizontalSlider_2", "positionHorizontalSlider_3",
                                      "positionHorizontalSlider_4", "positionHorizontalSlider_5", "positionHorizontalSlider_6"}));

    setPositionSpinBox(QList<QString>({"positionDoubleSpinBox", "positionDoubleSpinBox_2", "positionDoubleSpinBox_3",
                                       "positionDoubleSpinBox_4", "positionDoubleSpinBox_5", "positionDoubleSpinBox_6"}));

    update_motor_ids();
}

void ArmControl::update_motor_ids(){
    motorIDs[0] = ui->comboBox_0->currentText().toInt();
    motorIDs[1] = ui->comboBox_1->currentText().toInt();
    motorIDs[2] = ui->comboBox_2->currentText().toInt();
    motorIDs[3] = ui->comboBox_3->currentText().toInt();
    motorIDs[4] = ui->comboBox_4->currentText().toInt();
    motorIDs[5] = ui->comboBox_5->currentText().toInt();
}

/**
 * @brief 初始化所有电机
 *
 */
void ArmControl::on_initDriverPushButton_clicked()
{
    Drivers::initJoint();
    update_motor_ids();

    bool normal_status = 1;
    QString rtn_msg = QString("");
    double position;
    for (int i = 0; i < NODE_NUM; i++)
    {
        position = globalData->currentCanAnalyticalData[i].position;
        if ((359.0 < position && 360.0 > position) || globalData->statusId[i] != 0x06)
        {
            rtn_msg.append(tr("    电机%1 异常:%2！\n").arg(i + 1).arg(position));
            normal_status = 0;
        }
        else
        {
            rtn_msg.append(tr("[-] 电机%1 正常。%2 \n").arg(i + 1).arg(position));
        }
    }
    rtn_msg = normal_status ? QString("所有电机正常。") : rtn_msg;
    QMessageBox::information(this, tr("电机状态提示"),
                             rtn_msg,
                             QMessageBox::Ok);
}

/**
 * @brief 紧急停止所有电机
 *
 */
void ArmControl::on_emergencyStopPushButton_clicked()
{
    Drivers::stopJoint();
}

/**
 * @brief sync所有电机
 *
 */
void ArmControl::on_caliPushButton_clicked()
{
    syncPosition();
}

/**
 * @brief 键盘快捷键设置
 *
 * @param e
 */
void ArmControl::keyPressEvent(QKeyEvent *e)
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
