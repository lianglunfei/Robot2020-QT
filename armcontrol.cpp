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



#include <QGuiApplication>

#include <Qt3DRender/qcamera.h>
#include <Qt3DCore/qentity.h>
#include <Qt3DRender/qcameralens.h>

#include <QtWidgets/QApplication>
#include <QtWidgets/QWidget>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QCommandLinkButton>
#include <QtGui/QScreen>

#include <Qt3DInput/QInputAspect>

#include <Qt3DExtras/qtorusmesh.h>
#include <Qt3DRender/qmesh.h>
#include <Qt3DRender/qtechnique.h>
#include <Qt3DRender/qmaterial.h>
#include <Qt3DRender/qeffect.h>
#include <Qt3DRender/qtexture.h>
#include <Qt3DRender/qrenderpass.h>
#include <Qt3DRender/qsceneloader.h>
#include <Qt3DRender/qpointlight.h>

#include <Qt3DCore/qtransform.h>
#include <Qt3DCore/qaspectengine.h>

#include <Qt3DRender/qrenderaspect.h>
#include <Qt3DExtras/qforwardrenderer.h>

#include <Qt3DExtras/qt3dwindow.h>
#include <Qt3DExtras/qfirstpersoncameracontroller.h>
#include <Qt3DExtras/qorbitcameracontroller.h>

inline double rad2degree(double x){
    return (x > 0 ? x : (2*PI + x)) * 180 / PI;
}

inline double degree2rad(double x){
    return (x > 0 ? x : (360 + x)) * PI / 180;
}



ArmControl::ArmControl(QWidget *parent) : QDialog(parent),
                                          ui(new Ui::ArmControl)
{
    ui->setupUi(this);
    this->setWindowTitle(tr("Arm Control"));
    taskTimer = new QTimer;
    taskThread = new QThread;
    worker = new ArmSyncWorker();
    taskTimer->setTimerType(Qt::PreciseTimer);
    taskTimer->setInterval(100);
    taskTimer->moveToThread(taskThread);
    paintTimer=new QTimer(this);
//    paintTimer->start(50);
//    connect(paintTimer,SIGNAL(timeout()),this,SLOT(updateModel()));
    connect(taskTimer, SIGNAL(timeout()), this, SLOT(syncPosition()), Qt::DirectConnection);
    connect(this, SIGNAL(stopThread()), taskThread, SLOT(quit()));

    Eigen::Matrix<double, 6, 1> a;
    Eigen::Matrix<double, 6, 1> alpha;
    Eigen::Matrix<double, 6, 1> d;
    Eigen::Matrix<double, 6, 1> theta;
    a << 0, -425, -392.25, 0, 0, 0;
    alpha << PI/2, 0, 0,PI/2, -PI/2, 0;
    d << 89.459, 0, 0, 109.15, 94.65, 82.3;
    theta << 0, 0, 0, 0, 0, 0;

    //定义ur对象
    ur = new arm(a, alpha, d, theta);




    initObject();
    if (initUiObject())
    {
        initConnection();
        controlWidgetInit();
    }
    connect(ui->syncCheckBox, &QCheckBox::toggled, [this](bool set) {
             set? startSync():stopSync();
        });

    //3d模型窗口
    Qt3DExtras::Qt3DWindow *view = new Qt3DExtras::Qt3DWindow();
    view->defaultFrameGraph()->setClearColor(QColor(QRgb(0x4d4d4f)));

    QWidget *container = QWidget::createWindowContainer(view);
    QSize screenSize = view->screen()->size();
//    container->setMinimumSize(QSize(400, 100));
    container->setMaximumSize(screenSize);

    QWidget *widget = ui->ArmWidget;
    QHBoxLayout *hLayout = new QHBoxLayout(widget);

    hLayout->addWidget(container, 1);


//    widget->setWindowTitle(QStringLiteral("Basic shapes"));

    Qt3DInput::QInputAspect *input = new Qt3DInput::QInputAspect;
    view->registerAspect(input);

    // Root entity
    Qt3DCore::QEntity *rootEntity = new Qt3DCore::QEntity();

    // Camera
    Qt3DRender::QCamera *cameraEntity = view->camera();


    cameraEntity->setPosition(QVector3D(0, 50,-10));
    cameraEntity->setViewCenter(QVector3D(0, 0, 5));

    Qt3DCore::QEntity *lightEntity = new Qt3DCore::QEntity(rootEntity);
    Qt3DRender::QPointLight *light = new Qt3DRender::QPointLight(lightEntity);
    light->setColor("white");
    light->setIntensity(1);
    lightEntity->addComponent(light);
    Qt3DCore::QTransform *lightTransform = new Qt3DCore::QTransform(lightEntity);
    lightTransform->setTranslation(QVector3D(10, 20, 30.0f));
    lightEntity->addComponent(lightTransform);

    // For camera controls
    Qt3DExtras::QOrbitCameraController *camController = new Qt3DExtras::QOrbitCameraController(rootEntity);
    camController->setCamera(cameraEntity);

    // Scenemodifier
    modifier = new ArmScene(rootEntity);

    // Set root object of the scene
    view->setRootEntity(rootEntity);

    // Create control widgets
    QCommandLinkButton *info = new QCommandLinkButton();
    info->setText(QStringLiteral("Qt3D ready-made meshes"));
    info->setDescription(QString::fromLatin1("Qt3D provides several ready-made meshes, like torus, cylinder, cone, "
                                             "cube, plane and sphere."));
    info->setIconSize(QSize(0,0));





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
    {
        if ((Spin == findChild<QDoubleSpinBox *>(positionSpinBox[i]) || Slider == findChild<MyCustomSlider *>(positionSlider[i])))
        {
            int motorID = motorIDs[i]-1;
            readyToSendCanData[motorID]=
                findChild<QDoubleSpinBox *>(positionSpinBox[i])->text().toDouble();
            armAngle[i] =  degree2rad(readyToSendCanData[motorID]);
            updateModel();
//            qDebug()<<i<<": "<<armAngle[i];
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

// 通过TCP通信控制时的逻辑
void ArmControl::comDataRecv(rawData data)
{

    Eigen::Matrix4d T;
    T << data.R[0][0],data.R[0][1],data.R[0][2],data.p[0],
         data.R[1][0],data.R[1][1],data.R[1][2],data.p[1],
         data.R[2][0],data.R[2][1],data.R[2][2],data.p[2],
         0,0,0,0;
    // 求逆解
    Eigen::Matrix<double, 8, 6> solve = ur->ikine(T);
    // 取第一个解
    int choice = 0;
    ui->statusLabel->setText(tr("收到数据"));

    if(ui->comPreviewCheckBox->isChecked())
    {

        for (int i = 0; i < ARM_NODE_NUM; i++)
        {
            armAngle[i] = rad2degree(solve(choice,i));
//

            findChild<QDoubleSpinBox *>(positionSpinBox[i])->setValue(armAngle[i]);
            findChild<MyCustomSlider *>(positionSlider[i])->doubleSetValue(armAngle[i]);
        }
        updateModel();
    }

    if(ui->comCheckBox->isChecked())
    {
        for (int i = 0; i < ARM_NODE_NUM; i++)
        {
            armAngle[i] = rad2degree(solve(choice,i));
            int motorID = motorIDs[i]-1;
            readyToSendCanData[motorID]=armAngle[i];
        }
        updateModel();
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

    for(int i=0;i<6;i++)
    {
        armAngle[i] = 0.0f;
    }
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


void ArmControl::updateModel()
{
//    double a1[3][6] = {0};
    Eigen::Matrix<double, 6, 1> q(armAngle);
    Eigen::Matrix<double, 3, 6> T = ur->fkine2(q);
//    qDebug()<<T2(0,0)<<T2(0,1)<<T2(0,2);
    modifier->setArms(T.col(0).data(),
                      T.col(1).data(),
                      T.col(2).data(),
                      T.col(3).data(),
                      T.col(4).data(),
                      T.col(5).data());
//    updateModel();
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


