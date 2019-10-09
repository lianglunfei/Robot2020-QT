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
#define EXTERN_C  extern "C"
#else
#define EXTERN_C
#endif

#ifdef Q_OS_WIN
#define CALL __stdcall
#else
#define CALL
#endif

EXTERN_C DWORD VCI_OpenDevice(DWORD DeviceType, DWORD DeviceInd, DWORD Reserved);
EXTERN_C DWORD VCI_CloseDevice(DWORD DeviceType, DWORD DeviceInd);
EXTERN_C DWORD VCI_InitCAN(DWORD DeviceType, DWORD DeviceInd, DWORD CANInd, PVCI_INIT_CONFIG pInitConfig);

EXTERN_C DWORD VCI_ReadBoardInfo(DWORD DeviceType, DWORD DeviceInd, PVCI_BOARD_INFO pInfo);
EXTERN_C DWORD VCI_ReadErrInfo(DWORD DeviceType, DWORD DeviceInd, DWORD CANInd, PVCI_ERR_INFO pErrInfo);
EXTERN_C DWORD VCI_ReadCANStatus(DWORD DeviceType, DWORD DeviceInd, DWORD CANInd, PVCI_CAN_STATUS pCANStatus);

EXTERN_C DWORD VCI_GetReference(DWORD DeviceType, DWORD DeviceInd, DWORD CANInd, DWORD RefType, PVOID pData);
EXTERN_C DWORD VCI_SetReference(DWORD DeviceType, DWORD DeviceInd, DWORD CANInd, DWORD RefType, PVOID pData);

EXTERN_C ULONG VCI_GetReceiveNum(DWORD DeviceType, DWORD DeviceInd, DWORD CANInd);
EXTERN_C DWORD VCI_ClearBuffer(DWORD DeviceType, DWORD DeviceInd, DWORD CANInd);

EXTERN_C DWORD VCI_StartCAN(DWORD DeviceType, DWORD DeviceInd, DWORD CANInd);
EXTERN_C DWORD VCI_ResetCAN(DWORD DeviceType, DWORD DeviceInd, DWORD CANInd);

EXTERN_C ULONG VCI_Transmit(DWORD DeviceType, DWORD DeviceInd, DWORD CANInd, PVCI_CAN_OBJ pSend, UINT Len);
EXTERN_C ULONG VCI_Receive(DWORD DeviceType, DWORD DeviceInd, DWORD CANInd, PVCI_CAN_OBJ pReceive, UINT Len, INT WaitTime);
#endif
