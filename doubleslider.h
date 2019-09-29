/*
 * @Author: xingzhang.Wu 
 * @Date: 2019-09-29 10:02:04 
 * @Last Modified by:   xingzhang.Wu 
 * @Last Modified time: 2019-09-29 10:02:04 
 */
#ifndef DOUBLESLIDER_H
#define DOUBLESLIDER_H

#include <QSlider>

class DoubleSlider : public QSlider
{
    Q_OBJECT

public:
    DoubleSlider(QWidget *parent = 0) : QSlider(parent)
    {
        connect(this, SIGNAL(valueChanged(int)),
                this, SLOT(notifyValueChanged(int)));
    }

signals:
    void doubleValueChanged(double value);

public slots:
    void notifyValueChanged(int value)
    {
        double doubleValue = value / 10.0;
        emit doubleValueChanged(doubleValue);
    }

    void doubleSetValue(double doubleValue)
    {
        int value = doubleValue * 10;
        setValue(value);
    }
};

#endif // DOUBLESLIDER_H
