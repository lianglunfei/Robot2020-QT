/*
 * @Author: xingzhang.Wu 
 * @Date: 2019-09-29 10:02:04 
 * @Last Modified by:   xingzhang.Wu 
 * @Last Modified time: 2019-09-29 10:02:04 
 */
#ifndef DOUBLESLIDER_H
#define DOUBLESLIDER_H

#include <QSlider>
#include <QLabel>
#include <QMouseEvent>
#include <QPalette>

class DoubleSlider : public QSlider
{
    Q_OBJECT

public:
    DoubleSlider(QWidget *parent = 0) : QSlider(parent)
    {
        connect(this, SIGNAL(valueChanged(int)),
                this, SLOT(notifyValueChanged(int)));
//        m_displayLabel=new QLabel(this);
//        m_displayLabel->setFixedSize(QSize(20,20));
//        //设置游标背景为白色
//        m_displayLabel->setAutoFillBackground(true);
//        QPalette palette;
//        palette.setColor(QPalette::Background, Qt::white);
//        m_displayLabel->setPalette(palette);

//        m_displayLabel->setAlignment(Qt::AlignCenter);

//        m_displayLabel->setVisible(true);
//        m_displayLabel->move(0,3);
    }
//protected:
//    virtual void mousePressEvent(QMouseEvent *event){
//        if(!m_displayLabel->isVisible())
//        {
//            m_displayLabel->setVisible(true);
//            m_displayLabel->setText(QString::number(this->value()));
//        }
//        QSlider::mousePressEvent(event);
//    }

//    virtual void mouseReleaseEvent(QMouseEvent *event){
//        if(m_displayLabel->isVisible())
//        {
//            m_displayLabel->setVisible(false);
//        }
//        QSlider::mouseReleaseEvent(event);
//    }

//    virtual void mouseMoveEvent(QMouseEvent *event){
//        m_displayLabel->setText(QString::number(this->value()));
//        m_displayLabel->move((this->width()-m_displayLabel->width())*this->value()/(this->maximum()-this->minimum()),3);
//        QSlider::mouseMoveEvent(event);
//    }

//private:
//    QLabel*	m_displayLabel;
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
