/*
 * @Author: xingzhang.Wu 
 * @Date: 2019-09-29 10:03:11 
 * @Last Modified by:   xingzhang.Wu 
 * @Last Modified time: 2019-09-29 10:03:11 
 */
#ifndef JOINTSPEEDMEAS_H
#define JOINTSPEEDMEAS_H

#include <QWidget>
#include "plots.h"

namespace Ui
{
class JointSpeedMeas;
}

#define CURRENT_SPEED_INDEX 0x01

class JointSpeedMeas : public Plots
{
    Q_OBJECT

public:
    explicit JointSpeedMeas(Plots *parent = 0);
    ~JointSpeedMeas();

private:
    void initWidget();
    void addDataToUi(double key);
    void setLinesReplot(double key);
    void setLinesPausePlot(QCustomPlot *plot);

private:
    Ui::JointSpeedMeas *ui;
};

#endif // JOINTSPEEDMEAS_H
