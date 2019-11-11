/*
 * @Author: xingzhang.Wu 
 * @Date: 2019-09-29 10:02:20 
 * @Last Modified by: xingzhang.Wu
 * @Last Modified time: 2019-11-11 19:05:31
 */
#ifndef GLOBALDATA_H
#define GLOBALDATA_H

#include <QStringList>
#include <QMutex>

//CAN解析后的数据
typedef struct
{
    double position;
    double speed;
    double current;
} CanAnalysis;

//接收到导航的数据
typedef struct
{
    double x;
    double y;
    double z;
} Navigation;

// 数字枚举变量
enum
{
    PROTOCOL_TYPE_POS = 1,
    PROTOCOL_TYPE_SYNC_POS,
    PROTOCOL_TYPE_INTER_POS,
    PROTOCOL_TYPE_SPD,
    PROTOCOL_TYPE_CUR,
    PROTOCOL_TYPE_START,
    PROTOCOL_TYPE_OPENVALVE,
    PROTOCOL_TYPE_CLOSEVALVE,
    PROTOCOL_TYPE_CAL,
    PROTOCOL_TYPE_SHUTDOWN,
    PROTOCOL_TYPE_PAUSE,
    PROTOCOL_TYPE_SPD_SET,
    PROTOCOL_TYPE_POS_RF,
    PROTOCOL_TYPE_WHELL_RESET,
    PROTOCOL_TYPE_WHELL_SELECT,
    PROTOCOL_TYPE_WHELL_SPD,
    PROTOCOL_TYPE_WHELL_REF,
    PROTOCOL_TYPE_SPD_PAW
};

#define NONE_CONNECT 0
#define CONNECT_TYPE_ALYSIST 1
#define CONNECT_TYPE_GC 2

//开关变量
#define USE_ZLG
// #define DELAY_INIT 5

//配置变量
#define NODE_NUM 12
#define DRIVE_NEW 1
#define CAN_MAX_FRAM 100
#define SIMULATE_CONNECT NONE_CONNECT //CONNECT_TYPE_ALYSIST

#define global GlobalData::getInstance()

class GlobalData
{
private:
    GlobalData();

    static GlobalData *instance;
    static QMutex mutex;

public:
    CanAnalysis currentCanAnalyticalData[NODE_NUM]; //关节CAN解析后的数据
    QStringList currentCanData;                     //当前CAN原始数据字符串链表
    //参考位置，注意和csv文件保持一致，在自动控制类种被调用
    double refValue[12] = {99.052, 160.415, 88.0911, 329.31, 184.486, 148.467, 141.212, 261.981, 30.9121, 222, 250.584, 147.704};
    int runningId[NODE_NUM] = {0};       //当前能接收到CAN数据的节点ID
    int statusId[NODE_NUM] = {0};        //节点的状态位
    int connectType = 0;                 //CAN连接类型
    Navigation navigateData;             //接收到的导航目标数据
    unsigned int sendId[NODE_NUM] = {0}; //CAN 节点ID，在构造函数中进行初始化
    bool showDebugInfo = false;

    class Garbo
    {
    public:
        ~Garbo()
        {
            if (GlobalData::instance)
            {
                delete GlobalData::instance; //程序退出时，能够自动释放指针
                GlobalData::instance = nullptr;
            }
        }
    };

    static GlobalData *getInstance()
    {
        if (instance == nullptr)
        {
            mutex.lock();
            if (instance == nullptr) //双重互斥锁，保证不被实例化两次
            {
                instance = new GlobalData();
                static Garbo gb;
            }
            mutex.unlock();
        }
        return instance;
    }
};

#endif // GLOBALDATA_H
