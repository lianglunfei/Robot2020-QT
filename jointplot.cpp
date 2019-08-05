#include "jointplot.h"
#include "ui_jointplot.h"

#include "globaldata.h"

#include "debug.h"

/**
*@projectName   RobotControlSystem
*@brief         显示所有关节的波形，继承于Plots类，
*               只需要实现initWidget，addDataToUi，
*               setLinesReplot，setLinesPausePlot函数即可
*@parameter
*@author        XingZhang.Wu
*@date          20190724
**/
JointPlot::JointPlot() :
    ui(new Ui::JointPlot)
{
    ui->setupUi(this);
    this->setWindowTitle(tr("Joint Plot"));

    initWidget();
}

void JointPlot::initWidget()
{
    plotPartWidget(ui->positionWidget1, 0, 360, 3);
    plotPartWidget(ui->positionWidget2, 0, 360, 3);
    plotPartWidget(ui->positionWidget3, 0, 360, 3);
    plotPartWidget(ui->positionWidget4, 0, 360, 3);

    plotPartWidget(ui->speedWidget1, -3850, 3850, 3);
    plotPartWidget(ui->speedWidget2, -3850, 3850, 3);
    plotPartWidget(ui->speedWidget3, -3850, 3850, 3);
    plotPartWidget(ui->speedWidget4, -3850, 3850, 3);

    plotPartWidget(ui->currentWidget1, 0, 9, 3);
    plotPartWidget(ui->currentWidget2, 0, 9, 3);
    plotPartWidget(ui->currentWidget3, 0, 9, 3);
    plotPartWidget(ui->currentWidget4, 0, 9, 3);

    setPlotPen(ui->positionWidget1, QPen(QColor(40, 110, 255)), "Motor1", 0);
    setPlotPen(ui->positionWidget1, QPen(QColor(255, 110, 40)), "Motor2", 1);
    setPlotPen(ui->positionWidget1, QPen(QColor(20, 110, 40)), "Motor3", 2);

    setPlotPen(ui->speedWidget1, QPen(QColor(40, 110, 255)), "Motor1", 0);
    setPlotPen(ui->speedWidget1, QPen(QColor(255, 110, 40)), "Motor2", 1);
    setPlotPen(ui->speedWidget1, QPen(QColor(20, 110, 40)), "Motor3", 2);

    setPlotPen(ui->currentWidget1, QPen(QColor(40, 110, 255)), "Motor1", 0);
    setPlotPen(ui->currentWidget1, QPen(QColor(255, 110, 40)), "Motor2", 1);
    setPlotPen(ui->currentWidget1, QPen(QColor(20, 110, 40)), "Motor3", 2);

    setPlotPen(ui->positionWidget2, QPen(QColor(40, 110, 255)), "Motor4", 0);
    setPlotPen(ui->positionWidget2, QPen(QColor(255, 110, 40)), "Motor5", 1);
    setPlotPen(ui->positionWidget2, QPen(QColor(20, 110, 40)), "Motor6", 2);

    setPlotPen(ui->speedWidget2, QPen(QColor(40, 110, 255)), "Motor4", 0);
    setPlotPen(ui->speedWidget2, QPen(QColor(255, 110, 40)), "Motor5", 1);
    setPlotPen(ui->speedWidget2, QPen(QColor(20, 110, 40)), "Motor6", 2);

    setPlotPen(ui->currentWidget2, QPen(QColor(40, 110, 255)), "Motor4", 0);
    setPlotPen(ui->currentWidget2, QPen(QColor(255, 110, 40)), "Motor5", 1);
    setPlotPen(ui->currentWidget2, QPen(QColor(20, 110, 40)), "Motor6", 2);

    setPlotPen(ui->positionWidget3, QPen(QColor(40, 110, 255)), "Motor7", 0);
    setPlotPen(ui->positionWidget3, QPen(QColor(255, 110, 40)), "Motor8", 1);
    setPlotPen(ui->positionWidget3, QPen(QColor(20, 110, 40)), "Motor9", 2);

    setPlotPen(ui->speedWidget3, QPen(QColor(40, 110, 255)), "Motor7", 0);
    setPlotPen(ui->speedWidget3, QPen(QColor(255, 110, 40)), "Motor8", 1);
    setPlotPen(ui->speedWidget3, QPen(QColor(20, 110, 40)), "Motor9", 2);

    setPlotPen(ui->currentWidget3, QPen(QColor(40, 110, 255)), "Motor7", 0);
    setPlotPen(ui->currentWidget3, QPen(QColor(255, 110, 40)), "Motor8", 1);
    setPlotPen(ui->currentWidget3, QPen(QColor(20, 110, 40)), "Motor9", 2);

    setPlotPen(ui->positionWidget4, QPen(QColor(40, 110, 255)), "Motor10", 0);
    setPlotPen(ui->positionWidget4, QPen(QColor(255, 110, 40)), "Motor11", 1);
    setPlotPen(ui->positionWidget4, QPen(QColor(20, 110, 40)), "Motor12", 2);

    setPlotPen(ui->speedWidget4, QPen(QColor(40, 110, 255)), "Motor10", 0);
    setPlotPen(ui->speedWidget4, QPen(QColor(255, 110, 40)), "Motor11", 1);
    setPlotPen(ui->speedWidget4, QPen(QColor(20, 110, 40)), "Motor12", 2);

    setPlotPen(ui->currentWidget4, QPen(QColor(40, 110, 255)), "Motor10", 0);
    setPlotPen(ui->currentWidget4, QPen(QColor(255, 110, 40)), "Motor11", 1);
    setPlotPen(ui->currentWidget4, QPen(QColor(20, 110, 40)), "Motor12", 2);
}

void JointPlot::addDataToUi(double key)
{
    widgetAddData(ui->positionWidget1, 0, key, GlobalData::currentCanAnalyticalData[0].position);
    widgetAddData(ui->positionWidget1, 1, key, GlobalData::currentCanAnalyticalData[1].position);
    widgetAddData(ui->positionWidget1, 2, key, GlobalData::currentCanAnalyticalData[2].position);

    widgetAddData(ui->positionWidget2, 0, key, GlobalData::currentCanAnalyticalData[3].position);
    widgetAddData(ui->positionWidget2, 1, key, GlobalData::currentCanAnalyticalData[4].position);
    widgetAddData(ui->positionWidget2, 2, key, GlobalData::currentCanAnalyticalData[5].position);

    widgetAddData(ui->positionWidget3, 0, key, GlobalData::currentCanAnalyticalData[6].position);
    widgetAddData(ui->positionWidget3, 1, key, GlobalData::currentCanAnalyticalData[7].position);
    widgetAddData(ui->positionWidget3, 2, key, GlobalData::currentCanAnalyticalData[8].position);

    widgetAddData(ui->positionWidget4, 0, key, GlobalData::currentCanAnalyticalData[9].position);
    widgetAddData(ui->positionWidget4, 1, key, GlobalData::currentCanAnalyticalData[10].position);
    widgetAddData(ui->positionWidget4, 2, key, GlobalData::currentCanAnalyticalData[11].position);

    //speed
    widgetAddData(ui->speedWidget1, 0, key, GlobalData::currentCanAnalyticalData[0].speed);
    widgetAddData(ui->speedWidget1, 1, key, GlobalData::currentCanAnalyticalData[1].speed);
    widgetAddData(ui->speedWidget1, 2, key, GlobalData::currentCanAnalyticalData[2].speed);

    widgetAddData(ui->speedWidget2, 0, key, GlobalData::currentCanAnalyticalData[3].speed);
    widgetAddData(ui->speedWidget2, 1, key, GlobalData::currentCanAnalyticalData[4].speed);
    widgetAddData(ui->speedWidget2, 2, key, GlobalData::currentCanAnalyticalData[5].speed);

    widgetAddData(ui->speedWidget3, 0, key, GlobalData::currentCanAnalyticalData[6].speed);
    widgetAddData(ui->speedWidget3, 1, key, GlobalData::currentCanAnalyticalData[7].speed);
    widgetAddData(ui->speedWidget3, 2, key, GlobalData::currentCanAnalyticalData[8].speed);

    widgetAddData(ui->speedWidget4, 0, key, GlobalData::currentCanAnalyticalData[9].speed);
    widgetAddData(ui->speedWidget4, 1, key, GlobalData::currentCanAnalyticalData[10].speed);
    widgetAddData(ui->speedWidget4, 2, key, GlobalData::currentCanAnalyticalData[11].speed);

    //current
    widgetAddData(ui->currentWidget1, 0, key, GlobalData::currentCanAnalyticalData[0].current);
    widgetAddData(ui->currentWidget1, 1, key, GlobalData::currentCanAnalyticalData[1].current);
    widgetAddData(ui->currentWidget1, 2, key, GlobalData::currentCanAnalyticalData[2].current);

    widgetAddData(ui->currentWidget2, 0, key, GlobalData::currentCanAnalyticalData[3].current);
    widgetAddData(ui->currentWidget2, 1, key, GlobalData::currentCanAnalyticalData[4].current);
    widgetAddData(ui->currentWidget2, 2, key, GlobalData::currentCanAnalyticalData[5].current);

    widgetAddData(ui->currentWidget3, 0, key, GlobalData::currentCanAnalyticalData[6].current);
    widgetAddData(ui->currentWidget3, 1, key, GlobalData::currentCanAnalyticalData[7].current);
    widgetAddData(ui->currentWidget3, 2, key, GlobalData::currentCanAnalyticalData[8].current);

    widgetAddData(ui->currentWidget4, 0, key, GlobalData::currentCanAnalyticalData[9].current);
    widgetAddData(ui->currentWidget4, 1, key, GlobalData::currentCanAnalyticalData[10].current);
    widgetAddData(ui->currentWidget4, 2, key, GlobalData::currentCanAnalyticalData[11].current);
}

void JointPlot::setLinesReplot(double key)
{
    setLineReplot(ui->positionWidget1, POS1_INDEX, key);
    setLineReplot(ui->speedWidget1, SPEED1_INDEX, key);
    setLineReplot(ui->currentWidget1, CURRENT1_INDEX, key);

    setLineReplot(ui->positionWidget2, POS2_INDEX, key);
    setLineReplot(ui->speedWidget2, SPEED2_INDEX, key);
    setLineReplot(ui->currentWidget2, CURRENT2_INDEX, key);

    setLineReplot(ui->positionWidget3, POS3_INDEX, key);
    setLineReplot(ui->speedWidget3, SPEED3_INDEX, key);
    setLineReplot(ui->currentWidget3, CURRENT3_INDEX, key);

    setLineReplot(ui->positionWidget4, POS4_INDEX, key);
    setLineReplot(ui->speedWidget4, SPEED4_INDEX, key);
    setLineReplot(ui->currentWidget4, CURRENT4_INDEX, key);
}

void JointPlot::setLinesPausePlot(QCustomPlot *plot)
{
    setLinePausePlot(plot, ui->positionWidget1, POS1_INDEX);
    setLinePausePlot(plot, ui->speedWidget1, SPEED1_INDEX);
    setLinePausePlot(plot, ui->currentWidget1, CURRENT1_INDEX);

    setLinePausePlot(plot, ui->positionWidget2, POS2_INDEX);
    setLinePausePlot(plot, ui->speedWidget2, SPEED2_INDEX);
    setLinePausePlot(plot, ui->currentWidget2, CURRENT2_INDEX);

    setLinePausePlot(plot, ui->positionWidget3, POS3_INDEX);
    setLinePausePlot(plot, ui->speedWidget3, SPEED3_INDEX);
    setLinePausePlot(plot, ui->currentWidget3, CURRENT3_INDEX);

    setLinePausePlot(plot, ui->positionWidget4, POS4_INDEX);
    setLinePausePlot(plot, ui->speedWidget4, SPEED4_INDEX);
    setLinePausePlot(plot, ui->currentWidget4, CURRENT4_INDEX);
}

JointPlot::~JointPlot()
{
    delete ui;
}
