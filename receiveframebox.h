#ifndef RECEIVEFRAMEBOX_H
#define RECEIVEFRAMEBOX_H

#include <QGroupBox>

namespace Ui {
class ReceiveFrameBox;
}

class ReceiveFrameBox : public QGroupBox
{
    Q_OBJECT

public:
    explicit ReceiveFrameBox(QWidget *parent = 0);
    ~ReceiveFrameBox();

private:
    Ui::ReceiveFrameBox *ui;

    void initEvent();

private slots:
    void update();
};

#endif // RECEIVEFRAMEBOX_H
