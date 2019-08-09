#include "jointspeedmeas.h"
#include "ui_jointspeedmeas.h"

#include "globaldata.h"

#include "debug.h"

JointSpeedMeas::JointSpeedMeas() :
    ui(new Ui::JointSpeedMeas)
{
    ui->setupUi(this);
    this->setWindowTitle(tr("Joint Speed"));

    initWidget();
}

void JointSpeedMeas::initWidget()
{
    plotPartWidget(ui->rpmWidget, -50, 50, 1);
    setPlotPen(ui->rpmWidget, QPen(QColor(80, 160, 20)), "当前速度", 0);
}

void JointSpeedMeas::addDataToUi(double key)
{
    int index=ui->comboBox->currentIndex()>NODE_NUM-1?NODE_NUM-1:ui->comboBox->currentIndex();
    widgetAddData(ui->rpmWidget, 0, key, GlobalData::currentCanAnalyticalData[index].speed);
    ui->speedLcdNumber->display(GlobalData::currentCanAnalyticalData[index].speed);
}

void JointSpeedMeas::setLinesReplot(double key)
{
    setLineReplot(ui->rpmWidget, CURRENT_SPEED_INDEX, key);
}

void JointSpeedMeas::setLinesPausePlot(QCustomPlot *plot)
{
    setLinePausePlot(plot, ui->rpmWidget, CURRENT_SPEED_INDEX);
}

JointSpeedMeas::~JointSpeedMeas()
{
    delete ui;
}
