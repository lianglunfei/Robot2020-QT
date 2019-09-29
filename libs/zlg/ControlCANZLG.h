/*
 * @Author: xingzhang.Wu 
 * @Date: 2019-09-29 10:06:21 
 * @Last Modified by:   xingzhang.Wu 
 * @Last Modified time: 2019-09-29 10:06:21 
 */
#ifndef CONTROLCAN_H
#define CONTROLCAN_H

#include "cantypes.h"

#ifdef __cplusplus
#define EXTERNC extern "C"
#define DEF(a) = a
#else
#define EXTERNC
#define DEF(a)
#endif

#ifdef Q_OS_WIN
#define CALL __stdcall
#else
#define CALL
#endif

EXTERNC DWORD CALL VCI_OpenDevice(DWORD DeviceType,DWORD DeviceInd,DWORD Reserved);
EXTERNC DWORD CALL VCI_CloseDevice(DWORD DeviceType,DWORD DeviceInd);
EXTERNC DWORD CALL VCI_InitCAN(DWORD DeviceType, DWORD DeviceInd, DWORD CANInd, P_INIT_CONFIG pInitConfig);

EXTERNC DWORD CALL VCI_ReadBoardInfo(DWORD DeviceType,DWORD DeviceInd,P_BOARD_INFO pInfo);
EXTERNC DWORD CALL VCI_ReadErrInfo(DWORD DeviceType,DWORD DeviceInd,DWORD CANInd,P_ERR_INFO pErrInfo);
EXTERNC DWORD CALL VCI_ReadCANStatus(DWORD DeviceType,DWORD DeviceInd,DWORD CANInd,P_CAN_STATUS pCANStatus);

EXTERNC DWORD CALL VCI_GetReference(DWORD DeviceType,DWORD DeviceInd,DWORD CANInd,DWORD RefType,PVOID pData);
EXTERNC DWORD CALL VCI_SetReference(DWORD DeviceType,DWORD DeviceInd,DWORD CANInd,DWORD RefType,PVOID pData);

EXTERNC ULONG CALL VCI_GetReceiveNum(DWORD DeviceType,DWORD DeviceInd,DWORD CANInd);
EXTERNC DWORD CALL VCI_ClearBuffer(DWORD DeviceType,DWORD DeviceInd,DWORD CANInd);

EXTERNC DWORD CALL VCI_StartCAN(DWORD DeviceType,DWORD DeviceInd,DWORD CANInd);
EXTERNC DWORD CALL VCI_ResetCAN(DWORD DeviceType,DWORD DeviceInd,DWORD CANInd);

EXTERNC ULONG CALL VCI_Transmit(DWORD DeviceType,DWORD DeviceInd,DWORD CANInd,P_CAN_OBJ pSend,ULONG Len);
EXTERNC ULONG CALL VCI_Receive(DWORD DeviceType,DWORD DeviceInd,DWORD CANInd,P_CAN_OBJ pReceive,ULONG Len,INT WaitTime DEF(-1));

#endif
