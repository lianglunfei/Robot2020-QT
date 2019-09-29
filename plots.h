/*
 * @Author: xingzhang.Wu 
 * @Date: 2019-09-29 10:04:00 
 * @Last Modified by:   xingzhang.Wu 
 * @Last Modified time: 2019-09-29 10:04:00 
 */
#ifndef PLOTS_H
#define PLOTS_H

#include <QWidget>

class QCustomPlot;
class QCPAbstractPlottable;

class Plots : public QWidget
{
    Q_OBJECT
public:
    explicit Plots(QWidget *parent = 0);
    virtual ~Plots();

    virtual void addDataToUi(double key);
    virtual void setLinesReplot(double key);
    virtual void setLinesPausePlot(QCustomPlot *plot);
    virtual void showStatus(QString s);

    void plotPartWidget(QCustomPlot *part, int rangeMin, int rangeMax, int num);
    void setPlotPen(QCustomPlot *part, const QPen p, const QString name, int index);
    void setLineReplot(QCustomPlot *part, int index, double key);
    void setLinePausePlot(QCustomPlot *senderPart, QCustomPlot *currentPart, int index);
    void widgetAddData(QCustomPlot *plot, int num, double key, double value);

private:
    QTimer *plotDataTimer;

private slots:
    void realtimeDataPlot();
    void mouseDoubleClick(QMouseEvent *);
    void graphClicked(QCPAbstractPlottable *plottable, int dataIndex);

protected:
    int pauseFlag = 0;
};

#endif // PLOTS_H
