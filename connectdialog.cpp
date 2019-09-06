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
    ui->pluginListBox->addItem(tr("CANalyst"));
#ifdef Q_OS_WIN
    ui->pluginListBox->addItem(tr("GC"));
#endif
    ui->bauteComboBox->addItem(QString::number(CAN_BAUTE_1000));
    ui->bauteComboBox->addItem(QString::number(CAN_BAUTE_800));
    ui->bauteComboBox->addItem(QString::number(CAN_BAUTE_666));
    ui->bauteComboBox->addItem(QString::number(CAN_BAUTE_500));
    ui->bauteComboBox->addItem(QString::number(CAN_BAUTE_400));
    ui->bauteComboBox->addItem(QString::number(CAN_BAUTE_250));
    ui->bauteComboBox->addItem(QString::number(CAN_BAUTE_200));
    ui->bauteComboBox->addItem(QString::number(CAN_BAUTE_125));

    this->setWindowFlags(Qt::WindowStaysOnTopHint);
    this->setWindowTitle(tr("Connect"));
}

ConnectDialog::~ConnectDialog()
{
    delete ui;
}

void ConnectDialog::accept()
{
    int connectType=ui->pluginListBox->currentIndex()+1;
    int baud=ui->bauteComboBox->currentIndex();

    if(DataTransmission::CANOpenDevice(connectType)!=1) {
        qDebug() << "open failed";
        return;
    }

    if(DataTransmission::InitCANHelper(connectType, 0, baud)!=1) {
        qDebug() << "Init-CAN Channel 0 failed!";
        return;
    }

    if(DataTransmission::StartCANHelper(connectType, 0)!=1) {
        qDebug() << "Start-CAN Channel 0 failed!";
        return;
    }

    if(DataTransmission::InitCANHelper(connectType, 1, baud)!=1) {
        qDebug() << "Init-CAN Channel 1 failed!";
        return;
    }

    if(DataTransmission::StartCANHelper(connectType, 1)!=1) {
        qDebug() << "Start-CAN Channel 1 failed!";
        return;
    }

    GlobalData::connectType = connectType;

    qDebug() << "Connect CAN OK!";
    return QDialog::accept();
}
