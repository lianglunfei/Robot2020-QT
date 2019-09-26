#ifndef GLOBALDATA_H
#define GLOBALDATA_H

#include <QStringList>
#include <QMutex>

typedef struct
{
    double position;
    double speed;
    double current;
} CanAnalysis;

enum {
    PROTOCOL_TYPE_POS=1, PROTOCOL_TYPE_SYNC_POS, PROTOCOL_TYPE_INTER_POS,
    PROTOCOL_TYPE_SPD, PROTOCOL_TYPE_CUR, PROTOCOL_TYPE_START,
    PROTOCOL_TYPE_OPENVALVE, PROTOCOL_TYPE_CLOSEVALVE,
    PROTOCOL_TYPE_CAL, PROTOCOL_TYPE_SHUTDOWN, PROTOCOL_TYPE_PAUSE,
    PROTOCOL_TYPE_SPD_SET, PROTOCOL_TYPE_POS_RF,
    PROTOCOL_TYPE_WHELL_RESET, PROTOCOL_TYPE_WHELL_SELECT,
    PROTOCOL_TYPE_WHELL_SPD, PROTOCOL_TYPE_WHELL_REF, PROTOCOL_TYPE_SPD_PAW
};

#define NONE_CONNECT            0
#define CONNECT_TYPE_ALYSIST    1
#define CONNECT_TYPE_GC         2

#define NODE_NUM 3
#define DRIVE_NEW 1
#define SIMULATE_CONNECT        /*NONE_CONNECT//*/CONNECT_TYPE_ALYSIST

#define global GlobalData::getInstance()

class GlobalData
{
private:
    GlobalData();

    static GlobalData* instance;
    static QMutex mutex;

public:
    CanAnalysis currentCanAnalyticalData[NODE_NUM];
    QStringList currentCanData;
    int runningId[NODE_NUM]={0};
    int statusId[NODE_NUM]={0};
    int connectType=0;
    unsigned int sendId[NODE_NUM]={0};
    bool showDebugInfo=false;

    class Garbo
    {
    public:
        ~Garbo() {
            if(GlobalData::instance) {
                delete GlobalData::instance;
                GlobalData::instance=nullptr;
            }
        }
    };

    static GlobalData* getInstance() {
        if(instance==nullptr) {
            mutex.lock();
            if(instance==nullptr) {
                instance = new GlobalData();
                static Garbo gb;
            }
            mutex.unlock();
        }
        return instance;
    }
};

#endif // GLOBALDATA_H
