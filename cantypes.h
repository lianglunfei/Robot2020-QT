/*
 * @Author: wuxingzhang
 * @Date: 2019-09-06 15:28:16
 * @LastEditors: wuxingzhang
 * @LastEditTime: 2019-09-06 15:28:16
 * @Description: file content
 */
#ifndef CANTYPES_H
#define CANTYPES_H

typedef unsigned short USHORT;
typedef unsigned short UINT16;
typedef unsigned char BYTE;
typedef char CHAR;
typedef unsigned char UCHAR;
typedef unsigned int UINT;
typedef unsigned short WORD;
typedef void *PVOID;
typedef void *HANDLE;
typedef unsigned long ULONG, DWORD;
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

#define VCI_PCI5121 1
#define VCI_PCI9810 2
#define VCI_USBCAN1 3
#define VCI_USBCAN2 4
#define VCI_USBCAN2A 4
#define VCI_PCI9820 5
#define VCI_CAN232 6
#define VCI_PCI5110 7
#define VCI_CANLITE 8
#define VCI_ISA9620 9
#define VCI_ISA5420 10
#define VCI_PC104CAN 11
#define VCI_CANETUDP 12
#define VCI_CANETE 12
#define VCI_DNP9810 13
#define VCI_PCI9840 14
#define VCI_PC104CAN2 15
#define VCI_PCI9820I 16
#define VCI_CANETTCP 17
#define VCI_PEC9920 18
#define VCI_PCIE_9220 18
#define VCI_PCI5010U 19
#define VCI_USBCAN_E_U 20
#define VCI_USBCAN_2E_U 21
#define VCI_PCI5020U 22
#define VCI_EG20T_CAN 23
#define VCI_PCIE9221 24
#define VCI_WIFICAN_TCP 25
#define VCI_WIFICAN_UDP 26
#define VCI_PCIe9120 27
#define VCI_PCIe9110 28
#define VCI_PCIe9140 29
#define VCI_USBCAN_4E_U 31
#define VCI_CANDTU_200UR 32
#define VCI_CANDTU_MINI 33
#define VCI_USBCAN_8E_U 34
#define VCI_CANREPLAY 35
#define VCI_CANDTU_NET 36
#define VCI_CANDTU_100UR 37

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

//CANET错误码
#define ERR_CANETE_PORTOPENED 0x00010000 //端口已经被打开
#define ERR_CANETE_INDEXUSED 0x00020000  //设备索引号已经被占用
#define ERR_REF_TYPE_ID 0x00030000       //SetReference或GetReference传递的RefType不存在
#define ERR_CREATE_SOCKET 0x00030002     //创建Socket失败
#define ERR_OPEN_CONNECT 0x00030003      //打开Socket的连接时失败，可能设备连接已经存在
#define ERR_NO_STARTUP 0x00030004        //设备没启动
#define ERR_NO_CONNECTED 0x00030005      //设备无连接
#define ERR_SEND_PARTIAL 0x00030006      //只发送了部分的CAN帧
#define ERR_SEND_TOO_FAST 0x00030007     //数据发得太快，Socket缓冲区满了

#define CMD_DESIP 0
#define CMD_DESPORT 1
#define CMD_CHGDESIPANDPORT 2
#define CMD_SRCPORT 2
#define CMD_TCP_TYPE 4 //tcp 工作方式，服务器:1 或是客户端:0
#define TCP_CLIENT 0
#define TCP_SERVER 1
//服务器方式下有效
#define CMD_CLIENT_COUNT 5       //连接上的客户端计数
#define CMD_CLIENT 6             //连接上的客户端
#define CMD_DISCONN_CLINET 7     //断开一个连接
#define CMD_SET_RECONNECT_TIME 8 //使能自动重连
//CANDTU_NET支持GPS
#define CMD_GET_GPS 9
#define CMD_GET_GPS_NUM 10 //获取GPS信息的数目

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

typedef struct tagRemoteClient
{
    int iIndex;
    DWORD port;
    HANDLE hClient;
    char szip[32];
} REMOTE_CLIENT;

typedef struct _tagChgDesIPAndPort
{
    char szpwd[10];
    char szdesip[20];
    int desport;
    BYTE blistenonly;
} CHGDESIPANDPORT;

///////// new add struct for filter /////////
typedef struct _FILTER_RECORD
{
    DWORD ExtFrame; //是否为扩展帧
    DWORD Start;
    DWORD End;
} FILTER_RECORD, *P_FILTER_RECORD;

//定时自动发送帧结构
typedef struct _VCI_AUTO_SEND_OBJ
{
    BYTE Enable;    //使能本条报文 0:禁能 1:使能
    BYTE Index;     //报文编号     最大支持32条报文
    DWORD Interval; //定时发送时间 1ms为单位
    _CAN_OBJ obj;   //报文
} VCI_AUTO_SEND_OBJ, *PVCI_AUTO_SEND_OBJ;

//设置指示灯状态结构
typedef struct _VCI_INDICATE_LIGHT
{
    BYTE Indicate;              //指示灯编号
    BYTE AttribRedMode : 2;     //Red LED灭/亮/闪烁/自控
    BYTE AttribGreenMode : 2;   //Green LED灭/亮/闪烁/自控
    BYTE AttribReserved : 4;    //保留暂时不用
    BYTE FrequenceRed : 2;      //Red LED闪烁频率
    BYTE FrequenceGreen : 2;    //Green LED闪烁频率
    BYTE FrequenceReserved : 4; //保留暂时不用
} VCI_INDICATE_LIGHT, *PVCI_INDICATE_LIGHT;

//设置转发结构
typedef struct _VCI_CAN_OBJ_REDIRECT
{
    BYTE Action;       //标识开启或停止转发
    BYTE DestCanIndex; //CAN目标通道
} VCI_CAN_OBJ_REDIRECT, *PVCI_CAN_OBJ_REDIRECT;

typedef struct _CANDTUTIME
{
    UINT16 wYear;
    UINT16 wMonth;
    UINT16 wDay;
    UINT16 wHour;
    UINT16 wMinute;
    UINT16 wSecond;
} CANDTUTIME;

//GPS数据结构
typedef struct _tagCANDTUGPSData
{
    float fLatitude;  //纬度
    float fLongitude; //经度
    float fSpeed;     //速度
    CANDTUTIME candtuTime;
} CANDTUGPSData, *PCANDTUGPSData;

//获取GPS结构
typedef struct _VCI_CANDTU_GPS_DATA
{
    PCANDTUGPSData pGPSData; //用户提供接收GPS数据的缓冲区地址
    ULONG nGPSDataCnt;       //可以容纳的GPS数据个数
} VCI_CANDTU_GPS_DATA, *PVCI_CANDTU_GPS_DATA;

#endif // CANTYPES_H
