/*
 * @Author: xingzhang.Wu 
 * @Date: 2019-09-29 10:03:00 
 * @Last Modified by:   xingzhang.Wu 
 * @Last Modified time: 2019-09-29 10:03:00 
 */
#include "jointplot.h"
#include "ui_jointplot.h"

#include "globaldata.h"

#include "debug.h"

#define CURRENT_NODE_NUM (NODE_NUM > 12 ? 12 : NODE_NUM)
#define FIX_NODE_NUM 12

/**
 * @brief Construct a new Joint Plot:: Joint Plot object
 * 显示所有关节的波形，继承于Plots类，
 * 只需要实现initWidget，addDataToUi，
 * setLinesReplot，setLinesPausePlot函数即可
 * @param parent 
 */
JointPlot::JointPlot(Plots *parent) : Plots(parent),
                                      ui(new Ui::JointPlot)
{
    ui->setupUi(this);
    this->setWindowTitle(tr("Joint Plot"));

    initObject();
    initWidget();
}

/**
 * @brief 初始化界面对象
 * 
 */
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

/**
 * @brief 初始化界面控件
 * 
 */
void JointPlot::initWidget()
{
    for (int i = 0; i < FIX_NODE_NUM / 3; i++)
    {
        plotPartWidget(findChild<QCustomPlot *>(posWidget[i]), 0, 360, 3);
        plotPartWidget(findChild<QCustomPlot *>(spdWidget[i]), -3850, 3850, 3);
        plotPartWidget(findChild<QCustomPlot *>(curWidget[i]), 0, 9, 3);
        for (int j = 0; j < 3; j++)
        {
            setPlotPen(findChild<QCustomPlot *>(posWidget[i]), penColor[j], motorNum[3 * i + j], j);
            setPlotPen(findChild<QCustomPlot *>(spdWidget[i]), penColor[j], motorNum[3 * i + j], j);
            setPlotPen(findChild<QCustomPlot *>(curWidget[i]), penColor[j], motorNum[3 * i + j], j);
        }
    }
}

/**
 * @brief 向界面中添加数据
 * 
 * @param key 
 */
void JointPlot::addDataToUi(double key)
{
    for (int i = 0; i < CURRENT_NODE_NUM; i++)
    {
#if 1
        widgetAddData(findChild<QCustomPlot *>(posWidget[i / 3]), i % 3, key, global->currentCanAnalyticalData[i].position);
        widgetAddData(findChild<QCustomPlot *>(spdWidget[i / 3]), i % 3, key, global->currentCanAnalyticalData[i].speed);
        widgetAddData(findChild<QCustomPlot *>(curWidget[i / 3]), i % 3, key, global->currentCanAnalyticalData[i].current);
#else
        widgetAddData(findChild<QCustomPlot *>(posWidget[i / 3]), i % 3, key, qrand() % 360);
        widgetAddData(findChild<QCustomPlot *>(spdWidget[i / 3]), i % 3, key, qrand() % 3580);
        widgetAddData(findChild<QCustomPlot *>(curWidget[i / 3]), i % 3, key, qrand() % 10);
#endif
    }
}

/**
 * @brief 波形界面重绘，保证实时刷新
 * 
 * @param key 
 */
void JointPlot::setLinesReplot(double key)
{
    for (int i = 0; i < FIX_NODE_NUM / 3; i++)
    {
        setLineReplot(findChild<QCustomPlot *>(posWidget[i]), indexs[i * 3], key);
        setLineReplot(findChild<QCustomPlot *>(spdWidget[i]), indexs[i * 3 + 1], key);
        setLineReplot(findChild<QCustomPlot *>(curWidget[i]), indexs[i * 3 + 2], key);
    }
}

/**
 * @brief 波形界面暂停刷新
 * 
 * @param plot 
 */
void JointPlot::setLinesPausePlot(QCustomPlot *plot)
{
    for (int i = 0; i < FIX_NODE_NUM / 3; i++)
    {
        setLinePausePlot(plot, findChild<QCustomPlot *>(posWidget[i]), indexs[i * 3]);
        setLinePausePlot(plot, findChild<QCustomPlot *>(spdWidget[i]), indexs[i * 3 + 1]);
        setLinePausePlot(plot, findChild<QCustomPlot *>(curWidget[i]), indexs[i * 3 + 2]);
    }
}

/**
 * @brief 显示状态信息
 * 
 * @param s 
 */
void JointPlot::showStatus(QString s)
{
    ui->statusLabel->setText(s);
}

/**
 * @brief Destroy the Joint Plot:: Joint Plot object
 * 
 */
JointPlot::~JointPlot()
{
    delete ui;
}
