/*
 * @Descripttion: 
 * @version: 
 * @Author: Qingmao Wei
 * @Date: 2020-01-14 15:19:54
 * @LastEditors  : Qingmao Wei
 * @LastEditTime : 2020-01-15 16:37:36
 */
#include "sequencetableview.h"
#include "controltableview.h"
#include "globaldata.h"
#include "package.h"
#include "incompletecombox.h"
#include "debug.h"

#include <QHeaderView>
#include <QPushButton>
#include <QTimer>
#include <QThread>
#include <QCheckBox>
#include <QMessageBox>
#include <QCoreApplication>
#include <QTime>

#define ROW_BTN_NUM 5        // 按钮个数：上下移动、增加删除、运行
#define ACTION_COMBO_INDEX 0 // 行动作名所在的位置=0
#define BTN_START_INDEX 1    // 行按钮开始的位置=1
#define CHECKBOX_INDEX 6     // 选中按钮的位置=6

SequenceTableView::SequenceTableView(QWidget *parent) : QTableView(parent)
{
    seqWorker = &SequenceExcuteWorker::getInstance();
    interPeriod = 50;
    interValue = 1;
    currentRunRow = 0;
    continuedRunning = false;

    headerDataInit();
    modelInit();
    tableViewInit();
    addTableviewRow(0, 0, true);
    horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    connect(seqWorker, &SequenceExcuteWorker::execStatus, this, &SequenceTableView::runStatus);
    connect(seqWorker, &SequenceExcuteWorker::stopThread, this, &SequenceTableView::stopStatus);
}

SequenceTableView::~SequenceTableView()
{
    delete model;
}

/**
 * @brief 表头初始化，与BTN_START_INDEX、ROW_BTN_NUM、BEFORE_VALUE_NUM有关联
 * | action | run | add | del | up | down | select |
 * 6 cols
 */
void SequenceTableView::headerDataInit()
{
    headerData.clear();
    headerData << QObject::tr("Action") << QObject::tr("Run") << QObject::tr("Add")
               << QObject::tr("Del") << QObject::tr("Up") << QObject::tr("Down")
               << QObject::tr("");
}

/**
 * @brief valueList为初始化表中行时的数据来源
 * 
 */
void SequenceTableView::valueListInit()
{
    valueList.clear();
    // valueList << QObject::tr("H10");
    // valueList << QObject::tr("");
}

/**
 * @brief 数据模型初始化
 * 
 */
void SequenceTableView::modelInit()
{
    model = new QStandardItemModel();
    model->setColumnCount(headerData.length());
    for (int i = 0; i < headerData.length(); i++)
    {
        model->setHeaderData(i, Qt::Horizontal, headerData[i]);
    }
}

/**
 * @brief 表的初始化，绑定对应的model
 * 
 */
void SequenceTableView::tableViewInit()
{
    resizeColumnsToContents();
    setAlternatingRowColors(true);
    setSelectionBehavior(QAbstractItemView::SelectRows);
    //如果你用在QTableView中使用右键菜单，需启用该属性
    setContextMenuPolicy(Qt::CustomContextMenu);
    // horizontalHeader()->setFont(QFont("Times", 10, QFont::Bold));
    setModel(model);
}

// /**
//  * @brief: public 设置表模型的comboBox列表
//  * @param actions 要设置的QStingList
//  */
// void SequenceTableView::setActionList(const QStringList &actions)
// {
//     actionList = actions;
// }

/**
*@brief         表中新增一行widget，通过调用对应的model重新设置tableview完成
**/
void SequenceTableView::addTableviewRowWidget(int mode, int row, bool checkState, bool complete)
{
    //为这个第2列添加下拉框
    IncompleteCombox *m_combox = new IncompleteCombox();
    m_combox->addItems(actionList);
    m_combox->setProperty("row", row); //为按钮设置row属性
    m_combox->setParent(this);
    m_combox->setEditable(true);
    connect(this, &setActionList, m_combox, &IncompleteCombox::addItems);
    setIndexWidget(model->index(row, 0), m_combox);
    if (complete)
    {
        //为这个第i列添加按钮
        QPushButton *m_button[ROW_BTN_NUM];
        QStringList iconPath;
        iconPath << tr(":/images/go.png") << tr(":/images/add.png")
                 << tr(":/images/delete.png") << tr(":/images/up.png")
                 << tr(":/images/down.png");
        for (int i = 0; i < ROW_BTN_NUM; i++)
        {
            m_button[i] = new QPushButton();
            m_button[i]->setIcon(QIcon(iconPath[i]));
            connect(m_button[i], SIGNAL(clicked(bool)), this, SLOT(tableClickButton()));
            m_button[i]->setProperty("row", row);      //为按钮设置row属性
            m_button[i]->setProperty("column", i + 1); //为按钮设置column属性
            m_button[i]->setParent(this);
            setIndexWidget(model->index(row, i + 1), m_button[i]);
        }
    }

    QCheckBox *m_checkBox = new QCheckBox();
    if (checkState)
        m_checkBox->setChecked(true);
    else
        m_checkBox->setChecked(false);
    m_checkBox->setProperty("row", row); //为按钮设置row属性
    m_checkBox->setParent(this);
    setIndexWidget(model->index(row, ROW_BTN_NUM + 1), m_checkBox);
}

/**
*@projectName   RobotControlSystem
*@brief         表中widget对应的事件函数
*@parameter
*@author        XingZhang.Wu
*@date          20190805
**/
void SequenceTableView::tableClickButton()
{
    QPushButton *btn = (QPushButton *)sender(); //产生事件的对象
    int row = btn->property("row").toInt();     //取得按钮的行号属性
    int col = btn->property("column").toInt();  //取得按钮的列号属性
    qDebug() << "col=" << col << ", row=" << row;
    switch (col)
    {
    case BTN_START_INDEX: //run
        // qDebug()<<qobject_cast<IncompleteCombox *>(indexWidget(model->index(row, ACTION_COMBO_INDEX)))->currentText();
        if (!execOneAction(qobject_cast<IncompleteCombox *>(indexWidget(model->index(row, ACTION_COMBO_INDEX)))->currentText()))
        {
            currentRunRow = row;
            qobject_cast<QPushButton *>(indexWidget(model->index(row, BTN_START_INDEX)))->setEnabled(0);
        }
        break;
    case BTN_START_INDEX + 1: //add
        model->insertRow(row + 1);
        addTableviewRow(0, row + 1, true);
        updateTablePropertyAfterLine(row + 1, 0);
        break;
    case BTN_START_INDEX + 2: //delete
        model->removeRow(row);
        updateTablePropertyAfterLine(row, 0);
        break;
    case BTN_START_INDEX + 3: //up
        // if (row > 1)
        // {
        //     int modeBak = qobject_cast<IncompleteCombox *>(indexWidget(model->index(row, 1)))->currentIndex();
        //     bool checkBak = qobject_cast<QCheckBox *>(indexWidget(model->index(row, BTN_START_INDEX + 5)))->isChecked();
        //     QList<QStandardItem *> listItem = model->takeRow(row);
        //     model->insertRow(row - 1, listItem);
        //     addTableviewRowWidget(modeBak, row - 1, checkBak, true);
        //     updateTableRowProperty(row - 1, row - 1);
        //     updateTableRowProperty(row, row);
        // }
        break;
    case BTN_START_INDEX + 4: //down
        // if (row < model->rowCount() - 1)
        // {
        //     int modeBak = qobject_cast<IncompleteCombox *>(indexWidget(model->index(row, 1)))->currentIndex();
        //     bool checkBak = qobject_cast<QCheckBox *>(indexWidget(model->index(row, BTN_START_INDEX + 5)))->isChecked();
        //     QList<QStandardItem *> listItem = model->takeRow(row);
        //     model->insertRow(row + 1, listItem);
        //     addTableviewRowWidget(modeBak, row + 1, checkBak, true);
        //     updateTableRowProperty(row + 1, row + 1);
        //     updateTableRowProperty(row, row);
        // }
        break;
    default:
        break;
    }
}

/**
*@projectName   RobotControlSystem
*@brief         更新表中对应的widget属性值
*@parameter
*@author        XingZhang.Wu
*@date          20190805
**/
void SequenceTableView::updateTableRowProperty(int row, int property)
{
    // int len = valueList.length();
    int len = 0;
    qobject_cast<IncompleteCombox *>(indexWidget(model->index(row, len++)))->setProperty("row", property);
    qobject_cast<QPushButton *>(indexWidget(model->index(row, len++)))->setProperty("row", property);
    qobject_cast<QPushButton *>(indexWidget(model->index(row, len++)))->setProperty("row", property);
    qobject_cast<QPushButton *>(indexWidget(model->index(row, len++)))->setProperty("row", property);
    qobject_cast<QPushButton *>(indexWidget(model->index(row, len++)))->setProperty("row", property);
    qobject_cast<QPushButton *>(indexWidget(model->index(row, len++)))->setProperty("row", property);
    qobject_cast<QCheckBox *>(indexWidget(model->index(row, len++)))->setProperty("row", property);
}

void SequenceTableView::updateTablePropertyAfterLine(int row, int offset)
{
    qDebug() << "model->rowCount()=" << model->rowCount();
    for (int i = row; i < model->rowCount(); i++)
    {
        qDebug() << i;
        updateTableRowProperty(i, i + offset);
    }
}

/**
 * @brief 从表格更新valueList
 * 
 * @param row 
 */
void SequenceTableView::valueListUpdate(int row)
{
    // valueList.clear();
    // valueList << model->index(row, 0).data().toString();
    // valueList << model->index(row, 1).data().toString();
}

/**
*@projectName   RobotControlSystem
*@brief         表中新增一行，包括数据和widget
*@parameter
*@author        XingZhang.Wu
*@date          20190805
**/
void SequenceTableView::addTableviewRow(int mode, int row, bool hasWidget)
{
    addModelItemData(row);
    if (hasWidget)
        addTableviewRowWidget(mode, row, true, true);
}

/**
*@projectName   RobotControlSystem
*@brief         表中新增一行，通过重新设置model完成
*@parameter
*@author        XingZhang.Wu
*@date          20190805
**/
void SequenceTableView::addModelItemData(int row)
{
    for (int i = 0; i < 1 /* valueList.length() */; i++)
    {
        if (i == 0)
            model->setItem(row, i, new QStandardItem(1));
        else
            model->setItem(row, i + 1, new QStandardItem(valueList.at(i)));
    }
    //设置字符颜色
    // model->item(row, 0)->setForeground(QBrush(QColor(255, 0, 0)));
}

/**
 * @brief: 加载并执行一个动作
 * @param fine_name 动作的文件名,无后缀
 * @return: csv格式正确返回0, 失败返回-1.
 */
int SequenceTableView::execOneAction(QString fine_base_name)
{
    qDebug() << actionModels[fine_base_name];
    if (seqWorker->importCSV(actionModels[fine_base_name]))
    {
        QMessageBox::warning(this, tr("CSV error"),
                             tr("格式不正确。"),
                             QMessageBox::Ok | QMessageBox::Cancel);
        return -1;
    }
    seqWorker->seqExec(false, interValue, interPeriod);
    return 0;
}

void SequenceTableView::invokeNextAction()
{
    continuedRunning = 1;
    //恢复上一个按钮
    
    if (!execOneAction(qobject_cast<IncompleteCombox *>(indexWidget(model->index(currentRunRow, ACTION_COMBO_INDEX)))->currentText()))
    {
        qobject_cast<QPushButton *>(indexWidget(model->index(currentRunRow, BTN_START_INDEX)))->setEnabled(0);
    }
    // 查找下一个选中的条目
    do{
        currentRunRow++;
        // 如果超出最后一行还没找到
        if(currentRunRow >= model->rowCount())
        {
            continuedRunning = 0;
            currentRunRow--;
            return;
        }
    }while (!qobject_cast<QCheckBox *>(indexWidget(model->index(currentRunRow, CHECKBOX_INDEX)))->isEnabled());
        
    qDebug()<<currentRunRow<<" "<<model->rowCount();
    
}

/**
 * @brief: slot (外部UI)设置间隔时间和间隔次数
 * @param interValue 间隔次数
 * @param interPeriod 间隔时间
 */
void SequenceTableView::setInterValue(int interValue_)
{
    interValue = interValue_;
}
void SequenceTableView::setInterPeriod(int interPeriod_)
{
    interPeriod = interPeriod_;
}
void SequenceTableView::manualStop()
{
    continuedRunning = 0;
}

void SequenceTableView::setActionModels(QMap<QString, QString> actionModels_)
{
    actionModels = actionModels_;
}

/**
 * @brief 仿真数据运行状态显示
 * 
 * @param s 
 */
void SequenceTableView::runStatus(QString s)
{
}

/**
 * @brief 停止仿真时设置相关状态
 * 
 */
void SequenceTableView::stopStatus()
{
    
    if (!continuedRunning){
        qobject_cast<QPushButton *>(indexWidget(model->index(currentRunRow, BTN_START_INDEX)))->setEnabled(1);
        currentRunRow = 0;
    }
        
    else{
        if(currentRunRow!=0)
            qobject_cast<QPushButton *>(indexWidget(model->index(currentRunRow-1, BTN_START_INDEX)))->setEnabled(1);
        invokeNextAction();
    }
        
    // ui->messageLabel->setText("执行结束。");
}