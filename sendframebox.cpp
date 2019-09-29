/*
 * @Author: xingzhang.Wu 
 * @Date: 2019-09-29 10:04:59 
 * @Last Modified by: xingzhang.Wu
 * @Last Modified time: 2019-09-29 11:24:20
 */
#include "sendframebox.h"
#include "ui_sendframebox.h"

#include "hexintegervalidator.h"
#include "hexstringvalidator.h"
#include "convert.h"
#include "datatransmission.h"
#include "globaldata.h"
#include "debug.h"

#include <QTimer>

/**
 * @brief Construct a new Send Frame Box:: Send Frame Box object
 * 
 * @param parent 
 */
SendFrameBox::SendFrameBox(QWidget *parent) : QGroupBox(parent),
                                              ui(new Ui::SendFrameBox)
{
    ui->setupUi(this);

    m_hexIntegerValidator = new HexIntegerValidator(this);
    ui->frameIdEdit->setValidator(m_hexIntegerValidator);
    m_hexStringValidator = new HexStringValidator(this);
    ui->payloadEdit->setValidator(m_hexStringValidator);

    connect(ui->dataFrame, &QRadioButton::toggled, [this](bool set) {
        if (set)
            ui->flexibleDataRateBox->setEnabled(true);
    });

    connect(ui->remoteFrame, &QRadioButton::toggled, [this](bool set) {
        if (set)
        {
            ui->flexibleDataRateBox->setEnabled(false);
            ui->flexibleDataRateBox->setChecked(false);
        }
    });

    connect(ui->errorFrame, &QRadioButton::toggled, [this](bool set) {
        if (set)
        {
            ui->flexibleDataRateBox->setEnabled(false);
            ui->flexibleDataRateBox->setChecked(false);
        }
    });

    connect(ui->extendedFormatBox, &QCheckBox::toggled, [this](bool set) {
        m_hexIntegerValidator->setMaximum(set ? MaxExtendedId : MaxStandardId);
    });

    connect(ui->flexibleDataRateBox, &QCheckBox::toggled, [this](bool set) {
        m_hexStringValidator->setMaxLength(set ? MaxPayloadFd : MaxPayload);
        ui->bitrateSwitchBox->setEnabled(set);
        if (!set)
            ui->bitrateSwitchBox->setChecked(false);
    });

    auto frameIdTextChanged = [this]() {
        const bool hasFrameId = !ui->frameIdEdit->text().isEmpty();
        ui->sendButton->setEnabled(true);
        ui->sendButton->setToolTip(hasFrameId
                                       ? QString()
                                       : tr("Cannot send because no Frame ID was given."));
    };
    connect(ui->frameIdEdit, &QLineEdit::textChanged, frameIdTextChanged);

    repeatTimer = new QTimer(this);
    connect(repeatTimer, SIGNAL(timeout()), this, SLOT(repeatSendProcess()));
}

/**
 * @brief 解读界面输入的数据，调用底层CAN发送接口，将数据一次性或定时发送出去
 * 
 */
void SendFrameBox::sendFrameData()
{
    char szFrameID[9];
    unsigned char FrameID[4] = {0, 0, 0, 0};
    memset(szFrameID, '0', 9);
    char szData[25];
    unsigned char datalen = 0;

    QString sendInfo;

    if (ui->frameIdEdit->text().length() == 0 ||
        (ui->payloadEdit->text().length() == 0 && ui->dataFrame->isChecked()))
    {
        sendInfo.append("请输入数据");
    }

    if (ui->frameIdEdit->text().length() > 8)
    {
        sendInfo.append("ID值超过范围");
    }

    if (ui->payloadEdit->text().length() > 24)
    {
        sendInfo.append("数据长度超过范围,最大为8个字节");
    }

    if (ui->dataFrame->isChecked())
    {
        if (ui->frameIdEdit->text().length() % 3 == 1)
        {
            sendInfo.append("数据格式不对,请重新输入");
        }
    }

    if (!sendInfo.isEmpty())
    {
        return;
    }

    memcpy(&szFrameID[8 - (ui->frameIdEdit->text().length())], ui->frameIdEdit->text().toLatin1(), ui->frameIdEdit->text().length());

    qDebug() << "FrameID Len" << ui->frameIdEdit->text().length();
    Convert::strToData((unsigned char *)szFrameID, FrameID, 4, 0);

    datalen = (ui->payloadEdit->text().length() + 1) / 3;
    qDebug() << "datalen" << datalen;
    strcpy(szData, ui->payloadEdit->text().toLatin1());
    Convert::strToData((unsigned char *)szData, currentData, datalen, 1);
    qDebug() << "SzDATA" << szData;
    qDebug() << "DATA" << currentData[0] << currentData[1] << currentData[2] << currentData[3] << currentData[4] << currentData[5] << currentData[6] << currentData[7];

    //Id=((DWORD2)FrameID[0]<<24)+((DWORD2)FrameID[1]<<16)+((DWORD2)FrameID[2]<<8)+((DWORD2)FrameID[3])
    currentId = ((DWORD)FrameID[2] << 8) + ((DWORD)FrameID[3]);

    int ret = DataTransmission::CANTransmit(global->connectType, currentData, currentId);
    if (ret == -1)
    {
        qDebug() << "failed- device not open\n"; //=-1表示USB-CAN设备不存在或USB掉线
        return;
    }
    else if (ret == 0)
    {
        qDebug() << "send error\n";
        return;
    }
    else
    {
        qDebug() << "send successful\n";
    }

    if (ui->repeatSendCheckBox->isChecked())
        repeatTimer->start(ui->periodSpinBox->text().toInt());
}

/**
 * @brief 定时发送实际处理函数
 * 
 */
void SendFrameBox::repeatSendProcess()
{
    int ret = DataTransmission::CANTransmit(global->connectType, currentData, currentId);
    if (ret == -1)
        qDebug() << "failed- device not open\n"; //=-1表示USB-CAN设备不存在或USB掉线
    else if (ret == 0)
        qDebug() << "send error\n";
    else
        qDebug() << "send successful\n";
}

/**
 * @brief 按钮触发发送函数
 * 
 */
void SendFrameBox::on_sendButton_clicked()
{
    if (global->connectType)
        sendFrameData();
}

/**
 * @brief 定时发送停止
 * 
 * @param arg1 
 */
void SendFrameBox::on_repeatSendCheckBox_stateChanged(int arg1)
{
    if (arg1 == 0)
        repeatTimer->stop();
}

/**
 * @brief Destroy the Send Frame Box:: Send Frame Box object
 * 
 */
SendFrameBox::~SendFrameBox()
{
    delete ui;
}

/**
 * @brief 是否回显接收到的CAN信息
 * 
 * @param arg1 
 */
void SendFrameBox::on_receiveMsgCheckBox_stateChanged(int arg1)
{
    if (arg1 == 0)
        global->showDebugInfo = false;
    else
        global->showDebugInfo = true;
}
