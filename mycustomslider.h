#ifndef MYCUSTOMSLIDER_H
#define MYCUSTOMSLIDER_H

#include <QSlider>
#include <QLabel>
#include <QMouseEvent>
#include <QPalette>

class MyCustomSlider : public QSlider
{
    Q_OBJECT
public:
    MyCustomSlider(QWidget *parent=nullptr)
    {
        operateLock = false;
        connect(this, SIGNAL(valueChanged(int)),
            this, SLOT(notifyValueChanged(int)));
//        m_displayLabel=new QLabel(this);
//        m_displayLabel->setFixedSize(QSize(20,20));

//        m_displayLabel->setAlignment(Qt::AlignCenter);
//        m_displayLabel->setText(QString("↑"));
//        m_displayLabel->setVisible(true);
//        m_displayLabel->move(0,3);
    }

    void openOperateLock(){
        operateLock = false;
    }
    void closeOperateLock(){
        operateLock = true;
    }

//protected:
//    virtual void mousePressEvent(QMouseEvent *event){
//        openOperateLock();
//        QSlider::mousePressEvent(event);
//    }

//    virtual void mouseReleaseEvent(QMouseEvent *event){
//        QSlider::mouseReleaseEvent(event);
//    }

//    virtual void mouseMoveEvent(QMouseEvent *event){
//        closeOperateLock();
//        QSlider::mouseMoveEvent(event);
//    }
private:
//    QLabel*	m_displayLabel;
    bool operateLock;

signals:
    void doubleValueChanged(double value);

public slots:
    void notifyValueChanged(int value)
    {
        double doubleValue = value / 10.0;
//        if(!operateLock)
            emit doubleValueChanged(doubleValue);
//        m_displayLabel->move((this->width()-m_displayLabel->width())*this->value()/(this->maximum()-this->minimum()),4);
    }

    void doubleSetValue(double doubleValue)
    {
        int value = doubleValue * 10;
        setValue(value);
    }

};


#endif // MYCUSTOMSLIDER_H
