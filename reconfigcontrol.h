#ifndef RECONFIGCONTROL_H
#define RECONFIGCONTROL_H

#include <QDialog>

#include "globaldata.h"
//#include "controls.h"

#define RE_NODE_NUM 2
#define RE_SPEED_MIN -80000
#define RE_SPEED_MAX 80000
#define RE_POSITION_MIN 0
#define RE_POSITION_MAX 3600

namespace Ui
{
class ReconfigControl;
}

class ReconfigControl : public QDialog
{
    Q_OBJECT

public:
//    explicit ReconfigControl();
//    ~ReconfigControl();

    explicit ReconfigControl(QWidget *parent = 0);
    virtual ~ReconfigControl();

    void initConnection();
    void controlWidgetInit();
    bool initUiObject();

    void setSpeedSlider(const QList<QString> &l);
    void setPositionSlider(const QList<QString> &l);
    void setSpeedSpinBox(const QList<QString> &l);
    void setPositionSpinBox(const QList<QString> &l);
    void setSpeedSetButton(const QList<QString> &l);
    void setPositionSetButton(const QList<QString> &l);
    void setForwardReversalPushButton(const QList<QString> &l);
    void setRelativeSpinBox(const QList<QString> &l);





    QStringList PositionSlider;
    QStringList PositionSpinBox;
    QStringList PositionSetButton;
    QStringList SpeedSlider;
    QStringList SpeedSpinBox;
    QStringList SpeedSetButton;
    QStringList ForwardReversalPushButton;
    QStringList RelativeSpinBox;

    double ReadyToSendCanData[RE_NODE_NUM];
    int ReconfigIDs[RE_NODE_NUM];
    double ReadPlatformSpeedData,ReadPlatformPositionData,ReadReconfigSpeedData,ReadReconfigPositionData;

private:

    void initObject();
    void ReconfigAct(int);
    void PlatformAct(int);
    void PlatformSpeedControl(double);
    void PlatformPositionControl(double);
    void ReconfigSpeedControl(double);
    void ReconfigPositionControl(double);


    Ui::ReconfigControl *ui;


private slots:
//    void on_initDriverPushButton_clicked();

//    void on_emergencyStopPushButton_clicked();

//    void on_comboBox_currentIndexChanged(int index);

//    void on_caliPushButton_clicked();

//    void on_ReconfigControl_accepted();
    void setPosButtonClicked();
    void setSpeedButtonClicked();
    void speedValueChanged();
    void setSpeedButtonReleased();
    void posValueChanged();
    void ForwardReversalPushbtnClicked();

    void on_PlatformReModepushButton_clicked();

    void on_PlatformFWpushButton_pressed();
    void on_PlatformFWpushButton_released();

    void on_PlatformBWpushButton_pressed();
    void on_PlatformBWpushButton_released();


//    void on_ReOpenpushButton_clicked();
//    void on_ReClosepushButton_clicked();

    void on_ReOpenpushButton_pressed();
    void on_ReOpenpushButton_released();

    void on_ReClosepushButton_pressed();
    void on_ReClosepushButton_released();


//    void keyPressEvent(QKeyEvent *e) override;
};
#endif // RECONFIGCONTROL_H
