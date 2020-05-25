#ifndef MYCUSTOMSLIDER_H
#define MYCUSTOMSLIDER_H

#include <QSlider>
#include <QLabel>
#include <QMouseEvent>
#include <QPalette>
#include <QDebug>

class MyCustomSlider : public QSlider
{
    Q_OBJECT
public:
    MyCustomSlider(QWidget *parent=nullptr)
    {
        connect(this, SIGNAL(valueChanged(int)),
            this, SLOT(notifyValueChanged(int)));
        connect(this, SIGNAL(sliderMoved(int)),
            this, SLOT(notifySliderMoved(int)));

    }



signals:
    void doubleValueChanged(double value);
    void doubleSliderMoved(double value);

public slots:
    void notifyValueChanged(int value)
    {
        double doubleValue = value / 10.0;
            emit doubleValueChanged(doubleValue);
    }

    void notifySliderMoved(int value)
    {
        double doubleValue = value / 10.0;

            emit doubleSliderMoved(doubleValue);

    }

    void doubleSetValue(double doubleValue)
    {
//        qDebug()<<doubleValue;
        int value = doubleValue * 10;
        emit setValue(value);
    }

};


#endif // MYCUSTOMSLIDER_H
