#include "receiveerror.h"
#include "ui_receiveerror.h"

#include "qdebug.h"
#include <QDateTime>

ReceiveError::ReceiveError(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ReceiveError)
{
    ui->setupUi(this);
    this->setWindowTitle(tr("Receive Error"));
    init();
}

void ReceiveError::init()
{
    for(int i=0;i<NODE_NUM;i++) {
        ui->comboBox->addItem(QString::number(i+1));
    }
    ui->comboBox->addItem(tr("all"));
    ui->comboBox->setCurrentIndex(NODE_NUM);

    vLayout = new QVBoxLayout();
    hLayout1 = new QHBoxLayout();
    for(int i=0;i<NODE_NUM;i++) {
        nodeName[i] = new QLabel();
        nodeName[i]->setText("J"+QString::number(i+1));
    }
    nodeName[NODE_NUM] = new QLabel();
    nodeName[NODE_NUM]->setText("");

    hLayout2 = new QHBoxLayout();
    for(int i=0;i<NODE_NUM;i++) {
        nodeNum[i] = new QLabel();
        nodeNum[i]->setNum(0);
    }
    nodeNum[NODE_NUM] = new QLabel();
    nodeNum[NODE_NUM]->setText("丢失数");

    hLayout3 = new QHBoxLayout();
    for(int i=0;i<NODE_NUM;i++) {
        nodeMaxTime[i] = new QLabel();
        nodeMaxTime[i]->setNum(0);
    }
    nodeMaxTime[NODE_NUM] = new QLabel();
    nodeMaxTime[NODE_NUM]->setText("最大时");

    hLayout1->addWidget(nodeName[NODE_NUM]);
    hLayout2->addWidget(nodeNum[NODE_NUM]);
    hLayout3->addWidget(nodeMaxTime[NODE_NUM]);
    for(int i=0;i<NODE_NUM;i++) {
        hLayout1->addWidget(nodeName[i]);
        hLayout2->addWidget(nodeNum[i]);
        hLayout3->addWidget(nodeMaxTime[i]);
    }

    vLayout->addLayout(hLayout1);
    vLayout->addLayout(hLayout2);
    vLayout->addLayout(hLayout3);
    ui->frame->setLayout(vLayout);

    ui->pushButton->setText("重置@"+QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss"));

    time = new QTimer(this);
    connect(time, SIGNAL(timeout()), this, SLOT(update()));
    time->start(0);

    for(int i=0;i<NODE_NUM;i++)
        start[i].start();
}

void ReceiveError::errorHandle(int i)
{
    if(lastRunningJoint[i]
            && !GlobalData::runningId[i]) {
        ui->lcdNumber->display(++cout);
        nodeNum[i]->setNum(++coutId[i]);
        start[i].start();
    } else if(!lastRunningJoint[i]
              && GlobalData::runningId[i]
              && coutId[i]) {
        int elapsedTime = start[i].elapsed();
        maxTime[i] = elapsedTime>maxTime[i]?elapsedTime:maxTime[i];
        nodeMaxTime[i]->setNum(maxTime[i]);
    }
    if(GlobalData::runningId[i]) {
        nodeMaxTime[i]->setStyleSheet("color:black;");
    } else {
        nodeMaxTime[i]->setStyleSheet("color:red;");
    }
    lastRunningJoint[i] = GlobalData::runningId[i];
}

void ReceiveError::update()
{
    if(ui->comboBox->currentIndex()<NODE_NUM) {
        int j=ui->comboBox->currentIndex();
        errorHandle(j);
    } else {
        for(int i=0;i<NODE_NUM;i++) {
            errorHandle(i);
        }
    }
}

ReceiveError::~ReceiveError()
{
    delete time;
    for(int i=0;i<NODE_NUM+1;i++) {
        delete nodeNum[i];
        delete nodeName[i];
        delete nodeMaxTime[i];
    }
    delete hLayout1;
    delete hLayout2;
    delete hLayout3;
    delete ui;
}

void ReceiveError::on_pushButton_clicked()
{
    cout=0;
    for(int i=0;i<NODE_NUM;i++) {
        coutId[i]=0;
        maxTime[i]=0;
        nodeNum[i]->setNum(0);
        nodeMaxTime[i]->setNum(0);
    }
    ui->lcdNumber->display(cout);
    ui->pushButton->setText("重置@"+QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss"));
}
