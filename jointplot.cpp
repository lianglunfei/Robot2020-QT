#include "jointplot.h"
#include "ui_jointplot.h"

#include "globaldata.h"

#include "debug.h"

#define CURRENT_NODE_NUM (NODE_NUM>12?12:NODE_NUM)
#define FIX_NODE_NUM 12

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

    initObject();
    initWidget();
}

void JointPlot::initObject()
{
    posWidget = QList<QString>({"positionWidget1", "positionWidget2",
                                "positionWidget3", "positionWidget4"});

    spdWidget = QList<QString>({"speedWidget1", "speedWidget2",
                                "speedWidget3", "speedWidget4"});

    curWidget = QList<QString>({"currentWidget1", "currentWidget2",
                                "currentWidget3", "currentWidget4"});

    penColor = QList<QPen>({QColor(40, 110, 255), QColor(255, 110, 40), QColor(20, 110, 40)});

    motorNum = QList<QString>({"Motor1", "Motor2", "Motor3",
                              "Motor4", "Motor5", "Motor6",
                              "Motor7", "Motor8", "Motor9",
                              "Motor10", "Motor11", "Motor12"});

    indexs = QList<int>({POS1_INDEX, SPEED1_INDEX, CURRENT1_INDEX,
                        POS2_INDEX, SPEED2_INDEX, CURRENT2_INDEX,
                        POS3_INDEX, SPEED3_INDEX, CURRENT3_INDEX,
                        POS4_INDEX, SPEED4_INDEX, CURRENT4_INDEX});
}

void JointPlot::initWidget()
{
    for(int i=0;i<FIX_NODE_NUM/3;i++)  {
        plotPartWidget(findChild<QCustomPlot*>(posWidget[i]), 0, 360, 3);
        plotPartWidget(findChild<QCustomPlot*>(spdWidget[i]), -3850, 3850, 3);
        plotPartWidget(findChild<QCustomPlot*>(curWidget[i]), 0, 9, 3);
        for(int j=0;j<3;j++) {
            setPlotPen(findChild<QCustomPlot*>(posWidget[i]), penColor[j], motorNum[3*i+j], j);
            setPlotPen(findChild<QCustomPlot*>(spdWidget[i]), penColor[j], motorNum[3*i+j], j);
            setPlotPen(findChild<QCustomPlot*>(curWidget[i]), penColor[j], motorNum[3*i+j], j);
        }
    }
}

void JointPlot::addDataToUi(double key)
{
    for(int i=0;i<CURRENT_NODE_NUM;i++) {
        widgetAddData(findChild<QCustomPlot*>(posWidget[i/3]), i%3, key, global->currentCanAnalyticalData[i].position);
        widgetAddData(findChild<QCustomPlot*>(spdWidget[i/3]), i%3, key, global->currentCanAnalyticalData[i].speed);
        widgetAddData(findChild<QCustomPlot*>(curWidget[i/3]), i%3, key, global->currentCanAnalyticalData[i].current);
    }
}

void JointPlot::setLinesReplot(double key)
{
    for(int i=0;i<FIX_NODE_NUM/3;i++) {
        setLineReplot(findChild<QCustomPlot*>(posWidget[i]), indexs[i*3], key);
        setLineReplot(findChild<QCustomPlot*>(spdWidget[i]), indexs[i*3+1], key);
        setLineReplot(findChild<QCustomPlot*>(curWidget[i]), indexs[i*3+2], key);
    }
}

void JointPlot::setLinesPausePlot(QCustomPlot *plot)
{
    for(int i=0;i<FIX_NODE_NUM/3;i++) {
        setLinePausePlot(plot, findChild<QCustomPlot*>(posWidget[i]), indexs[i*3]);
        setLinePausePlot(plot, findChild<QCustomPlot*>(spdWidget[i]), indexs[i*3+1]);
        setLinePausePlot(plot, findChild<QCustomPlot*>(curWidget[i]), indexs[i*3+2]);
    }
}

void JointPlot::showStatus(QString s)
{
    ui->statusLabel->setText(s);
}

JointPlot::~JointPlot()
{
    delete ui;
}
