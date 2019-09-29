/*
 * @Author: xingzhang.Wu 
 * @Date: 2019-09-29 10:04:36 
 * @Last Modified by:   xingzhang.Wu 
 * @Last Modified time: 2019-09-29 10:04:36 
 */
#ifndef RECEIVEFRAMEBOX_H
#define RECEIVEFRAMEBOX_H

#include <QGroupBox>

namespace Ui
{
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
