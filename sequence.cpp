/*
 * @Descripttion: 
 * @version: 
 * @Author: xingzhang.Wu
 * @Date: 2020-01-13 10:18:42
 * @LastEditors  : Qingmao Wei
 * @LastEditTime : 2020-01-15 16:14:56
 */
#include "sequence.h"
#include "globaldata.h"
#include "debug.h"
#include "package.h"
// #include "incompletecombox.h"


#include <QTimer>
#include <QThread>
#include <QTime>
#include <QFile>


#define BTN_START_INDEX NODE_NUM + 3 //行按钮开始的位置=节点数+mode+time+name
#define ROW_BTN_NUM 5                //按钮个数：上下移动、增加删除、运行
#define BEFORE_VALUE_NUM 2           //节点数之前的数值个数：name、mode
#define POS_LIMIT_VALUE 361          //最大允许位置变化偏差，超过会显示数据异常
#define SHOW_BTN_NUM 50              //表格显示按钮的行数，一般设定前50行s

SequenceExcuteWorker::SequenceExcuteWorker()
{
    headerDataInit();
    eventInit();
    modelInit();  
}
SequenceExcuteWorker::~SequenceExcuteWorker()
{
    qDebug()<<"--------------Delete-------------";
    delete taskTimer;
    delete taskThread;
    delete seqModel;
}



void SequenceExcuteWorker::headerDataInit()
{
    headerData.clear();
    headerData << QObject::tr("Name") << QObject::tr("Mode");
    for (int i = 0; i < NODE_NUM; i++)
    {
        headerData << QString("Value%1").arg(i + 1);
    }
    headerData << QObject::tr("Time") << QObject::tr("Run") << QObject::tr("Add")
               << QObject::tr("Delete") << QObject::tr("Up") << QObject::tr("Down")
               << QObject::tr("Select");
}

int SequenceExcuteWorker::eventInit()
{
    taskTimer = new QTimer;
    taskThread = new QThread;
    taskTimer->setTimerType(Qt::PreciseTimer);
    taskTimer->setInterval(10);
    taskTimer->moveToThread(taskThread);
    connect(taskTimer, SIGNAL(timeout()), this, SLOT(execSeqEvent()), Qt::DirectConnection);
    connect(this, SIGNAL(stopThread()), taskThread, SLOT(quit()));
    return 0;
}

int SequenceExcuteWorker::modelInit()
{
    seqModel = new QStandardItemModel();
    seqModel->setColumnCount(headerData.length());
    for (int i = 0; i < headerData.length(); i++)
    {
        seqModel->setHeaderData(i, Qt::Horizontal, headerData[i]);
    }
    return 0;
}


/**
*@projectName   RobotControlSystem
*@brief         获取表中某一行的数据
*@parameter
*@author        XingZhang.Wu
*@date          20190805
**/
void SequenceExcuteWorker::getModelRowValue(double *value, int row, int len)
{
    for (int i = BEFORE_VALUE_NUM; i < len + BEFORE_VALUE_NUM; i++)
    {
        value[i - BEFORE_VALUE_NUM] = seqModel->index(row, i).data().toDouble();
    }
}


/**
 * @brief: 判断model中的某一行的执行类型(相对,绝对), 据此执行该行数据
 * @param row 行数
 * @return: 执行成功返回0
 */
int SequenceExcuteWorker::runFunc(int row)
{
    double value[NODE_NUM];
    double refValue[NODE_NUM];
    getModelRowValue(refValue, 0, NODE_NUM);
    getModelRowValue(value, row, NODE_NUM);

    if (0 == seqModel->index(row, 1).data().toInt())
    {
        Package::packOperateMulti(globalData->sendId, value, NODE_NUM, PROTOCOL_TYPE_SPD);
    }
    else if (1 == seqModel->index(row, 1).data().toInt())
    {
        // qDebug()<<"hello step "<<row;
        // qDebug()<<
        for (int i = 0; i < NODE_NUM; i++) {
            value[i] += refValue[i];
            double realVal = 0;
            if (std::abs(globalData->currentCanAnalyticalData[i].position - value[i]) > 180)
                realVal = 360 - std::abs(globalData->currentCanAnalyticalData[i].position - value[i]);
            else
                realVal = std::abs(globalData->currentCanAnalyticalData[i].position - value[i]);
            if (realVal > POS_LIMIT_VALUE){
                qDebug()<<realVal;
                return -1;
            }
                
        }
        //In order to be able to reach the initial position, set this mode here.
        Package::packOperateMulti(globalData->sendId, value, NODE_NUM, PROTOCOL_TYPE_POS);
    }
    else
    { //relative position mode
        for (int i = 0; i < NODE_NUM; i++)
        {
            value[i] += globalData->currentCanAnalyticalData[i].position;
        }
        Package::packOperateMulti(globalData->sendId, value, NODE_NUM, PROTOCOL_TYPE_POS);
    }
    return 0;
}


/**
*@projectName   RobotControlSystem
*@brief         返回1表示执行正在进行，返回2表示执行暂停
*@parameter
*@author        XingZhang.Wu
*@date          20190806
**/
int SequenceExcuteWorker::seqExec(bool cycle, int value, int period)
{
    cycleFlag = cycle;
    interValue = value;
    interPeriod = period;

    if (taskThread->isRunning())
    {
        if (execRunOrPauseFlag == 12)
        {
            execRunOrPauseFlag = 11;
            return 1;
        }
        else
        {
            execRunOrPauseFlag = 12;
            return 2;
        }
    }
    else if (seqModel->rowCount() > 1)
    { //首次运行
        execRunOrPauseFlag = 11;
        QTimer::singleShot(0, taskTimer, SLOT(start()));
        taskThread->start();
        return 1;
    }
    return 0;
}

/**
 * @brief: 逆序执行
 * @param {type} 
 * @return: 返回1表示执行正在进行，返回2表示执行暂停
 */
int SequenceExcuteWorker::reverseSeqExec(bool cycle, int value, int period)
{
    cycleFlag = cycle;
    interValue = value;
    interPeriod = period;

    if (taskThread->isRunning())
    {
        if (execRunOrPauseFlag == 22)
        {
            execRunOrPauseFlag = 21;
            return 1;
        }
        else
        {
            execRunOrPauseFlag = 22;
            return 2;
        }
    }
    else if (seqModel->rowCount() > 1)
    { //首次运行
        execRunOrPauseFlag = 21;
        QTimer::singleShot(0, taskTimer, SLOT(start()));
        taskThread->start();
        return 1;
    }
    return 0;
}

int SequenceExcuteWorker::execPause()
{
    if (taskThread->isRunning())
    {
        if (execRunOrPauseFlag % 10 == 1)
        { //正在运行，则加1变为暂停状态
            execRunOrPauseFlag++;
        }
        return execRunOrPauseFlag / 10; //返回当前是顺序运行还是逆序运行状态
    }
    return 0;
}

void SequenceExcuteWorker::execStop()
{
    execRunOrPauseFlag = 3;
}

/**
*@projectName   RobotControlSystem
*@brief         重点部分，顺序或逆序执行表格命令，也可以定时执行
*@parameter
*@author        XingZhang.Wu
*@date          20190809
**/
void SequenceExcuteWorker::execSeqEvent()
{
    static double refValue[NODE_NUM];
    static int row = -1;
    static int timeCnt = 1;
    static int lastRow[2] = {-1, -1}; //0:lastLast 1:last
    static int g_lastRow = -1;
    static int listHead, listHeadBak, listTail;
    static int groupCnt = 1;
    static QTime time;

    //init row
    if (-1 == row)
    { 
        time.start();
        groupCnt = 1;
        listHeadBak = 1;
        listTail = seqModel->rowCount() - 1;
        // setListBoundaryValue(listHeadBak, listTail);
        listHead = listHeadBak;
        if (execRunOrPauseFlag / 10 == 2)
            row = listTail;
        else
            row = listHead;
        getModelRowValue(refValue, 0, NODE_NUM);
    }
    //顺序执行越界处理
    if (row > listTail)
    { 
        if (cycleFlag)
        {
            groupCnt++;
            row = listHead;
            lastRow[0] = -1;
            lastRow[1] = -1;
            g_lastRow = -1;
        }
        else
        {
            taskTimer->stop();
            row = -1; //顺序执行结束
            lastRow[0] = -1;
            lastRow[1] = -1;
            g_lastRow = -1;
            execRunOrPauseFlag = 0;
            emit stopThread();
            return;
        }
    }
    //逆序执行越界处理
    if (row < listHead)
    { 
        if (cycleFlag)
        {
            groupCnt++;
            row = listTail;
            lastRow[0] = -1;
            lastRow[1] = -1;
            g_lastRow = -1;
        }
        else
        {
            taskTimer->stop();
            row = -1; //逆序执行结束
            lastRow[0] = -1;
            lastRow[1] = -1;
            g_lastRow = -1;
            execRunOrPauseFlag = 0;
            emit stopThread();
            return;
        }
    }
    //手动暂停
    if (2 == execRunOrPauseFlag % 10)
    { 
        return;
    }
    //手动结束
    else if (execRunOrPauseFlag % 10 > 2)
    { 
        taskTimer->stop();
        execRunOrPauseFlag = 0;
        row = -1;
        lastRow[0] = -1;
        lastRow[1] = -1;
        g_lastRow = -1;
        emit stopThread();
        return;
    }

    if (-1 == g_lastRow)
    {
        g_lastRow = row;
    }

    if (row % interValue == 0)
    {
        int runTime = interPeriod;
        if (execRunOrPauseFlag / 10 == 1)
        { //顺序执行,采用上一行命令的时间
            if (row >= listHead)
            { //1-[T2-2]-[T3-3]  T1可以设置的比较小  P1-T2-P2,其中T2指的是P1转到P2的时间
                if (runTime == 0)
                    runTime = static_cast<int>(seqModel->index(g_lastRow, NODE_NUM + BEFORE_VALUE_NUM).data().toDouble() + 0.5);

                if (timeCnt < runTime / 10)
                { //time must be an integer multiple of 10
                    timeCnt++;
                    return;
                }
            }
            else
            {
                runTime = 0;
            }
        }
        else
        { //逆序执行,采用上一行命令的时间
            if (row <= listTail)
            { //3-[T3-2]-[T2-1] 其中T3代表P2转到P3的时间
                if (runTime == 0)
                {
                    runTime = static_cast<int>(seqModel->index(lastRow[0], NODE_NUM + BEFORE_VALUE_NUM).data().toDouble() + 0.5);
                    // //如果上次运行的为速度，则只间隔上次速度所需时间再执行位置
                    // if (0 == qobject_cast<IncompleteCombox *>(indexWidget(seqModel->index(g_lastRow, 1)))->currentIndex())
                    // {
                    //     runTime = static_cast<int>(seqModel->index(g_lastRow, NODE_NUM + BEFORE_VALUE_NUM).data().toDouble() + 0.5);
                    // }
                }

                if (timeCnt < runTime / 10)
                { //time must be an integer multiple of 10
                    timeCnt++;
                    return;
                }
            }
            else
            {
                runTime = 0;
            }
        }
        emit execStatus(seqModel->index(g_lastRow, 0).data().toString() + QString(" has runned for %1 ms\n").arg(timeCnt * 10) + QString("group %2: ").arg(groupCnt) + seqModel->index(row, 0).data().toString() + QString(" is running\n") + QString("elapsed time: %1 s").arg(time.elapsed() / 1000));
        timeCnt = 1;

        if (runFunc(row) < 0)
        {
            taskTimer->stop();
            emit execStatus(QString("行 %1").arg(row)+ " 数据导入异常！");
            execRunOrPauseFlag = 0;
            row = -1;
            lastRow[0] = -1;
            lastRow[1] = -1;
            g_lastRow = -1;
            emit stopThread();
            return;
        }

        //逆向时位置控制时保留上个位置的时间间隔
        if (execRunOrPauseFlag / 10 == 2)
        {
            lastRow[0] = lastRow[1];
            lastRow[1] = row;
        }
        g_lastRow = row;
    }
    if (1 == execRunOrPauseFlag / 10)
        row++;
    else
        row--;
}


/**
 * @brief: 读取csv文件, 进行格式检查, 返回model
 * @param fileName 要读取的csv文件名
 * @param seqModel 返回读取的model的指针
 * @return: 状态: -1 失败; 0成功
 */
int SequenceExcuteWorker::importCSV(QString fileName)
{
    // qDebug()<<"in file.";
    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qDebug() << "Open file failed!";
        return -1;
    }
    // qDebug()<<"file opened.";

    QList<QStringList> qlist;
    QStringList strlist;
    QTextStream in(&file);
    int row = 0;
    int col = 0, colMin = 0;

    while (!in.atEnd())
    {
        QString fileLine = in.readLine();
        strlist = fileLine.split(",", QString::SkipEmptyParts);
        //        qDebug() << strlist;
        qlist.push_back(strlist);

        // 取更大值
        if (col == 0)
        {
            col = strlist.count();
        }
        else
        {
            col = col > strlist.count() ? col : strlist.count();
            colMin = col < strlist.count() ? col : strlist.count();
        }
        row++;
    }

    // qDebug()<<"read end.";
    // csv model格式检查
    if (0 == col || 0 == row)
    {
        
        file.close();
        return -1;
    }
    
    // 头两行为空
    if (qlist[0].length() == 0 && qlist[1].length() == 0){
        
        file.close();
        return -1;
    }
    // 第一行不是'ref'
    if (!QString(qlist[1][0]).contains("ref") || col != (BTN_START_INDEX + ROW_BTN_NUM + 1))
    {
        
        file.close();
        return -1;
    }

    qDebug()<<seqModel;

    seqModel->clear();
    seqModel->setRowCount(row - 1);
    seqModel->setColumnCount(col);

    //ref line
    for (int j = 0; j < NODE_NUM + 1; j++)
    {
        QStandardItem *item = new QStandardItem(qlist[1][j]);
        //这里的指针会在QStandardItemModel中的析构函数里面被释放掉 qDeleteAll(d->rowHeaderItems);
        if (j == 0)
            seqModel->setItem(0, 0, item);
        else
            seqModel->setItem(0, j + 1, item);
    }

    for (int i = 1; i < row - 1; i++) //Remove the first line header
    {
        for (int j = 0; j < col; j++) //note:21
        {   
            if (j != 1 && j < colMin)
            {
                QStandardItem *item = new QStandardItem(qlist[i + 1][j]);
                //这里的指针会在QStandardItemModel中的析构函数里面被释放掉 qDeleteAll(d->rowHeaderItems);
                seqModel->setItem(i, j, item);
            }
        }
        QStandardItem *item = new QStandardItem(qlist[i + 1][1]);
        seqModel->setItem(i+1, 1, item);
        // qDebug()<<QString(qlist[i + 1][1]).toInt();
        // qDebug()<<seqModel->index(i+1,1).data().toInt()<<"-----";
    }
    file.close();
    return 0;
}
