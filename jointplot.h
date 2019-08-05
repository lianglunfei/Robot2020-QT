#ifndef JOINTPLOT_H
#define JOINTPLOT_H

#include <QWidget>
#include "plots.h"

namespace Ui {
class JointPlot;
}

#define POS1_INDEX 0x01
#define SPEED1_INDEX 0x02
#define CURRENT1_INDEX 0x04
#define POS2_INDEX 0x08
#define SPEED2_INDEX 0x10
#define CURRENT2_INDEX 0x20
#define POS3_INDEX 0x40
#define SPEED3_INDEX 0x80
#define CURRENT3_INDEX 0x100
#define POS4_INDEX 0x200
#define SPEED4_INDEX 0x400
#define CURRENT4_INDEX 0x800

class JointPlot : public Plots
{
    Q_OBJECT

public:
    explicit JointPlot();
    ~JointPlot();

private:
    void initWidget();
    void addDataToUi(double key);
    void setLinesReplot(double key);
    void setLinesPausePlot(QCustomPlot *plot);

private:
    Ui::JointPlot *ui;
};

#endif // JOINTPLOT_H
