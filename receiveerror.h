#ifndef RECEIVEERROR_H
#define RECEIVEERROR_H

#include <QDialog>
#include <QTimer>
#include <QElapsedTimer>

#include "globaldata.h"

class QLabel;
class QVBoxLayout;
class QHBoxLayout;

namespace Ui {
class ReceiveError;
}

class ReceiveError : public QDialog
{
    Q_OBJECT

public:
    explicit ReceiveError(QWidget *parent = nullptr);
    ~ReceiveError();

private:
    Ui::ReceiveError *ui;
    int countLostAll=0;
    int countLostId[NODE_NUM]={0};
    int countErrorId[NODE_NUM]={0};
    int lastRunningJoint[NODE_NUM]={0};
    int lastStatusId[NODE_NUM]={0};
    QTimer *time=nullptr;
    QLabel *nodeNum[NODE_NUM+1];
    QLabel *nodeName[NODE_NUM+1];
    QLabel *nodeMaxTime[NODE_NUM+1];
    QLabel *nodeStatus[NODE_NUM+1];
    int maxTime[NODE_NUM]={0};
    QElapsedTimer start[NODE_NUM];
    QVBoxLayout *vLayout;
    QHBoxLayout *hLayout1;
    QHBoxLayout *hLayout2;
    QHBoxLayout *hLayout3;
    QHBoxLayout *hLayout4;

    void init();
    void errorHandle(int i);

private slots:
    void update();
    void on_pushButton_clicked();
};

#endif // RECEIVEERROR_H
