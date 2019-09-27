#include "package.h"
#include "globaldata.h"
#include "datatransmission.h"
#include "protocol.h"

#include "debug.h"

Package::Package()
{
}

/**
*@projectName   RobotControlSystem
*@brief         解读从CAN卡读到的数据，并更加电机协议换算成电机电流、速度、角度信息。
*@parameter
*@author        XingZhang.Wu
*@date          20190723
**/
bool Package::unpackOperate()
{
    static unsigned char receivedCanData[NODE_NUM * 8] = {0}; //96+16
    bool isConnected = false;
    int dataLen[50] = {0};
    int id[50] = {0};
    unsigned char data[50][8] = {{0}};
    int len = DataTransmission::CANReceive(global->connectType, global->currentCanData, dataLen, id, data);
    for (int i = 0; i < len; i++)
    {
        Protocol::getRawData(data[i], receivedCanData, dataLen[i], id[i]);
    }
    for (int leg = 0; leg < NODE_NUM; leg++)
    { //New add two wheel 12+2
        //data:26144/360=728.18
        global->currentCanAnalyticalData[leg].position = Protocol::parsePos(receivedCanData, leg);
        global->currentCanAnalyticalData[leg].speed = Protocol::parseSpeed(receivedCanData, leg);
        global->currentCanAnalyticalData[leg].current = Protocol::parseCurrent(receivedCanData, leg);
    }
    if ((data[0][1] + data[1][2] + data[2][3]) != 0)
    {
        isConnected = true;
    }
    int nodeId[NODE_NUM] = {0};
    int nodeStatus[NODE_NUM] = {0};
    memset(nodeStatus, -1, sizeof(nodeStatus)); //全部初始化为-1

    for (int i = 0; i < 50; i++)
    {
        if (id[i] > 0)
        {
            nodeId[id[i] - global->sendId[0]] = 1; //该Id接收到了数据
            nodeStatus[id[i] - global->sendId[0]] = data[i][0];
        }
    }
    for (int i = 0; i < NODE_NUM; i++)
    {
        global->runningId[i] = nodeId[i];
        global->statusId[i] = nodeStatus[i];
    }
    return isConnected;
}

/**
*@projectName   RobotControlSystem
*@brief         以速度、角度、位置信息将数据进行打包，然后再调用底层发送接口传递给CAN。
*@parameter
*@author        XingZhang.Wu
*@date          20190723
**/
bool Package::packOperate(unsigned int id, double data, int type)
{
    if (!global->connectType)
        return false;
    unsigned char packData[8] = {0};
    switch (type)
    {
    case PROTOCOL_TYPE_POS:
        Protocol::packPos(packData, data);
        break;
    case PROTOCOL_TYPE_POS_RF:
        Protocol::packPosRF(packData, data);
        break;
    case PROTOCOL_TYPE_SPD:
        Protocol::packSpeed(packData, data);
        break;
    case PROTOCOL_TYPE_CAL:
        Protocol::packCal(packData, data);
        break;
    case PROTOCOL_TYPE_START:
        Protocol::packStart(packData, data);
        break;
    case PROTOCOL_TYPE_SHUTDOWN:
        Protocol::packShutDown(packData, data);
        break;
    case PROTOCOL_TYPE_WHELL_RESET:
        Protocol::packWheelReset(packData, data);
        break;
    case PROTOCOL_TYPE_WHELL_SELECT:
        Protocol::packWheelSelect(packData, data);
        break;
    case PROTOCOL_TYPE_WHELL_SPD:
        Protocol::packWheelSpeed(packData, data);
        break;
    case PROTOCOL_TYPE_WHELL_REF:
        Protocol::packWheelRef(packData, data);
        break;
    case PROTOCOL_TYPE_SPD_PAW:
        Protocol::packSpdPaw(packData, data);
        break;
    case PROTOCOL_TYPE_OPENVALVE:
        Protocol::packOpenValve(packData, data);
        break;
    case PROTOCOL_TYPE_CLOSEVALVE:
        Protocol::packCloseValve(packData, data);
        break;
    default:
        break;
    }
    DataTransmission::CANTransmit(global->connectType, packData, id);
    return true;
}

/**
*@projectName   RobotControlSystem
*@brief         可以同时打包多帧数据，直接调用底层发送多帧的CAN接口，减少各帧数据发送的延时。
*@parameter
*@author        XingZhang.Wu
*@date          20190723
**/
bool Package::packOperateMulti(unsigned int *id, double *data, int num, int type)
{
    if (!global->connectType)
        return false;
    unsigned char packData[num][8] = {0};
    for (int i = 0; i < num; i++)
    {
        switch (type)
        {
        case PROTOCOL_TYPE_POS:
            Protocol::packPos(packData[i], data[i]);
            break;
        case PROTOCOL_TYPE_SPD:
            Protocol::packSpeed(packData[i], data[i]);
            break;
        case PROTOCOL_TYPE_SPD_SET:
            Protocol::packSpeedSet(packData[i], data[i]);
            break;
        case PROTOCOL_TYPE_START:
            Protocol::packStart(packData[i], data[i]);
            break;
        case PROTOCOL_TYPE_PAUSE:
            Protocol::packPause(packData[i], data[i]);
            break;
        case PROTOCOL_TYPE_SHUTDOWN:
            Protocol::packShutDown(packData[i], data[i]);
            break;
        case PROTOCOL_TYPE_OPENVALVE:
            Protocol::packOpenValve(packData[i], data[i]);
            break;
        case PROTOCOL_TYPE_CLOSEVALVE:
            Protocol::packCloseValve(packData[i], data[i]);
            break;
        default:
            break;
        }
    }
    return DataTransmission::CANTransmitMulti(global->connectType, packData, (int *)id, num);
}
