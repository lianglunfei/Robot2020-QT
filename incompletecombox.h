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
