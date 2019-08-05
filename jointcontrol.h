#ifndef JOINTCONTROL_H
#define JOINTCONTROL_H

#include <QDialog>

#include "globaldata.h"

#include "controls.h"

namespace Ui {
class JointControl;
}

class JointControl : public Controls
{
    Q_OBJECT

public:
    explicit JointControl();
    ~JointControl();

private:
    Ui::JointControl *ui;

    void initObject();
};

#endif // JOINTCONTROL_H
