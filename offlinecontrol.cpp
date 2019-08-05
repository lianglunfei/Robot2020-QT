#include "offlinecontrol.h"
#include "ui_offlinecontrol.h"

#include "incompletecombox.h"
#include "globaldata.h"
#include "package.h"
#include "debug.h"

#include <QStandardItemModel>

OfflineControl::OfflineControl(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::OfflineControl)
{
    ui->setupUi(this);
    this->setWindowTitle(tr("Offline Control"));

    headerDataInit();
    valueListInit();
    modelInit();
    tableViewInit();
    addTableviewRow(0, 0, false);
}

void OfflineControl::headerDataInit()
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

void OfflineControl::modelInit()
{
    model = new QStandardItemModel();
    model->setColumnCount(headerData.length());
    for(int i=0;i<headerData.length();i++) {
        model->setHeaderData(i,Qt::Horizontal,headerData[i]);
        qDebug() << headerData[i];
    }
}

void OfflineControl::tableViewInit()
{
    ui->tableView->resizeColumnsToContents();
    ui->tableView->setAlternatingRowColors(true);
    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    //如果你用在QTableView中使用右键菜单，需启用该属性
    ui->tableView->setContextMenuPolicy(Qt::CustomContextMenu);
    ui->tableView->horizontalHeader()->setFont(QFont("Times", 10, QFont::Bold));
    ui->tableView->setModel(model);
}

void OfflineControl::valueListInit()
{
    valueList.clear();
    valueList << QObject::tr("ref");
    for(int i=0;i<NODE_NUM;i++) {
        valueList << QObject::tr("0");
    }
    valueList << QObject::tr("500");
}

void OfflineControl::valueListAdd()
{
    valueList << ui->nameLineEdit->text();
    for(int i=0;i<NODE_NUM;i++) {
        valueList << QObject::tr("0");
    }
    valueList << QString::number(ui->timeIntervalSpinBox->value());
}

void OfflineControl::addModelItemData(int row)
{
    for(int i=0;i<valueList.length();i++) {
        model->setItem(row,i,new QStandardItem(valueList.at(i)));
    }
    //设置字符颜色
    model->item(row,0)->setForeground(QBrush(QColor(255, 0, 0)));
}

void OfflineControl::addTableviewRowWidget(int mode, int row, bool checkState, bool complete)
{
    int column=valueList.length();
    int btnNum=5;

    //为这个第2列添加下拉框
    IncompleteCombox *m_combox= new IncompleteCombox();
    m_combox->addItem("speed");
    m_combox->addItem("absolute pos");
    m_combox->addItem("relative pos");
    m_combox->setCurrentIndex(mode);
    m_combox->setProperty("row", row);  //为按钮设置row属性
    ui->tableView->setIndexWidget(model->index(row, 1), m_combox);
    if(complete) {
        //为这个第i列添加按钮
        QPushButton *m_button[btnNum];
        QStringList iconPath;
        iconPath << tr(":/images/go.png") << tr(":/images/add.png")
                 << tr(":/images/delete.png") << tr(":/images/up.png")
                 << tr(":/images/down.png");
        for(int i=0;i<btnNum;i++) {
            m_button[i] = new QPushButton();
            m_button[i]->setIcon(QIcon(iconPath[i]));
            connect(m_button[i], SIGNAL(clicked(bool)), this, SLOT(tableClickButton()));
            m_button[i]->setProperty("row", row);  //为按钮设置row属性
            m_button[i]->setProperty("column", column+i+1);  //为按钮设置column属性
            ui->tableView->setIndexWidget(model->index(row, column+i+1), m_button[i]);
        }
    }

    QCheckBox *m_checkBox = new QCheckBox();
    if(checkState)
        m_checkBox->setChecked(true);
    else
        m_checkBox->setChecked(false);
    m_checkBox->setProperty("row", row);  //为按钮设置row属性
    ui->tableView->setIndexWidget(model->index(row, column+btnNum+1), m_checkBox);
}

void OfflineControl::getModelRowValue(double* refValue, int row, int len)
{
    for(int i=2;i<len+2;i++)
    {
        refValue[i-2] = model->index(row,i).data().toDouble();
    }
}

void OfflineControl::tableClickButton()
{
    QPushButton *btn = (QPushButton *)sender();   //产生事件的对象
    int row = btn->property("row").toInt();  //取得按钮的行号属性
    int col = btn->property("column").toInt();  //取得按钮的列号属性

    switch (col) {
    case NODE_NUM+3://run
        double value[NODE_NUM];
        double refValue[NODE_NUM];
        getModelRowValue(refValue, 0, NODE_NUM);
        getModelRowValue(value, row, NODE_NUM);

        if(0 == qobject_cast<IncompleteCombox *>(ui->tableView->indexWidget(model->index(row,1)))->currentIndex()) {
            Package::packOperateMulti(GlobalData::sendId, value, NODE_NUM, PROTOCOL_TYPE_SPD);
        }
        else if(1 == qobject_cast<IncompleteCombox *>(ui->tableView->indexWidget(model->index(row,1)))->currentIndex()) {
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
    case NODE_NUM+4://add
        model->insertRow(row);
        valueListAdd();
        addTableviewRow(ui->spdPosSelectComboBox->currentIndex(), row, true);
        updateTablePropertyAfterLine(row,0);
        break;
    case NODE_NUM+5://delete
        model->removeRow(row);
        updateTablePropertyAfterLine(row,0);
        break;
    case NODE_NUM+6://up
        if(row > 1) {
            int modeBak = qobject_cast<IncompleteCombox *>(ui->tableView->indexWidget(model->index(row,1)))->currentIndex();
            bool checkBak = qobject_cast<QCheckBox *>(ui->tableView->indexWidget(model->index(row,20)))->isChecked();
            QList<QStandardItem *> listItem = model->takeRow(row);
            model->insertRow(row-1,listItem);
            addTableviewRowWidget(modeBak,row-1,checkBak,true);
            updateTableRowProperty(row-1,row-1);
            updateTableRowProperty(row,row);
        }
        break;
    case NODE_NUM+7://down
        if(row < model->rowCount()-1) {
            int modeBak = qobject_cast<IncompleteCombox *>(ui->tableView->indexWidget(model->index(row,1)))->currentIndex();
            bool checkBak = qobject_cast<QCheckBox *>(ui->tableView->indexWidget(model->index(row,20)))->isChecked();
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

void OfflineControl::updateTableRowProperty(int row, int property)
{
    int len=valueList.length();
    qobject_cast<IncompleteCombox *>(ui->tableView->indexWidget(model->index(row,1)))->setProperty("row", property);
    qobject_cast<QPushButton *>(ui->tableView->indexWidget(model->index(row,len+1)))->setProperty("row", property);
    qobject_cast<QPushButton *>(ui->tableView->indexWidget(model->index(row,len+2)))->setProperty("row", property);
    qobject_cast<QPushButton *>(ui->tableView->indexWidget(model->index(row,len+3)))->setProperty("row", property);
    qobject_cast<QPushButton *>(ui->tableView->indexWidget(model->index(row,len+4)))->setProperty("row", property);
    qobject_cast<QPushButton *>(ui->tableView->indexWidget(model->index(row,len+5)))->setProperty("row", property);
    qobject_cast<QCheckBox *>(ui->tableView->indexWidget(model->index(row,len+6)))->setProperty("row", property);
}

void OfflineControl::updateTablePropertyAfterLine(int row, int offset)
{
    for(int i=row;i<model->rowCount();i++) {
        updateTableRowProperty(i,i+offset);
    }
}

void OfflineControl::addTableviewRow(int mode, int row, bool hasWidget)
{
    addModelItemData(row);
    if(hasWidget)
        addTableviewRowWidget(mode, row, true, true);
}

OfflineControl::~OfflineControl()
{
    delete ui;
}

void OfflineControl::on_addRecordPushButton_clicked()
{
    valueListAdd();
    addTableviewRow(ui->spdPosSelectComboBox->currentIndex(), model->rowCount(), true);
}
