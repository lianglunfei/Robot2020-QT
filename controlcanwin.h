#ifndef CONTROLCANWIN_H
#define CONTROLCANWIN_H

#include <qsystemdetection.h>

#if defined(Q_OS_WIN)

////文件版本：v2.00 20150920
//#include <cvidef.h>	//使用CVI平台开发，请使用该语句。

//接口卡类型定义

#define VCI_USBCAN1		3
#define VCI_USBCAN2		4
#define VCI_USBCAN2A		4

#define VCI_USBCAN_E_U 		20
#define VCI_USBCAN_2E_U 	21

typedef unsigned short USHORT;
typedef unsigned char BYTE;
typedef char CHAR;
typedef unsigned int UINT;
typedef int INT;
typedef unsigned char UCHAR;
typedef unsigned short WORD;
typedef unsigned int DWORD2;
typedef unsigned long ULONG;
//typedef unsigned long DWORD;
typedef void * PVOID;


//函数调用返回状态值
#define	STATUS_OK					1
#define STATUS_ERR					0

//设备模式定义
#define VCI_NORMAL          0
#define VCI_ONLY_LISTEN     1
#define VCI_SPONTANEOUSLY   2

/*------------------------------------------------兼容ZLG的函数及数据类型------------------------------------------------*/
//1.ZLGCAN系列接口卡信息的数据类型。
typedef  struct  _VCI_BOARD_INFO{
        USHORT	hw_Version;
        USHORT	fw_Version;
        USHORT	dr_Version;
        USHORT	in_Version;
        USHORT	irq_Num;
        BYTE	can_Num;
        CHAR	str_Serial_Num[20];
        CHAR	str_hw_Type[40];
        USHORT	Reserved[4];
} VCI_BOARD_INFO,*PVCI_BOARD_INFO;

//2.定义CAN信息帧的数据类型。
typedef  struct  _VCI_CAN_OBJ{
    UINT	ID;
    UINT	TimeStamp;
    BYTE	TimeFlag;
    BYTE	SendType;
    BYTE	RemoteFlag;//是否是远程帧
    BYTE	ExternFlag;//是否是扩展帧
    BYTE	DataLen;
    BYTE	Data[8];
    BYTE	Reserved[3];
}VCI_CAN_OBJ,*PVCI_CAN_OBJ;

//3.定义初始化CAN的数据类型
typedef struct _VCI_INIT_CONFIG{
    DWORD2	AccCode;
    DWORD2	AccMask;
    DWORD2	Reserved;
    UCHAR	Filter;
    UCHAR	Timing0;
    UCHAR	Timing1;
    UCHAR	Mode;
}VCI_INIT_CONFIG,*PVCI_INIT_CONFIG;

///////// new add struct for filter /////////
typedef struct _VCI_FILTER_RECORD{
    DWORD2 ExtFrame;	//是否为扩展帧
    DWORD2 Start;
    DWORD2 End;
}VCI_FILTER_RECORD,*PVCI_FILTER_RECORD;

#define EXTERNC		extern "C"

EXTERNC DWORD2 __stdcall VCI_OpenDevice(DWORD2 DeviceType,DWORD2 DeviceInd,DWORD2 Reserved);
EXTERNC DWORD2 __stdcall VCI_CloseDevice(DWORD2 DeviceType,DWORD2 DeviceInd);
EXTERNC DWORD2 __stdcall VCI_InitCAN(DWORD2 DeviceType, DWORD2 DeviceInd, DWORD2 CANInd, PVCI_INIT_CONFIG pInitConfig);

EXTERNC DWORD2 __stdcall VCI_ReadBoardInfo(DWORD2 DeviceType,DWORD2 DeviceInd,PVCI_BOARD_INFO pInfo);

EXTERNC DWORD2 __stdcall VCI_SetReference(DWORD2 DeviceType,DWORD2 DeviceInd,DWORD2 CANInd,DWORD2 RefType,PVOID pData);

EXTERNC ULONG __stdcall VCI_GetReceiveNum(DWORD2 DeviceType,DWORD2 DeviceInd,DWORD2 CANInd);
EXTERNC DWORD2 __stdcall VCI_ClearBuffer(DWORD2 DeviceType,DWORD2 DeviceInd,DWORD2 CANInd);

EXTERNC DWORD2 __stdcall VCI_StartCAN(DWORD2 DeviceType,DWORD2 DeviceInd,DWORD2 CANInd);
EXTERNC DWORD2 __stdcall VCI_ResetCAN(DWORD2 DeviceType,DWORD2 DeviceInd,DWORD2 CANInd);

EXTERNC ULONG __stdcall VCI_Transmit(DWORD2 DeviceType,DWORD2 DeviceInd,DWORD2 CANInd,PVCI_CAN_OBJ pSend,ULONG Len);
EXTERNC ULONG __stdcall VCI_Receive(DWORD2 DeviceType,DWORD2 DeviceInd,DWORD2 CANInd,PVCI_CAN_OBJ pReceive,ULONG Len,INT WaitTime);


/*------------------------------------------------其他补充函数及数据结构描述------------------------------------------------*/

//USB-CAN总线适配器板卡信息的数据类型1，该类型为VCI_FindUsbDevice函数的返回参数。
typedef  struct  _VCI_BOARD_INFO1{
    USHORT	hw_Version;
    USHORT	fw_Version;
    USHORT	dr_Version;
    USHORT	in_Version;
    USHORT	irq_Num;
    BYTE	can_Num;
    BYTE	Reserved;
    CHAR	str_Serial_Num[8];
    CHAR	str_hw_Type[16];
    CHAR	str_Usb_Serial[4][4];
} VCI_BOARD_INFO1,*PVCI_BOARD_INFO1;

//USB-CAN总线适配器板卡信息的数据类型2，该类型为VCI_FindUsbDevice函数的返回参数。为扩展更多的设备
typedef  struct  _VCI_BOARD_INFO2{
    USHORT	hw_Version;
    USHORT	fw_Version;
    USHORT	dr_Version;
    USHORT	in_Version;
    USHORT	irq_Num;
    BYTE	can_Num;
    BYTE	Reserved;
    CHAR	str_Serial_Num[8];
    CHAR	str_hw_Type[16];
    CHAR	str_Usb_Serial[10][4];
} VCI_BOARD_INFO2,*PVCI_BOARD_INFO2;


#define EXTERNC		extern "C"

EXTERNC DWORD2 __stdcall VCI_GetReference2(DWORD2 DevType,DWORD2 DevIndex,DWORD2 CANIndex,DWORD2 Reserved,BYTE *pData);
EXTERNC DWORD2 __stdcall VCI_SetReference2(DWORD2 DevType,DWORD2 DevIndex,DWORD2 CANIndex,DWORD2 RefType,BYTE *pData);


EXTERNC DWORD2 __stdcall VCI_ConnectDevice(DWORD2 DevType,DWORD2 DevIndex);
EXTERNC DWORD2 __stdcall VCI_UsbDeviceReset(DWORD2 DevType,DWORD2 DevIndex,DWORD2 Reserved);
EXTERNC DWORD2 __stdcall VCI_FindUsbDevice(PVCI_BOARD_INFO1 pInfo);
EXTERNC DWORD2 __stdcall VCI_FindUsbDevice2(PVCI_BOARD_INFO2 pInfo);

#endif

#endif
