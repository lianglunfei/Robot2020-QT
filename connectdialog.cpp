#include "connectdialog.h"
#include "ui_connectdialog.h"

#include "datatransmission.h"
#include "globaldata.h"

#include "debug.h"

ConnectDialog::ConnectDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ConnectDialog)
{
    ui->setupUi(this);
    this->setWindowFlags(Qt::WindowStaysOnTopHint);
    this->setWindowTitle(tr("Connect"));
}

ConnectDialog::~ConnectDialog()
{
    delete ui;
}

void ConnectDialog::on_buttonBox_accepted()
{
    int connectType=ui->pluginListBox->currentIndex()+1;
    int baud=ui->bauteComboBox->currentIndex();

    if(DataTransmission::CANOpenDevice(connectType)!=1) {
        qDebug() << "open failed";
        return;
    }

    if(DataTransmission::InitCAN(connectType, 0, baud)!=1) {
        qDebug() << "Init-CAN Channel 0 failed!";
        return;
    }

    if(DataTransmission::StartCAN(connectType, 0)!=1) {
        qDebug() << "Start-CAN Channel 0 failed!";
        return;
    }

    if(DataTransmission::InitCAN(connectType, 1, baud)!=1) {
        qDebug() << "Init-CAN Channel 1 failed!";
        return;
    }

    if(DataTransmission::StartCAN(connectType, 1)!=1) {
        qDebug() << "Start-CAN Channel 1 failed!";
        return;
    }

    GlobalData::connectType = connectType;

    qDebug() << "Connect CAN OK!";
    accept();
}
