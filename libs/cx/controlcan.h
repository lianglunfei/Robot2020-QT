#ifndef CONTROLCAN_H
#define CONTROLCAN_H

#include "cantypes.h"
#include <qsystemdetection.h>

////文件版本：v2.01 20190527
#ifdef __cplusplus
#define EXTERN_C extern "C"
#else
#define EXTERN_C
#endif

EXTERN_C DWORD VCI_OpenDevice(DWORD DeviceType, DWORD DeviceInd, DWORD Reserved);
EXTERN_C DWORD VCI_CloseDevice(DWORD DeviceType, DWORD DeviceInd);
EXTERN_C DWORD VCI_InitCAN(DWORD DeviceType, DWORD DeviceInd, DWORD CANInd, P_INIT_CONFIG pInitConfig);

EXTERN_C DWORD VCI_ReadBoardInfo(DWORD DeviceType, DWORD DeviceInd, P_BOARD_INFO pInfo);

EXTERN_C DWORD VCI_SetReference(DWORD DeviceType, DWORD DeviceInd, DWORD CANInd, DWORD RefType, PVOID pData);

EXTERN_C ULONG VCI_GetReceiveNum(DWORD DeviceType, DWORD DeviceInd, DWORD CANInd);
EXTERN_C DWORD VCI_ClearBuffer(DWORD DeviceType, DWORD DeviceInd, DWORD CANInd);

EXTERN_C DWORD VCI_StartCAN(DWORD DeviceType, DWORD DeviceInd, DWORD CANInd);
EXTERN_C DWORD VCI_ResetCAN(DWORD DeviceType, DWORD DeviceInd, DWORD CANInd);

EXTERN_C ULONG VCI_Transmit(DWORD DeviceType, DWORD DeviceInd, DWORD CANInd, P_CAN_OBJ pSend, UINT Len);
EXTERN_C ULONG VCI_Receive(DWORD DeviceType, DWORD DeviceInd, DWORD CANInd, P_CAN_OBJ pReceive, UINT Len, INT WaitTime);

EXTERN_C DWORD VCI_UsbDeviceReset(DWORD DevType, DWORD DevIndex, DWORD Reserved);
EXTERN_C DWORD VCI_FindUsbDevice2(P_BOARD_INFO pInfo);

#endif
