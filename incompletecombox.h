/*
 * @Author: xingzhang.Wu 
 * @Date: 2019-09-29 10:02:47 
 * @Last Modified by:   xingzhang.Wu 
 * @Last Modified time: 2019-09-29 10:02:47 
 */
#ifndef INCOMPLETECOMBOX_H
#define INCOMPLETECOMBOX_H

#include <QComboBox>
#include <QWheelEvent>

class IncompleteCombox : public QComboBox
{
    Q_OBJECT

public:
    IncompleteCombox();

protected:
    void wheelEvent(QWheelEvent *) {}
    void mousePressEvent(QMouseEvent *) {}
    void mouseDoubleClickEvent(QMouseEvent *)
    {
        showPopup();
    }
};

#endif // INCOMPLETECOMBOX_H
