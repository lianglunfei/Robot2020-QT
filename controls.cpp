#include "controls.h"

#include "doubleslider.h"
#include "package.h"
#include "debug.h"

#include <QDoubleSpinBox>
#include <QPushButton>

/**
*@projectName   RobotControlSystem
*@brief         基类，用于实现控制界面的基本操作，
*               包括界面的object初始化绑定、信号与槽的初始化、
*               各种事件处理函数的实现，由于这些为公共接口，无需在子类中再次实现。
*@parameter
*@author        XingZhang.Wu
*@date          20190731
**/
Controls::Controls(QWidget *parent) :
    QDialog(parent),
    currentNode(-1)
{
}

/**
*@projectName   RobotControlSystem
*@brief         判断对象数组是否有赋值，如果没有则返回false
*@parameter
*@author        XingZhang.Wu
*@date          20190731
**/
bool Controls::initUiObject()
{
    if(speedSlider.length()==0 || positionSlider.length()==0 || speedSpinBox.length()==0 || positionSpinBox.length()==0
            || positionSetButton.length()==0 || speedSetButton.length()==0 || forwardReversePushButton.length()==0
            || relativeSpinBox.length()==0)
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
void Controls::controlWidgetInit()
{
    for(int i=0;i<speedSlider.length();i++) {
        findChild<DoubleSlider*>(speedSlider[i])->setRange(SPEED_MIN, SPEED_MAX);
    }
    for(int i=0;i<positionSlider.length();i++) {
        findChild<DoubleSlider*>(positionSlider[i])->setRange(POSITION_MIN, POSITION_MAX);
    }
    for(int i=0;i<speedSpinBox.length();i++) {
        findChild<QDoubleSpinBox*>(speedSpinBox[i])->setRange(SPEED_MIN, SPEED_MAX);
    }
    for(int i=0;i<positionSpinBox.length();i++) {
        findChild<QDoubleSpinBox*>(positionSpinBox[i])->setRange(POSITION_MIN, POSITION_MAX);
    }
}

/**
*@projectName   RobotControlSystem
*@brief         信号与槽的初始化
*@parameter
*@author        XingZhang.Wu
*@date          20190731
**/
void Controls::initConnection()
{
    for(int i=0;i<speedSpinBox.length();i++) {
        connect(findChild<QDoubleSpinBox*>(speedSpinBox[i]), SIGNAL(valueChanged(double)),
                findChild<DoubleSlider*>(speedSlider[i]), SLOT(doubleSetValue(double)));
        connect(findChild<QDoubleSpinBox*>(speedSpinBox[i]), SIGNAL(editingFinished()),
                this, SLOT(speedValueChanged()));
    }
    for(int i=0;i<positionSpinBox.length();i++) {
        connect(findChild<QDoubleSpinBox*>(positionSpinBox[i]), SIGNAL(valueChanged(double)),
                findChild<DoubleSlider*>(positionSlider[i]), SLOT(doubleSetValue(double)));
        connect(findChild<QDoubleSpinBox*>(positionSpinBox[i]), SIGNAL(editingFinished()),
                this, SLOT(posValueChanged()));
    }
    for(int i=0;i<positionSlider.length();i++) {
        connect(findChild<DoubleSlider*>(positionSlider[i]), SIGNAL(doubleValueChanged(double)),
                findChild<QDoubleSpinBox*>(positionSpinBox[i]), SLOT(setValue(double)));
        connect(findChild<DoubleSlider*>(positionSlider[i]), SIGNAL(sliderReleased()),
                this, SLOT(posValueChanged()));
    }
    for(int i=0;i<speedSlider.length();i++) {
        connect(findChild<DoubleSlider*>(speedSlider[i]), SIGNAL(doubleValueChanged(double)),
                findChild<QDoubleSpinBox*>(speedSpinBox[i]), SLOT(setValue(double)));
        connect(findChild<DoubleSlider*>(speedSlider[i]), SIGNAL(sliderReleased()),
                this, SLOT(speedValueChanged()));
    }
    for(int i=0;i<positionSetButton.length();i++) {
        connect(findChild<QPushButton*>(positionSetButton[i]), SIGNAL(pressed()),
                this, SLOT(setPosButtonClicked()));
    }
    for(int i=0;i<speedSetButton.length();i++) {
        connect(findChild<QPushButton*>(speedSetButton[i]), SIGNAL(pressed()),
                this, SLOT(setSpeedButtonClicked()));
        connect(findChild<QPushButton*>(speedSetButton[i]), SIGNAL(released()),
                this, SLOT(setSpeedButtonReleased()));
    }
    for(int i=0;i<forwardReversePushButton.length();i++) {
        connect(findChild<QPushButton*>(forwardReversePushButton[i]), SIGNAL(pressed()),
                this, SLOT(forwardReversalPushbtnClicked()));
    }
}

void Controls::setCurrentNode(int nodeNum)
{
    currentNode = nodeNum>NODE_NUM-1?NODE_NUM-1:nodeNum;
}

/**
*@projectName   RobotControlSystem
*@brief         正转反转函数处理
*@parameter
*@author        XingZhang.Wu
*@date          20190731
**/
void Controls::forwardReversalPushbtnClicked()
{
    auto *btn = qobject_cast<QPushButton *>(this->sender());
    double readValue;

    if(currentNode>0) {
        readValue = findChild<QDoubleSpinBox*>(relativeSpinBox[0])->text().toDouble();
        if(btn == findChild<QPushButton*>(forwardReversePushButton[0])) {
            forwardRotation(currentNode, readValue);
        }
        else if(btn == findChild<QPushButton*>(forwardReversePushButton[0])) {
            reverseRotation(currentNode, readValue);
        }
    } else {
        int len=forwardReversePushButton.length();
        for(int i=0;i<len/6;i++) {
            readValue = findChild<QDoubleSpinBox*>(relativeSpinBox[3*i])->text().toDouble();
            if((i*3) < NODE_NUM && btn == findChild<QPushButton*>(forwardReversePushButton[3*i])) {
                forwardRotation(0+3*i, readValue);
            }
            else if((i*3) < NODE_NUM && btn == findChild<QPushButton*>(forwardReversePushButton[(3*i)+len/2])) {
                reverseRotation(0+3*i, readValue);
            }

            readValue = findChild<QDoubleSpinBox*>(relativeSpinBox[(3*i)+1])->text().toDouble();
            if((i*3)+1 < NODE_NUM && btn == findChild<QPushButton*>(forwardReversePushButton[(3*i)+1])) {
                forwardRotation(1+3*i, readValue);
            }
            else if((i*3)+1 < NODE_NUM && btn == findChild<QPushButton*>(forwardReversePushButton[(3*i)+len/2+1])) {
                reverseRotation(1+3*i, readValue);
            }

            readValue = findChild<QDoubleSpinBox*>(relativeSpinBox[(3*i)+2])->text().toDouble();
            if((i*3)+2 < NODE_NUM && btn == findChild<QPushButton*>(forwardReversePushButton[(3*i)+2])) {
                forwardRotation(2+3*i, readValue);
            }
            else if((i*3)+2 < NODE_NUM && btn == findChild<QPushButton*>(forwardReversePushButton[(3*i)+len/2+2])) {
                reverseRotation(2+3*i, readValue);
            }
        }
    }
}

void Controls::forwardRotation(int idIndex, double readValue)
{
    if(idIndex >= NODE_NUM)
        return;
    double sendValue=0;

    //Temporarily annotate this feature
    sendValue = GlobalData::currentCanAnalyticalData[idIndex].position
            + readValue;
    Package::packOperate(GlobalData::sendId[idIndex], sendValue, PROTOCOL_TYPE_POS);
}

void Controls::reverseRotation(int idIndex, double readValue)
{
    if(idIndex >= NODE_NUM)
        return;
    double sendValue=0;

    //Temporarily annotate this feature
    sendValue = GlobalData::currentCanAnalyticalData[idIndex].position
            - readValue;
    Package::packOperate(GlobalData::sendId[idIndex], sendValue, PROTOCOL_TYPE_POS);
}

void Controls::speedValueChanged()
{
    auto *Spin = qobject_cast<QDoubleSpinBox *>(this->sender());
    auto *Slider = qobject_cast<DoubleSlider *>(this->sender());

    if(currentNode>0) {
        if(Spin == findChild<QDoubleSpinBox*>(speedSpinBox[0])
                || Slider == findChild<DoubleSlider*>(speedSlider[0])) {
            readyToSendCanData[currentNode].speed =
                    findChild<QDoubleSpinBox*>(speedSpinBox[0])->text().toDouble();
        }
    } else {
        for(int i=0;i<speedSpinBox.length()/3;i++) {//4 parts
            if((i*3) < NODE_NUM && Spin == findChild<QDoubleSpinBox*>(speedSpinBox[i*3])
                    || Slider == findChild<DoubleSlider*>(speedSlider[i*3])) {
                readyToSendCanData[i*3].speed =
                        findChild<QDoubleSpinBox*>(speedSpinBox[i*3])->text().toDouble();
            }
            else if((i*3)+1 < NODE_NUM && Spin == findChild<QDoubleSpinBox*>(speedSpinBox[(i*3)+1])
                    || Slider == findChild<DoubleSlider*>(speedSlider[(i*3)+1])) {
                readyToSendCanData[(i*3)+1].speed =
                        findChild<QDoubleSpinBox*>(speedSpinBox[(i*3)+1])->text().toDouble();
            }
            else if((i*3)+2 < NODE_NUM && Spin == findChild<QDoubleSpinBox*>(speedSpinBox[(i*3)+2])
                    || Slider == findChild<DoubleSlider*>(speedSlider[(i*3)+2])) {
                readyToSendCanData[(i*3)+2].speed =
                        findChild<QDoubleSpinBox*>(speedSpinBox[(i*3)+2])->text().toDouble();
            }
        }
    }
}

void Controls::posValueChanged()
{
    auto *Spin = qobject_cast<QDoubleSpinBox *>(this->sender());
    auto *Slider = qobject_cast<DoubleSlider *>(this->sender());

    if(currentNode>0) {
        if(Spin ==  findChild<QDoubleSpinBox*>(positionSpinBox[0])
                || Slider == findChild<DoubleSlider*>(positionSlider[0])) {
            readyToSendCanData[currentNode].position =
                    findChild<QDoubleSpinBox*>(positionSpinBox[0])->text().toDouble();
        }
    } else {
        for(int i=0;i<positionSpinBox.length()/3;i++) {//4 parts
            if((i*3) < NODE_NUM && Spin ==  findChild<QDoubleSpinBox*>(positionSpinBox[i*3])
                    || Slider == findChild<DoubleSlider*>(positionSlider[i*3])) {
                readyToSendCanData[i*3].position =
                        findChild<QDoubleSpinBox*>(positionSpinBox[i*3])->text().toDouble();
            }
            else if((i*3)+1 < NODE_NUM && Spin ==  findChild<QDoubleSpinBox*>(positionSpinBox[(i*3)+1])
                    || Slider == findChild<DoubleSlider*>(positionSlider[(i*3)+1])) {
                readyToSendCanData[(i*3)+1].position =
                        findChild<QDoubleSpinBox*>(positionSpinBox[(i*3)+1])->text().toDouble();
            }
            else if((i*3)+2 < NODE_NUM && Spin ==  findChild<QDoubleSpinBox*>(positionSpinBox[(i*3)+2])
                    || Slider == findChild<DoubleSlider*>(positionSlider[(i*3)+2])) {
                readyToSendCanData[(i*3)+2].position =
                        findChild<QDoubleSpinBox*>(positionSpinBox[(i*3)+2])->text().toDouble();
            }
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
void Controls::setPosButtonClicked()
{
    auto *btn = qobject_cast<QPushButton *>(this->sender());

    if(currentNode>0) {
        if(btn == findChild<QPushButton*>(positionSetButton[0])) {
            Package::packOperate(GlobalData::sendId[(currentNode)], readyToSendCanData[(currentNode)].position,
                    PROTOCOL_TYPE_POS);
        }
    } else {
        for(int i=0;i<positionSetButton.length()/3;i++) {//4 parts
            if((i*3) < NODE_NUM && btn == findChild<QPushButton*>(positionSetButton[i*3])) {
                Package::packOperate(GlobalData::sendId[(i*3)], readyToSendCanData[(i*3)].position,
                        PROTOCOL_TYPE_POS);
            }
            else if((i*3)+1 < NODE_NUM && btn == findChild<QPushButton*>(positionSetButton[(i*3)+1])) {
                Package::packOperate(GlobalData::sendId[(i*3)+1], readyToSendCanData[(i*3)+1].position,
                        PROTOCOL_TYPE_POS);
            }
            else if((i*3)+2 < NODE_NUM && btn == findChild<QPushButton*>(positionSetButton[(i*3)+2])) {
                Package::packOperate(GlobalData::sendId[(i*3)+2], readyToSendCanData[(i*3)+2].position,
                        PROTOCOL_TYPE_POS);
            }
        }
    }
}

void Controls::setSpeedButtonClicked()
{
    auto *btn = qobject_cast<QPushButton *>(this->sender());

    if(currentNode>0) {
        if(btn == findChild<QPushButton*>(speedSetButton[0])) {
            Package::packOperate(GlobalData::sendId[currentNode], readyToSendCanData[currentNode].speed,
                    PROTOCOL_TYPE_SPD);
        }
    } else {
        for(int i=0;i<speedSetButton.length()/3;i++) {//4 parts
            if((i*3) < NODE_NUM && btn == findChild<QPushButton*>(speedSetButton[i*3])) {
                Package::packOperate(GlobalData::sendId[(i*3)], readyToSendCanData[(i*3)].speed,
                        PROTOCOL_TYPE_SPD);
            }
            else if((i*3)+1 < NODE_NUM && btn == findChild<QPushButton*>(speedSetButton[(i*3)+1])) {
                Package::packOperate(GlobalData::sendId[(i*3)+1], readyToSendCanData[(i*3)+1].speed,
                        PROTOCOL_TYPE_SPD);
            }
            else if((i*3)+2 < NODE_NUM && btn == findChild<QPushButton*>(speedSetButton[(i*3)+2])) {
                Package::packOperate(GlobalData::sendId[(i*3)+2], readyToSendCanData[(i*3)+2].speed,
                        PROTOCOL_TYPE_SPD);
            }
        }
    }
}

/**
*@projectName   RobotControlSystem
*@brief         速度为点动模式，按下按钮才会动，松开运动停止
*@parameter
*@author        XingZhang.Wu
*@date          20190731
**/
void Controls::setSpeedButtonReleased()
{
    auto *btn = qobject_cast<QPushButton *>(this->sender());

    if(currentNode>0) {
        if(btn == findChild<QPushButton*>(speedSetButton[0])) {
            Package::packOperate(GlobalData::sendId[(currentNode)], 0,
                    PROTOCOL_TYPE_SPD);
        }
    } else {
        for(int i=0;i<speedSetButton.length()/3;i++) {//4 parts
            if((i*3) < NODE_NUM && btn == findChild<QPushButton*>(speedSetButton[i*3])) {
                Package::packOperate(GlobalData::sendId[(i*3)], 0,
                        PROTOCOL_TYPE_SPD);
            }
            else if((i*3)+1 < NODE_NUM && btn == findChild<QPushButton*>(speedSetButton[(i*3)+1])) {
                Package::packOperate(GlobalData::sendId[(i*3)+1], 0,
                        PROTOCOL_TYPE_SPD);
            }
            else if((i*3)+2 < NODE_NUM && btn == findChild<QPushButton*>(speedSetButton[(i*3)+2])) {
                Package::packOperate(GlobalData::sendId[(i*3)+2], 0,
                        PROTOCOL_TYPE_SPD);
            }
        }
    }
}

void Controls::setSpeedSlider(const QList<QString>& l)
{
    speedSlider = l;
}

void Controls::setPositionSlider(const QList<QString>& l)
{
    positionSlider = l;
}

void Controls::setSpeedSpinBox(const QList<QString>& l)
{
    speedSpinBox = l;
}

void Controls::setPositionSpinBox(const QList<QString>& l)
{
    positionSpinBox = l;
}

void Controls::setSpeedSetButton(const QList<QString>& l)
{
    speedSetButton = l;
}

void Controls::setPositionSetButton(const QList<QString>& l)
{
    positionSetButton = l;
}

void Controls::setForwardReversePushButton(const QList<QString>& l)
{
    forwardReversePushButton = l;
}

void Controls::setRelativeSpinBox(const QList<QString>& l)
{
    relativeSpinBox = l;
}
