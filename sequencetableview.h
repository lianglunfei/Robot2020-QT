/*
 * @Descripttion: 
 * @version: 
 * @Author: Qingmao Wei
 * @Date: 2020-01-14 15:19:54
 * @LastEditors  : Qingmao Wei
 * @LastEditTime : 2020-01-15 14:22:23
 */
#ifndef SEQUENCETABLEVIEW_H
#define SEQUENCETABLEVIEW_H

#include "sequence.h"
#include <QTableView>
#include <QStandardItemModel>

class SequenceTableView : public QTableView
{
    Q_OBJECT
public:
    SequenceTableView(QWidget *parent = 0);
    ~SequenceTableView();
    void setActionModels(QMap<QString,QString>);
    
private:
    QStandardItemModel *model;
    QStringList headerData;
    QStringList valueList;
    QStringList actionList;
    SequenceExcuteWorker *seqWorker;
    QMap<QString,QString> actionModels;
    int interValue;
    int interPeriod;

    void headerDataInit();
    void valueListInit();
    void modelInit();
    void tableViewInit();

    void addTableviewRowWidget(int mode, int row, bool checkState, bool complete);
    void getModelRowValue(double *value, int row, int len);
    void updateTableRowProperty(int row, int property);
    void updateTablePropertyAfterLine(int row, int offset);
    void valueListUpdate(int row);
    void addTableviewRow(int mode, int row, bool hasWidget);
    void addModelItemData(int row);
    int execOneAction(QString);

private slots:
    void tableClickButton();
public slots:    
    void setInterValue(int);
    void setInterPeriod(int);
signals:
    void setActionList(const QStringList &);
    
};

#endif // SEQUENCETABLEVIEW_H
