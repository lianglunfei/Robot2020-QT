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
    unsigned char receivedCanData[NODE_NUM*8]={0};//96+16
    bool isConnected=false;
    int dataLen[50]={0};
    int id[50]={0};
    unsigned char data[50][8]={{0}};
    int len = DataTransmission::CANReceive(CONNECT_TYPE_ALYSIST, GlobalData::currentCanData, dataLen, id, data);
    for(int i=0;i<len;i++) {
        Protocol::getRawData(data[i], receivedCanData, dataLen[i], id[i]);
    }
    for (int leg = 0; leg < NODE_NUM; leg++)
    { //New add two wheel 12+2
        //data:26144/360=728.18
        GlobalData::currentCanAnalyticalData[leg].position = Protocol::parsePos(receivedCanData, leg);
        GlobalData::currentCanAnalyticalData[leg].speed = Protocol::parseSpeed(receivedCanData, leg);
        GlobalData::currentCanAnalyticalData[leg].current = Protocol::parseCurrent(receivedCanData, leg);
    }
    if ((receivedCanData[1] + receivedCanData[2] + receivedCanData[3]) != 0)
    {
        isConnected = true;
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
    if(!GlobalData::connectType)
        return false;
    unsigned char packData[8]={0};
    switch (type) {
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
    DataTransmission::CANTransmit(CONNECT_TYPE_ALYSIST, packData, id);
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
    if(!GlobalData::connectType)
        return false;
    unsigned char packData[num][8]={0};
    for(int i=0;i<num;i++) {
        switch (type) {
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
    DataTransmission::CANTransmitMulti(CONNECT_TYPE_ALYSIST, packData, (int*)id, num);
}
