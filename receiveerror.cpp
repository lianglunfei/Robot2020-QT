#include "receiveerror.h"
#include "ui_receiveerror.h"

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
    time = new QTimer(this);
    connect(time, SIGNAL(timeout()), this, SLOT(update()));
    time->start(0);
}

void ReceiveError::update()
{
    if(ui->comboBox->currentIndex()<12) {
        if(lastRunningJoint[ui->comboBox->currentIndex()]
                && !GlobalData::runningId[ui->comboBox->currentIndex()]) {
            ui->lcdNumber->display(++cout);
        }
        lastRunningJoint[ui->comboBox->currentIndex()] = GlobalData::runningId[ui->comboBox->currentIndex()];
    } else {
        for(int i=0;i<NODE_NUM;i++) {
            if(lastRunningJoint[i]
                    && !GlobalData::runningId[i]) {
                ui->lcdNumber->display(++cout);
            }
            lastRunningJoint[i] = GlobalData::runningId[i];
        }
    }
}

ReceiveError::~ReceiveError()
{
    delete ui;
}

void ReceiveError::on_pushButton_clicked()
{
    cout=0;
    ui->lcdNumber->display(cout);
}
