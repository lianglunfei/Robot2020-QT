#ifndef ARMCONTROL_H
#define ARMCONTROL_H


#include <QDialog>

#include "globaldata.h"
#include "armsyncworker.h"
#include "kinemetics.h"
#include "Eigen/Core"
#include "Eigen/Dense"
#include "armscene.h"

#include <QTimer>
#include <QThread>

#define ARM_NODE_NUM 6
#define SPEED_MIN -38500
#define SPEED_MAX 38500
#define POSITION_MIN 0
#define POSITION_MAX 3600

namespace Ui {
class ArmControl;
}

class ArmControl : public QDialog
{
    Q_OBJECT

public:
    explicit ArmControl(QWidget *parent = 0);
    virtual ~ArmControl();

    int motorIDs[ARM_NODE_NUM];
    void initConnection();
    void controlWidgetInit();
    void initModel();
    bool initUiObject();
    void setCurrentNode(int nodeNum);
    void setPositionSlider(const QList<QString> &l);
    void setPositionSpinBox(const QList<QString> &l);
    void setPositionSetButton(const QList<QString> &l);

    void startSync();
    void stopSync();

    double readyToSendCanData[NODE_NUM];
    QStringList positionSlider;
    QStringList positionSpinBox;
    QStringList positionSetButton;
    int currentNode;

private:
    Ui::ArmControl *ui;
    QTimer *taskTimer;
    QTimer *paintTimer;
    QThread *taskThread;
    ArmSyncWorker *worker;
    double armAngle[6];
    double pos[3];
    arm *ur;
    ArmScene *modifier;

    void initObject();
    void keyPressEvent(QKeyEvent *e) override;
    void updateModel(int);
    void clawAct(int);

public slots:
    void syncPosition();


private slots:
    void setPosButtonClicked();
    void posValueChanged();
    void comDataRecv(rawData);
    void on_initDriverPushButton_clicked();
    void on_emergencyStopPushButton_clicked();
    void on_caliPushButton_clicked();
    void on_clawOpenPushButton_pressed();
    void on_clawOpenPushButton_released();
    void on_clawClosePushButton_pressed();
    void on_clawClosePushButton_released();
    void update_motor_ids();

signals:
    void stopThread();
};

#endif // ARMCONTROL_H
