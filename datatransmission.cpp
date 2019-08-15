#include "datatransmission.h"
#include "globaldata.h"

#include "debug.h"

/**
*@projectName   RobotControlSystem
*@brief         作为持久层，用于提供发送/接收CAN数据、读取CAN数据、读写串口数据接口等
*@parameter
*@author        XingZhang.Wu
*@date          20190710
**/
DataTransmission::DataTransmission()
{

}

/**
*@projectName   RobotControlSystem
*@brief         使用CAN第一步，打开CAN
*@parameter
*@author        XingZhang.Wu
*@date          20190801
**/
int DataTransmission::CANOpenDevice(int connectType)
{
    switch (connectType) {
    case CONNECT_TYPE_ALYSIST:
        return VCI_OpenDevice(VCI_USBCAN1,0,0);
    default:
        break;
    }
    return 0;
}

/**
*@projectName   RobotControlSystem
*@brief         使用CAN第二步，初始化CAN
*@parameter
*@author        XingZhang.Wu
*@date          20190801
**/
int DataTransmission::InitCAN(int connectType, int devIndex, int baud)
{
    switch (connectType) {
    case CONNECT_TYPE_ALYSIST:
        VCI_INIT_CONFIG InitInfo;  //结构体
        switch (baud) {
        case 0: //1000
            InitInfo.Timing0 = 0;
            InitInfo.Timing1 =0x14;
            break;
        case 1: //800
            InitInfo.Timing0 = 0;
            InitInfo.Timing1 = 0x16;
            break;
        case 2: //666
            InitInfo.Timing0 = 0x80;
            InitInfo.Timing1 = 0xb6;
            break;
        case 3: //500
            InitInfo.Timing0 = 0;
            InitInfo.Timing1 = 0x1c;
            break;
        case 4://400
            InitInfo.Timing0 = 0x80;
            InitInfo.Timing1 = 0xfa;
            break;
        case 5://250
            InitInfo.Timing0 = 0x01;
            InitInfo.Timing1 = 0x1c;
            break;
        case 6://200
            InitInfo.Timing0 = 0x81;
            InitInfo.Timing1 = 0xfa;
            break;
        case 7://125
            InitInfo.Timing0 = 0x03;
            InitInfo.Timing1 = 0x1c;
            break;
        case 8://100
            InitInfo.Timing0 = 0x04;
            InitInfo.Timing1 = 0x1c;
            break;
        case 9://80
            InitInfo.Timing0 = 0x83;
            InitInfo.Timing1 = 0xff;
            break;
        case 10://50
            InitInfo.Timing0 = 0x09;
            InitInfo.Timing1 = 0x1c;
            break;
        }
        InitInfo.Filter=0;         //滤波模式-->同时对标准帧和拓展帧过滤
        InitInfo.AccCode=0x80000000;    //验收码
        InitInfo.AccMask=0xFFFFFFFF;  //屏蔽码-->全部接收
        InitInfo.Mode=VCI_NORMAL;   //自发自收模式
        return VCI_InitCAN(VCI_USBCAN1, 0, devIndex,&InitInfo);
    }
    return 0;
}

/**
*@projectName   RobotControlSystem
*@brief         使用CAN第三步，开始CAN
*@parameter
*@author        XingZhang.Wu
*@date          20190801
**/
int DataTransmission::StartCAN(int connectType, int devIndex)
{
    switch (connectType) {
    case CONNECT_TYPE_ALYSIST:
        return VCI_StartCAN(VCI_USBCAN1, 0, devIndex);
    default:
        break;
    }
    return 0;
}

/**
*@projectName   RobotControlSystem
*@brief         发送函数接口，区分不同CAN卡，其他类可以直接调用，不用管里边是怎么实现的。
*@parameter
*@author        XingZhang.Wu
*@date          20190712
**/
int DataTransmission::CANTransmit(int connectType, unsigned char data[], int id)
{
    switch (connectType) {
    case CONNECT_TYPE_ALYSIST:
        VCI_CAN_OBJ frameinfo;
        frameinfo.SendType=0;
        frameinfo.DataLen=8;
        frameinfo.RemoteFlag=0;
        frameinfo.ExternFlag=0;
        frameinfo.ID=(DWORD2)id;
        qDebug() << "id: " << id;
        memset(&frameinfo.Data, 0 ,sizeof(frameinfo.Data));
        for(int i=0;i<8;i++) {
            frameinfo.Data[i]=data[i];
            qDebug() << data[i];
        }
        return VCI_Transmit(VCI_USBCAN1, 0, 0, &frameinfo, 1);
        break;
    default:
        break;
    }
    return 0;
}

/**
*@projectName   RobotControlSystem
*@brief         同时发送多帧数据的底层CAN数据接口，在这里对不同的CAN卡做区分，上层无需理会这里是怎么实现的。
*@parameter
*@author        XingZhang.Wu
*@date          20190723
**/
int DataTransmission::CANTransmitMulti(int connectType, unsigned char data[][8], int id[], int len)
{
    switch (connectType) {
    case CONNECT_TYPE_ALYSIST:
        VCI_CAN_OBJ frameinfo[len];
        for(int i=0;i<len;i++) {
            frameinfo[i].SendType=0;
            frameinfo[i].DataLen=8;
            frameinfo[i].RemoteFlag=0;
            frameinfo[i].ExternFlag=0;
            frameinfo[i].ID=(DWORD2)id[i];
            memset(&frameinfo[i].Data, 0 ,sizeof(frameinfo[i].Data));
            for(int j=0;j<8;j++) {
                frameinfo[i].Data[j]=data[i][j];
            }
        }
        return VCI_Transmit(VCI_USBCAN1, 0, 0, frameinfo, len);
        break;
    }
    return 0;
}

/**
*@projectName   RobotControlSystem
*@brief         接收CAN数据函数接口，自动区分不同的CAN卡，读取到的数据以统一的接口开放给外部使用。
*@parameter
*@author        XingZhang.Wu
*@date          20190712
**/
int DataTransmission::CANReceive(int connectType, QStringList &list, int dataLen[], int id[], unsigned char data[][8])
{
    switch (connectType) {
    case CONNECT_TYPE_ALYSIST:
        VCI_CAN_OBJ frameinfo[50];
        int len=1;
        len=VCI_Receive(VCI_USBCAN1,0,0,frameinfo,50,10);
        QString str;
        QString tmpstr;

        if(len>0)
        {
            for(int i=0;i<len;i++) {
                str = "";
                if(frameinfo[i].TimeFlag==0)
                    tmpstr=" ";
                else
                    tmpstr=QString("%1     ").arg(frameinfo[i].TimeStamp);
                str+=tmpstr;
                if(frameinfo[i].RemoteFlag==0) //Format
                    tmpstr="Data     ";
                else
                    tmpstr="Remote   ";
                str+=tmpstr;
                if(frameinfo[i].ExternFlag==0) //Type
                    tmpstr="Stand    ";
                else
                    tmpstr="Exten    ";
                str+=tmpstr;
                tmpstr=QString("%1       ").arg(frameinfo[i].ID,2,16,QLatin1Char('0'));
                str+=tmpstr;

                id[i] = frameinfo[i].ID;

                 //Data
                if(frameinfo[i].RemoteFlag==0) {
                    if(frameinfo[i].DataLen>8)
                        frameinfo[i].DataLen=8;
                    dataLen[i]=frameinfo[i].DataLen;

                    for(int j=0;j<frameinfo[i].DataLen;j++) {
                        str+=QString("%1   ").arg(frameinfo[i].Data[j],2,16,QLatin1Char('0'));
                        data[i][j]=frameinfo[i].Data[j];
                    }
                }
#ifdef OPEN_DEBUG
//                qDebug() << str;
#endif
                list.append(str);
            }
        }
        return len;
    }
}
