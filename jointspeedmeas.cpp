/*
 * @Author: xingzhang.Wu 
 * @Date: 2019-09-29 10:03:07 
 * @Last Modified by:   xingzhang.Wu 
 * @Last Modified time: 2019-09-29 10:03:07 
 */
#include "jointspeedmeas.h"
#include "ui_jointspeedmeas.h"

#include "globaldata.h"

#include "debug.h"

/**
 * @brief Construct a new Joint Speed Meas:: Joint Speed Meas object
 * 
 * @param parent 
 */
JointSpeedMeas::JointSpeedMeas(Plots *parent) : Plots(parent),
                                                ui(new Ui::JointSpeedMeas)
{
    ui->setupUi(this);
    this->setWindowTitle(tr("Joint Speed"));

    initWidget();
}

/**
 * @brief 初始化界面控件
 * 
 */
void JointSpeedMeas::initWidget()
{
    plotPartWidget(ui->rpmWidget, -50, 50, 1);
    setPlotPen(ui->rpmWidget, QPen(QColor(80, 160, 20)), "当前速度", 0);
}

/**
 * @brief 虚函数处理，向UI中填充数据
 * 
 * @param key 
 */
void JointSpeedMeas::addDataToUi(double key)
{
    int index = ui->comboBox->currentIndex() > NODE_NUM - 1 ? NODE_NUM - 1 : ui->comboBox->currentIndex();
#if 1
    widgetAddData(ui->rpmWidget, 0, key, global->currentCanAnalyticalData[index].speed);
    ui->speedLcdNumber->display(global->currentCanAnalyticalData[index].speed);
#else
    int tst = qrand() % 50;
    (void)index;
    widgetAddData(ui->rpmWidget, 0, key, tst);
    ui->speedLcdNumber->display(tst);
#endif
}

/**
 * @brief 波形界面重绘，保证实时刷新
 * 
 * @param key 
 */
void JointSpeedMeas::setLinesReplot(double key)
{
    setLineReplot(ui->rpmWidget, CURRENT_SPEED_INDEX, key);
}

/**
 * @brief 波形界面暂停更新绘制
 * 
 * @param plot 
 */
void JointSpeedMeas::setLinesPausePlot(QCustomPlot *plot)
{
    setLinePausePlot(plot, ui->rpmWidget, CURRENT_SPEED_INDEX);
}

/**
 * @brief Destroy the Joint Speed Meas:: Joint Speed Meas object
 * 
 */
JointSpeedMeas::~JointSpeedMeas()
{
    delete ui;
}
