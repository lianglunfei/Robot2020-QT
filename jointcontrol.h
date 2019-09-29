/*
 * @Author: xingzhang.Wu 
 * @Date: 2019-09-29 10:02:56 
 * @Last Modified by:   xingzhang.Wu 
 * @Last Modified time: 2019-09-29 10:02:56 
 */
#ifndef JOINTCONTROL_H
#define JOINTCONTROL_H

#include <QDialog>

#include "globaldata.h"

#include "controls.h"

namespace Ui
{
class JointControl;
}

class JointControl : public Controls
{
    Q_OBJECT

public:
    explicit JointControl();
    ~JointControl() override;

private slots:
    void on_initDriverPushButton_clicked();

    void on_emergencyStopPushButton_clicked();

    void on_caliPushButton_clicked();

private:
    Ui::JointControl *ui;

    void initObject();

    void keyPressEvent(QKeyEvent *e) override;
};

#endif // JOINTCONTROL_H
