#include "controltableview.h"
#include "globaldata.h"
#include "package.h"
#include "incompletecombox.h"
#include "debug.h"

#include <QHeaderView>
#include <QPushButton>
#include <QCheckBox>

#define BTN_START_INDEX NODE_NUM+3 //行按钮开始的位置=节点数+mode+time+name
#define ROW_BTN_NUM 5 //按钮个数：上下移动、增加删除、运行
#define BEFORE_VALUE_NUM 2 //节点数之前的数值个数：name、mode

ControlTableView::ControlTableView(QWidget *parent)
{
    headerDataInit();
    valueListInit();
    modelInit();
    tableViewInit();
    addTableviewRow(0, 0, false);
}

/**
*@projectName   RobotControlSystem
*@brief         表头初始化，与BTN_START_INDEX、ROW_BTN_NUM、BEFORE_VALUE_NUM有关联
*@parameter
*@author        XingZhang.Wu
*@date          20190805
**/
void ControlTableView::headerDataInit()
{
    headerData.clear();
    headerData << QObject::tr("Name") << QObject::tr("Mode");
    for(int i=0;i<NODE_NUM;i++) {
        headerData << QString("Value%1").arg(i+1);
    }
    headerData << QObject::tr("Time") << QObject::tr("Run") << QObject::tr("Add")
               << QObject::tr("Delete") << QObject::tr("Up") << QObject::tr("Down")
               << QObject::tr("Select");
}

void ControlTableView::modelInit()
{
    model = new QStandardItemModel();
    model->setColumnCount(headerData.length());
    for(int i=0;i<headerData.length();i++) {
        model->setHeaderData(i,Qt::Horizontal,headerData[i]);
    }
}

void ControlTableView::tableViewInit()
{
    resizeColumnsToContents();
    setAlternatingRowColors(true);
    setSelectionBehavior(QAbstractItemView::SelectRows);
    //如果你用在QTableView中使用右键菜单，需启用该属性
    setContextMenuPolicy(Qt::CustomContextMenu);
    horizontalHeader()->setFont(QFont("Times", 10, QFont::Bold));
    setModel(model);
}

/**
*@projectName   RobotControlSystem
*@brief         valueList为初始化表中行时的数据来源
*@parameter
*@author        XingZhang.Wu
*@date          20190805
**/
void ControlTableView::valueListInit()
{
    valueList.clear();
    valueList << QObject::tr("ref");
    for(int i=0;i<NODE_NUM;i++) {
        valueList << QObject::tr("0");
    }
    valueList << QObject::tr("500");
}

/**
*@projectName   RobotControlSystem
*@brief         valueList为新增表中行时的数据来源
*@parameter
*@author        XingZhang.Wu
*@date          20190805
**/
void ControlTableView::valueListUpdate(QString currentName, int currentPeriod, int currentType)
{
    valueList.clear();
    valueList << currentName;
    for(int i=0;i<NODE_NUM;i++) {
        if(currentType>0) {
            valueList << QString::number(GlobalData::currentCanAnalyticalData[i].position);
        } else {
            valueList << QString::number(GlobalData::currentCanAnalyticalData[i].speed);
        }
    }
    valueList << QString::number(currentPeriod);
}

void ControlTableView::valueListUpdate(int row)
{
    valueList.clear();
    valueList << model->index(row,0).data().toString();
    for(int i=BEFORE_VALUE_NUM;i<NODE_NUM+BEFORE_VALUE_NUM;i++)
    {
        valueList << model->index(row,i).data().toString();
    }
    valueList << model->index(row,NODE_NUM+BEFORE_VALUE_NUM).data().toString();
}

/**
*@projectName   RobotControlSystem
*@brief         表中新增一行，通过重新设置model完成
*@parameter
*@author        XingZhang.Wu
*@date          20190805
**/
void ControlTableView::addModelItemData(int row)
{
    for(int i=0;i<valueList.length();i++) {
        if(i==0)
            model->setItem(row,i,new QStandardItem(valueList.at(i)));
        else
            model->setItem(row,i+1,new QStandardItem(valueList.at(i)));
    }
    //设置字符颜色
    model->item(row,0)->setForeground(QBrush(QColor(255, 0, 0)));
}

/**
*@projectName   RobotControlSystem
*@brief         表中新增一行widget，通过调用对应的model重新设置tableview完成
*@parameter
*@author        XingZhang.Wu
*@date          20190805
**/
void ControlTableView::addTableviewRowWidget(int mode, int row, bool checkState, bool complete)
{
    int column=valueList.length();

    //为这个第2列添加下拉框
    IncompleteCombox *m_combox= new IncompleteCombox();
    m_combox->addItem("speed");
    m_combox->addItem("absolute pos");
    m_combox->addItem("relative pos");
    m_combox->setCurrentIndex(mode);
    m_combox->setProperty("row", row);  //为按钮设置row属性
    setIndexWidget(model->index(row, 1), m_combox);
    if(complete) {
        //为这个第i列添加按钮
        QPushButton *m_button[ROW_BTN_NUM];
        QStringList iconPath;
        iconPath << tr(":/images/go.png") << tr(":/images/add.png")
                 << tr(":/images/delete.png") << tr(":/images/up.png")
                 << tr(":/images/down.png");
        for(int i=0;i<ROW_BTN_NUM;i++) {
            m_button[i] = new QPushButton();
            m_button[i]->setIcon(QIcon(iconPath[i]));
            connect(m_button[i], SIGNAL(clicked(bool)), this, SLOT(tableClickButton()));
            m_button[i]->setProperty("row", row);  //为按钮设置row属性
            m_button[i]->setProperty("column", column+i+1);  //为按钮设置column属性
            setIndexWidget(model->index(row, column+i+1), m_button[i]);
        }
    }

    QCheckBox *m_checkBox = new QCheckBox();
    if(checkState)
        m_checkBox->setChecked(true);
    else
        m_checkBox->setChecked(false);
    m_checkBox->setProperty("row", row);  //为按钮设置row属性
    setIndexWidget(model->index(row, column+ROW_BTN_NUM+1), m_checkBox);
}

/**
*@projectName   RobotControlSystem
*@brief         获取表中某一行的数据
*@parameter
*@author        XingZhang.Wu
*@date          20190805
**/
void ControlTableView::getModelRowValue(double* value, int row, int len)
{
    for(int i=BEFORE_VALUE_NUM;i<len+BEFORE_VALUE_NUM;i++)
    {
        value[i-BEFORE_VALUE_NUM] = model->index(row,i).data().toDouble();
    }
}

/**
*@projectName   RobotControlSystem
*@brief         表中widget对应的事件函数
*@parameter
*@author        XingZhang.Wu
*@date          20190805
**/
void ControlTableView::tableClickButton()
{
    QPushButton *btn = (QPushButton *)sender();   //产生事件的对象
    int row = btn->property("row").toInt();  //取得按钮的行号属性
    int col = btn->property("column").toInt();  //取得按钮的列号属性

    switch (col) {
    case BTN_START_INDEX://run
        double value[NODE_NUM];
        double refValue[NODE_NUM];
        getModelRowValue(refValue, 0, NODE_NUM);
        getModelRowValue(value, row, NODE_NUM);

        if(0 == qobject_cast<IncompleteCombox *>(indexWidget(model->index(row,1)))->currentIndex()) {
            Package::packOperateMulti(GlobalData::sendId, value, NODE_NUM, PROTOCOL_TYPE_SPD);
        }
        else if(1 == qobject_cast<IncompleteCombox *>(indexWidget(model->index(row,1)))->currentIndex()) {
            for(int i=0;i<NODE_NUM;i++) {
                value[i] += refValue[i];
            }
            //In order to be able to reach the initial position, set this mode here.
            Package::packOperateMulti(GlobalData::sendId, value, NODE_NUM, PROTOCOL_TYPE_POS);
        }
        else {//relative position mode
            for(int i=0;i<NODE_NUM;i++) {
                value[i] += GlobalData::currentCanAnalyticalData[i].position;
            }
            Package::packOperateMulti(GlobalData::sendId, value, NODE_NUM, PROTOCOL_TYPE_POS);
        }
        break;
    case BTN_START_INDEX+1://add
        model->insertRow(row);
        valueListUpdate(row+1);
        addTableviewRow(qobject_cast<IncompleteCombox *>(indexWidget(model->index(row+1,1)))->currentIndex(), row, true);
        updateTablePropertyAfterLine(row,0);
        break;
    case BTN_START_INDEX+2://delete
        model->removeRow(row);
        updateTablePropertyAfterLine(row,0);
        break;
    case BTN_START_INDEX+3://up
        if(row > 1) {
            int modeBak = qobject_cast<IncompleteCombox *>(indexWidget(model->index(row,1)))->currentIndex();
            bool checkBak = qobject_cast<QCheckBox *>(indexWidget(model->index(row,BTN_START_INDEX+5)))->isChecked();
            QList<QStandardItem *> listItem = model->takeRow(row);
            model->insertRow(row-1,listItem);
            addTableviewRowWidget(modeBak,row-1,checkBak,true);
            updateTableRowProperty(row-1,row-1);
            updateTableRowProperty(row,row);
        }
        break;
    case BTN_START_INDEX+4://down
        if(row < model->rowCount()-1) {
            int modeBak = qobject_cast<IncompleteCombox *>(indexWidget(model->index(row,1)))->currentIndex();
            bool checkBak = qobject_cast<QCheckBox *>(indexWidget(model->index(row,BTN_START_INDEX+5)))->isChecked();
            QList<QStandardItem *> listItem = model->takeRow(row);
            model->insertRow(row+1,listItem);
            addTableviewRowWidget(modeBak,row+1,checkBak,true);
            updateTableRowProperty(row+1,row+1);
            updateTableRowProperty(row,row);
        }
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
void ControlTableView::updateTableRowProperty(int row, int property)
{
    int len=valueList.length();
    qobject_cast<IncompleteCombox *>(indexWidget(model->index(row,1)))->setProperty("row", property);
    qobject_cast<QPushButton *>(indexWidget(model->index(row,len+1)))->setProperty("row", property);
    qobject_cast<QPushButton *>(indexWidget(model->index(row,len+2)))->setProperty("row", property);
    qobject_cast<QPushButton *>(indexWidget(model->index(row,len+3)))->setProperty("row", property);
    qobject_cast<QPushButton *>(indexWidget(model->index(row,len+4)))->setProperty("row", property);
    qobject_cast<QPushButton *>(indexWidget(model->index(row,len+5)))->setProperty("row", property);
    qobject_cast<QCheckBox *>(indexWidget(model->index(row,len+6)))->setProperty("row", property);
}

void ControlTableView::updateTablePropertyAfterLine(int row, int offset)
{
    for(int i=row;i<model->rowCount();i++) {
        updateTableRowProperty(i,i+offset);
    }
}

/**
*@projectName   RobotControlSystem
*@brief         表中新增一行，包括数据和widget
*@parameter
*@author        XingZhang.Wu
*@date          20190805
**/
void ControlTableView::addTableviewRow(int mode, int row, bool hasWidget)
{
    addModelItemData(row);
    if(hasWidget)
        addTableviewRowWidget(mode, row, true, true);
}

/**
*@projectName   RobotControlSystem
*@brief         隐藏表格中的数据
*@parameter
*@author        XingZhang.Wu
*@date          20190806
**/
void ControlTableView::hideTableviewData(bool is_hide)
{
    for(int i=BEFORE_VALUE_NUM;i<BEFORE_VALUE_NUM+NODE_NUM;i++) {
        setColumnHidden(i, !is_hide);
    }
}