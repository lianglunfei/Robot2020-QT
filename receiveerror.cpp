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
    time = new QTimer(this);
    connect(time, SIGNAL(timeout()), this, SLOT(update()));
    time->start(0);
}

void ReceiveError::update()
{
    if(ui->comboBox->currentIndex()<12) {
        if(lastJointPos[ui->comboBox->currentIndex()]
                && !GlobalData::currentCanAnalyticalData[ui->comboBox->currentIndex()].position) {
            ui->lcdNumber->display(++cout);
        }
        lastJointPos[ui->comboBox->currentIndex()] = GlobalData::currentCanAnalyticalData[ui->comboBox->currentIndex()].position;
    } else {
        for(int i=0;i<NODE_NUM;i++) {
            if(lastJointPos[i]
                    && !GlobalData::currentCanAnalyticalData[i].position) {
                ui->lcdNumber->display(++cout);
            }
            lastJointPos[i] = GlobalData::currentCanAnalyticalData[i].position;
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
