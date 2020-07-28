#ifndef RECONFIGCONTROL_H
#define RECONFIGCONTROL_H

#include <QDialog>

#include "globaldata.h"
//#include "controls.h"

#define RE_NODE_NUM 2
#define SPEED_MIN -3000
#define SPEED_MAX 3000
#define POSITION_MIN 0
#define POSITION_MAX 360

namespace Ui
{
class ReconfigControl;
}

class ReconfigControl : public QDialog
{
    Q_OBJECT

public:
    explicit ReconfigControl();
    ~ReconfigControl();

    void initConnection();
    void controlWidgetInit();
    bool initUiObject();

    void setSpeedSlider(const QList<QString> &l);
    void setPositionSlider(const QList<QString> &l);
    void setSpeedSpinBox(const QList<QString> &l);
    void setPositionSpinBox(const QList<QString> &l);
    void setSpeedSetButton(const QList<QString> &l);
    void setPositionSetButton(const QList<QString> &l);
    void setForwardReversePushButton(const QList<QString> &l);
    void setRelativeSpinBox(const QList<QString> &l);


    QStringList PositionSlider;
    QStringList PositionSpinBox;
    QStringList PositionSetButton;
    QStringList SpeedSlider;
    QStringList SpeedSpinBox;
    QStringList SpeedSetButton;
    QStringList ForwardReversePushButton;
    QStringList RelativeSpinBox;

    double ReadyToSendCanData[RE_NODE_NUM];
    int ReconfigIDs[RE_NODE_NUM];

private:
    void ReconfigAct(int);
    void PlatformAct(int);
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
    void forwardReversalPushbtnClicked();

    void on_PlatformFWpushButton_clicked();
    void on_PlatformBWpushButton_clicked();
    void on_ReOpenpushButton_clicked();
    void on_ReClosepushButton_clicked();



private:
    Ui::ReconfigControl *ui;

    void initObject();

//    void keyPressEvent(QKeyEvent *e) override;
};
#endif // RECONFIGCONTROL_H
