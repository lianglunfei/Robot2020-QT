#ifndef CANTYPES_H
#define CANTYPES_H

typedef unsigned short USHORT;
typedef unsigned char BYTE;
typedef char CHAR;
typedef unsigned char UCHAR;
typedef unsigned int UINT;
typedef unsigned short WORD;
typedef unsigned int DWORD;
typedef void *PVOID;
typedef unsigned long ULONG;
typedef int INT;

//函数调用返回状态值
#define STATUS_OK 1
#define STATUS_ERR 0

#define TRUE 1
#define FALSE 0

//接口卡类型定义

#define USBCAN1 3
#define USBCAN2 4
#define USBCAN2A 4

#define USBCAN_E_U 20
#define USBCAN_2E_U 21

//设备模式定义
#define NORMAL 0
#define ONLY_LISTEN 1
#define SPONTANEOUSLY 2

//CAN错误码
#define ERR_CAN_OVERFLOW 0x0001 //CAN控制器内部FIFO溢出
#define ERR_CAN_ERRALARM 0x0002 //CAN控制器错误报警
#define ERR_CAN_PASSIVE 0x0004  //CAN控制器消极错误
#define ERR_CAN_LOSE 0x0008     //CAN控制器仲裁丢失
#define ERR_CAN_BUSERR 0x0010   //CAN控制器总线错误
#define ERR_CAN_REG_FULL 0x0020 //CAN接收寄存器满
#define ERR_CAN_REG_OVER 0x0040 //CAN接收寄存器溢出
#define ERR_CAN_ZHUDONG 0x0080  //CAN控制器主动错误

//通用错误码
#define ERR_DEVICEOPENED 0x0100   //设备已经打开
#define ERR_DEVICEOPEN 0x0200     //打开设备错误
#define ERR_DEVICENOTOPEN 0x0400  //设备没有打开
#define ERR_BUFFEROVERFLOW 0x0800 //缓冲区溢出
#define ERR_DEVICENOTEXIST 0x1000 //此设备不存在
#define ERR_LOADKERNELDLL 0x2000  //装载动态库失败
#define ERR_CMDFAILED 0x4000      //执行命令失败错误码
#define ERR_BUFFERCREATE 0x8000   //内存不足

#define CMD_DESIP 0
#define CMD_DESPORT 1
#define CMD_CHGDESIPANDPORT 2

//1.ZLGCAN系列接口卡信息的数据类型。
typedef struct _BOARD_INFO
{
    USHORT hw_Version;
    USHORT fw_Version;
    USHORT dr_Version;
    USHORT in_Version;
    USHORT irq_Num;
    BYTE can_Num;
    CHAR str_Serial_Num[20];
    CHAR str_hw_Type[40];
    USHORT Reserved[4];
} BOARD_INFO, *P_BOARD_INFO;

//2.定义CAN信息帧的数据类型。
typedef struct _CAN_OBJ
{
    UINT ID;
    UINT TimeStamp;
    BYTE TimeFlag;
    BYTE SendType;
    BYTE RemoteFlag; //是否是远程帧
    BYTE ExternFlag; //是否是扩展帧
    BYTE DataLen;
    BYTE Data[8];
    BYTE Reserved[3];
} CAN_OBJ, *P_CAN_OBJ;

//3.定义CAN控制器状态的数据类型。
typedef struct _CAN_STATUS
{
    UCHAR ErrInterrupt;
    UCHAR regMode;
    UCHAR regStatus;
    UCHAR regALCapture;
    UCHAR regECCapture;
    UCHAR regEWLimit;
    UCHAR regRECounter;
    UCHAR regTECounter;
    DWORD Reserved;
} CAN_STATUS, *P_CAN_STATUS;

//4.定义错误信息的数据类型。
typedef struct _ERR_INFO
{
    UINT ErrCode;
    BYTE Passive_ErrData[3];
    BYTE ArLost_ErrData;
} ERR_INFO, *P_ERR_INFO;

//5.定义初始化CAN的数据类型
typedef struct _INIT_CONFIG
{
    DWORD AccCode;
    DWORD AccMask;
    DWORD Reserved;
    UCHAR Filter;
    UCHAR Timing0;
    UCHAR Timing1;
    UCHAR Mode;
} INIT_CONFIG, *P_INIT_CONFIG;

typedef struct _tagChgDesIPAndPort
{
    char szpwd[10];
    char szdesip[20];
    int desport;
} CHGDESIPANDPORT;

///////// new add struct for filter /////////
typedef struct _FILTER_RECORD
{
    DWORD ExtFrame; //是否为扩展帧
    DWORD Start;
    DWORD End;
} FILTER_RECORD, *P_FILTER_RECORD;

#endif // CANTYPES_H
