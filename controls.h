#ifndef CONTROLS_H
#define CONTROLS_H

#include <QDialog>

#include "globaldata.h"

#define SPEED_MIN       -38500
#define SPEED_MAX       38500
#define POSITION_MIN    0
#define POSITION_MAX    3600

class Controls : public QDialog
{
    Q_OBJECT

public:
    explicit Controls(QWidget *parent = 0);

    void initConnection();
    void controlWidgetInit();
    bool initUiObject();
    void setCurrentNode(int nodeNum);

    void setSpeedSlider(const QList<QString>& l);
    void setPositionSlider(const QList<QString>& l);
    void setSpeedSpinBox(const QList<QString>& l);
    void setPositionSpinBox(const QList<QString>& l);
    void setSpeedSetButton(const QList<QString>& l);
    void setPositionSetButton(const QList<QString>& l);
    void setForwardReversePushButton(const QList<QString>& l);
    void setRelativeSpinBox(const QList<QString>& l);

private:
    CanAnalysis readyToSendCanData[NODE_NUM];
    QStringList speedSlider;
    QStringList positionSlider;
    QStringList speedSpinBox;
    QStringList positionSpinBox;
    QStringList speedSetButton;
    QStringList positionSetButton;
    QStringList forwardReversePushButton;
    QStringList relativeSpinBox;
    int currentNode;

    void forwardRotation(int idIndex, double readValue);
    void reverseRotation(int idIndex, double readValue);

private slots:
    void setPosButtonClicked();
    void setSpeedButtonClicked();
    void setSpeedButtonReleased();
    void speedValueChanged();
    void posValueChanged();
    void forwardReversalPushbtnClicked();
};

#endif // CONTROLS_H
