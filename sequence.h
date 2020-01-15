/*
 * @Descripttion: 
 * @version: 
 * @Author: xingzhang.Wu
 * @Date: 2020-01-13 10:18:51
 * @LastEditors  : Qingmao Wei
 * @LastEditTime : 2020-01-15 10:08:32
 */
#ifndef SEQUENCE_H
#define SEQUENCE_H

#include <QObject>
#include <QTableView>
#include <QStandardItemModel>

class SequenceExcuteWorker : public QObject
{
    Q_OBJECT
private:
    SequenceExcuteWorker();
    ~SequenceExcuteWorker();
    SequenceExcuteWorker(const SequenceExcuteWorker&);
	SequenceExcuteWorker& operator=(const SequenceExcuteWorker&);

    QStringList headerData;
    QStringList valueList;
    QTimer *taskTimer;
    QThread *taskThread;
    QStandardItemModel *seqModel;
    int execRunOrPauseFlag;
    bool cycleFlag;
    int interValue;
    int interPeriod;

    void headerDataInit();
    int eventInit();
    int modelInit();
    int runFunc(int row);
    void setListBoundaryValue(int &up, int &down);
    void getModelRowValue(double *value, int row, int len);

public:
    static SequenceExcuteWorker& getInstance() {
        static SequenceExcuteWorker instance;
        return instance;
    }
    
    void execStop();
    int execPause();
    int seqExec(bool cycle, int value, int period);
    int reverseSeqExec(bool cycle, int value, int period);
    int importCSV(QString file_name);
  
private slots:
    void execSeqEvent();

signals:
    void stopThread();
    void execStatus(QString s);
};



#endif