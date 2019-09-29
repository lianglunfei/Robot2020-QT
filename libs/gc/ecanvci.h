/*
 * @Author: xingzhang.Wu 
 * @Date: 2019-09-29 10:05:42 
 * @Last Modified by:   xingzhang.Wu 
 * @Last Modified time: 2019-09-29 10:05:42 
 */
#ifndef ECANVCI_H
#define ECANVCI_H

#include "cantypes.h"
#include "qsystemdetection.h"

#ifdef Q_OS_WIN

#define Dll_EXPORTS

#ifdef Dll_EXPORTS
#define DllAPI __declspec(dllexport)
#else
#define DllAPI __declspec(dllimport)

#endif

#define EXTERNC extern "C"
#define CALL __stdcall //__cdecl

EXTERNC DllAPI DWORD CALL OpenDevice(DWORD DeviceType, DWORD DeviceInd, DWORD Reserved);
EXTERNC DllAPI DWORD CALL CloseDevice(DWORD DeviceType, DWORD DeviceInd);

EXTERNC DllAPI DWORD CALL InitCAN(DWORD DeviceType, DWORD DeviceInd, DWORD CANInd, P_INIT_CONFIG pInitConfig);

EXTERNC DllAPI DWORD CALL ReadBoardInfo(DWORD DeviceType, DWORD DeviceInd, P_BOARD_INFO pInfo);
EXTERNC DllAPI DWORD CALL ReadErrInfo(DWORD DeviceType, DWORD DeviceInd, DWORD CANInd, P_ERR_INFO pErrInfo);
EXTERNC DllAPI DWORD CALL ReadCANStatus(DWORD DeviceType, DWORD DeviceInd, DWORD CANInd, P_CAN_STATUS pCANStatus);

EXTERNC DllAPI DWORD CALL GetReference(DWORD DeviceType, DWORD DeviceInd, DWORD CANInd, DWORD RefType, PVOID pData);
EXTERNC DllAPI DWORD CALL SetReference(DWORD DeviceType, DWORD DeviceInd, DWORD CANInd, DWORD RefType, PVOID pData);

EXTERNC DllAPI ULONG CALL GetReceiveNum(DWORD DeviceType, DWORD DeviceInd, DWORD CANInd);
EXTERNC DllAPI DWORD CALL ClearBuffer(DWORD DeviceType, DWORD DeviceInd, DWORD CANInd);

EXTERNC DllAPI DWORD CALL StartCAN(DWORD DeviceType, DWORD DeviceInd, DWORD CANInd);
EXTERNC DllAPI DWORD CALL ResetCAN(DWORD DeviceType, DWORD DeviceInd, DWORD CANInd);

EXTERNC DllAPI ULONG CALL Transmit(DWORD DeviceType, DWORD DeviceInd, DWORD CANInd, P_CAN_OBJ pSend, ULONG Len);
EXTERNC DllAPI ULONG CALL Receive(DWORD DeviceType, DWORD DeviceInd, DWORD CANInd, P_CAN_OBJ pReceive, ULONG Len, INT WaitTime);

#endif

#endif
