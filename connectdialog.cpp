/*
 * @Author: xingzhang.Wu 
 * @Date: 2019-09-29 09:57:48 
 * @Last Modified by:   xingzhang.Wu 
 * @Last Modified time: 2019-09-29 09:57:48 
 */
#include "connectdialog.h"
#include "ui_connectdialog.h"

#include "datatransmission.h"
#include "globaldata.h"

#include "debug.h"

ConnectDialog::ConnectDialog(QWidget *parent) : QDialog(parent),
                                                ui(new Ui::ConnectDialog)
{
    ui->setupUi(this);
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
    if (DataTransmission::CANCloseDevice(global->connectType) != -1)
        qDebug() << "Disconnect CAN OK!";
    delete ui;
}

void ConnectDialog::disconnect()
{
    if (DataTransmission::CANCloseDevice(global->connectType) != -1) {
        global->connectType = 0;
        qDebug() << "Disconnect CAN OK!";
    }
}

void ConnectDialog::accept()
{
    int baud = ui->bauteComboBox->currentIndex();

    if (DataTransmission::connectToCan(global->connectType, baud) == -1)
        return;

    qDebug() << "Connect CAN OK!";
    return QDialog::accept();
}
