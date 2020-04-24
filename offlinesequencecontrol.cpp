/*
 * @Descripttion: 
 * @version: 
 * @Author: xingzhang.Wu
 * @Date: 2020-01-08 15:36:56
 * @LastEditors  : Qingmao Wei
 * @LastEditTime : 2020-01-16 15:47:28
 */

#include "offlinesequencecontrol.h"
#include "ui_offlinesequencecontrol.h"
#include "debug.h"
#include "drivers.h"

#include <QFileDialog>
#include <QKeyEvent>
#include <QMessageBox>
#include <QSpinBox>
#include <QEventLoop>
#include <QTimer>
#include <QElapsedTimer>
// #include <QWidget>

OfflineSequenceControl::OfflineSequenceControl(QWidget *parent) : QDialog(parent),
                                                                  ui(new Ui::OfflineSequenceControl)
{
    setFocusPolicy(Qt::StrongFocus);
    ui->setupUi(this);
    ui->forwardButton->setShortcut(Qt::Key_Up);
    ui->leftButton->setShortcut(Qt::Key_Left);
    ui->backButton->setShortcut(Qt::Key_Down);
    ui->rightButton->setShortcut(Qt::Key_Right);
    ui->exportButton->setEnabled(0);
    ui->importButton->setEnabled(0);
    ui->resetActionButton->setDisabled(1);
    ui->openValveButton->setEnabled(0);
    ui->openValveButton->hide();

    QIcon myicon;                                  //新建QIcon对象
    myicon.addFile(tr("qrc:/images/uparrow.ico")); //让QIcon对象指向想要的图标
    ui->forwardButton->setIcon(myicon);            //给按钮添加图标

    // ui->pauseButton->setShortcut(Qt::Key_P);
    ui->stopButton->setShortcut(QKeySequence(Qt::ALT + Qt::Key_A));
    seqWorker = &SequenceExcuteWorker::getInstance();
    movementPtrInit();
    deactivateAllMoveButtons();
    ui->pauseButton->setEnabled(0);
    connect(seqWorker, &SequenceExcuteWorker::execStatus, this, &OfflineSequenceControl::runStatus);
    connect(seqWorker, &SequenceExcuteWorker::stopThread, this, &OfflineSequenceControl::stopStatus);
    connect(ui->interPeriodSpinBox, static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged), ui->tableView, &SequenceTableView::setInterPeriod);
    connect(ui->interValueSpinBox, static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged), ui->tableView, &SequenceTableView::setInterValue);
//    ui->assetPathlineEdit->returnPressed.connect(self.lineEdit_function)
    connect(ui->assetPathlineEdit, SIGNAL(returnPressed()), this, SLOT(transDirLine()));
    // connect(seqWorker, &SequenceExcuteWorker::stopThread, this, &OfflineSequenceControl::activateAllMoveButtons);
    // connect()
}

void OfflineSequenceControl::movementPtrInit()
{
    movementButtons[forword_op] = ui->forwardButton;
    movementButtons[back_op] = ui->backButton;
    movementButtons[left_op] = ui->leftButton;
    movementButtons[right_op] = ui->rightButton;
    movementButtons[pause_op] = ui->pauseButton;
    movementButtons[stand_op] = ui->standButton;
    movementButtons[sit_op] = ui->sitButton;

    movementComboxes[forword_op] = ui->forwardComboBox;
    movementComboxes[back_op] = ui->backComboBox;
    movementComboxes[left_op] = ui->leftComboBox;
    movementComboxes[right_op] = ui->rightComboBox;
}

/**
 * @msg: 获取路径下所有.csv的文件名
 * @param path 要获取的路径
 * @return: 文件名组成的QstringList
 */
QStringList OfflineSequenceControl::getFileNames(const QString &path)
{
    QDir dir(path);
    QStringList nameFilters;
    nameFilters << "*.csv";
    QStringList files = dir.entryList(nameFilters, QDir::Files | QDir::Readable, QDir::Name);
    return files;
}

/**
 * @brief: 读取动作库
 */
void OfflineSequenceControl::readAssetDir(const QString &dirpath)
{

    // 检查路径是否为空（即 未选择）
    if (dirpath.isEmpty())
    {
        qDebug() << "Nothing.";
        return;
    }
    // 获取路径下所有文件名
    QStringList fileNames = getFileNames(dirpath);
    qDebug() << fileNames;

    // 存入 动作名 数组
    for (int iter = 0; iter < fileNames.size(); iter++)
    {
        QFileInfo info(dirpath + '/' + fileNames[iter]);
        if (info.exists())
        {
            qDebug() << info.baseName();
            actionNames.append(info.baseName());
        }
    }
    ui->backComboBox->addItems(actionNames);
    ui->forwardComboBox->addItems(actionNames);
    ui->leftComboBox->addItems(actionNames);
    ui->rightComboBox->addItems(actionNames);

    ui->tableView->actionList = actionNames;
    emit ui->tableView->setActionList(actionNames);

    // TODO: 读取这些文件 检查csv文件格式

    // 存入 绝对文件名 map
    for (int iter = 0; iter < fileNames.size(); iter++)
    {
        QFileInfo info(dirpath + '/' + fileNames[iter]);
        if (info.exists())
        {
            // qDebug() << info.baseName() << " " << info.absoluteFilePath();
            actionModels.insert(info.baseName(), info.absoluteFilePath());
        }
    }
    ui->tableView->setActionModels(actionModels);
    ui->messageLabel->setText(QString("加载完成"));
    activateAllMoveButtons();
}

/**
 * @brief: Open File Explorer
 */
void OfflineSequenceControl::transDirLine()
{
    readAssetDir(ui->assetPathlineEdit->text());
}



/**
 * @brief: Open File Explorer
 */
void OfflineSequenceControl::on_assetBrowseButton_clicked()
{
    qDebug() << "In file explorer.";

    //定义文件对话框类
    QFileDialog *fileDialog = new QFileDialog(this);
    QString dirpath = fileDialog->getExistingDirectory(this, QString("选择动作库文件夹"), QString("./"), QFileDialog::ShowDirsOnly);
    // 检查路径是否为空（即 未选择）
    if (dirpath.isEmpty())
    {
        qDebug() << "Nothing.";
        return;
    }
    readAssetDir(dirpath);
}

void OfflineSequenceControl::on_leftButton_clicked()
{
    ui->leftButton->setDefault(1);
    UIWaitTask(left_op);
}
void OfflineSequenceControl::on_forwardButton_clicked()
{
    ui->forwardButton->setDefault(1);
    UIWaitTask(forword_op);
}
void OfflineSequenceControl::on_backButton_clicked()
{
    ui->backButton->setDefault(1);
    UIWaitTask(back_op);
}
void OfflineSequenceControl::on_rightButton_clicked()
{
    ui->rightButton->setDefault(1);
    UIWaitTask(right_op);
}
void OfflineSequenceControl::on_standButton_clicked()
{
    ui->standButton->setDefault(1);
    UIWaitTask(stand_op);
}
void OfflineSequenceControl::on_sitButton_clicked()
{
    ui->sitButton->setDefault(1);
    UIWaitTask(sit_op);
}

void OfflineSequenceControl::on_stopButton_clicked()
{
    ui->stopButton->setDefault(1);
    seqWorker->execStop();
    ui->tableView->manualStop();
    Drivers::stopJoint();
}

void OfflineSequenceControl::on_pauseButton_clicked()
{
    ui->pauseButton->setDefault(1);
    UIWaitTask(pause_op);
}
void OfflineSequenceControl::on_playButton_clicked()
{
    ui->tableView->invokeNextAction();
}

/**
 * @brief 初始化所有电机
 * 
 */
void OfflineSequenceControl::on_resetButton_clicked()
{
    seqWorker->execStop();
    Drivers::initJoint();

    // 等待500ms再检查电机状态
    QEventLoop loop;                              //定义一个新的事件循环
    QTimer::singleShot(500, &loop, SLOT(quit())); //创建单次定时器，槽函数为事件循环的退出函数
    loop.exec();                                  //事件循环开始执行，程序会卡在这里，直到定时时间到，本循环被退出
    // int status;
    // if ((status = seqWorker->checkMotorStatus()) == -1)//若电机正常
    // {
    //     QMessageBox::information(this, tr("电机状态提示"),
    //                              tr("所有电机角度正常。"),
    //                              QMessageBox::Ok);
    // }
    // else
    // {
    //     QMessageBox::warning(this, tr("电机状态提示"),
    //                              tr("电机id=%1角度异常！").arg(status),
    //                              QMessageBox::Ok);
    // }
    // Drivers::initMotor();
    // qDebug()<<
    QMessageBox::information(this, tr("电机状态提示"),
                             seqWorker->checkMotorStatus(),
                             QMessageBox::Ok);
}

/**
 * @brief 初始化所有电机
 * 
 */
void OfflineSequenceControl::on_resetActionButton_clicked()
{
    if (seqWorker->importCSV(actionModels[QString("stand")]))
    {
        QMessageBox::warning(this, tr("CSV error"),
                             tr("格式不正确。"),
                             QMessageBox::Ok | QMessageBox::Cancel);
        return;
    }
    if (seqWorker->resetActionFunc())
    {
        QMessageBox::information(this, tr("电机状态提示"),
                                 tr("回到初始位置成功。"),
                                 QMessageBox::Ok);
    }
    else
    {
        QMessageBox::information(this, tr("电机状态提示"),
                                 tr("回到初始位置失败。"),
                                 QMessageBox::Ok);
    }
}

void OfflineSequenceControl::on_openValveButton_clicked()
{
    on_stopButton_clicked();
    Drivers::initValve();
}

void OfflineSequenceControl::deactivateAllMoveButtons()
{
    ui->backButton->setDisabled(1);
    ui->rightButton->setDisabled(1);
    ui->forwardButton->setDisabled(1);
    ui->leftButton->setDisabled(1);
    ui->sitButton->setDisabled(1);
    ui->standButton->setDisabled(1);
    ui->pauseButton->setEnabled(1);
    ui->resetActionButton->setDisabled(1);
}

void OfflineSequenceControl::activateAllMoveButtons()
{
    ui->backButton->setEnabled(1);
    ui->rightButton->setEnabled(1);
    ui->forwardButton->setEnabled(1);
    ui->leftButton->setEnabled(1);
    ui->sitButton->setEnabled(1);
    ui->standButton->setEnabled(1);
    ui->pauseButton->setEnabled(0);
    ui->resetActionButton->setEnabled(1);
}

int OfflineSequenceControl::UIWaitTask(move_ops mov)
{

    // 方向键
    if (mov < pause_op)
    {
        if (seqWorker->importCSV(actionModels[movementComboxes[mov]->currentText()]))
        {
            QMessageBox::warning(this, tr("CSV error"),
                                 tr("格式不正确。"),
                                 QMessageBox::Ok | QMessageBox::Cancel);
            return -1;
        }
        // qDebug()<<"excute";
        seqWorker->seqExec(false, ui->interValueSpinBox->value(), ui->interPeriodSpinBox->value());
        // qDebug()<<"excute done";
        return 0;
    }

    // else
    int ret;
    switch (mov)
    {
    case pause_op:
        ret = seqWorker->seqExec(false, ui->interValueSpinBox->value(), ui->interPeriodSpinBox->value());
        if (ret == 1)
            ui->pauseButton->setText("Pause(P)");
        else
            ui->pauseButton->setText("Continue(P)");
        return 0;
        break;
    case stand_op:
        if (seqWorker->importCSV(actionModels[QString("stand")]))
        {
            QMessageBox::warning(this, tr("CSV error"),
                                 tr("格式不正确。"),
                                 QMessageBox::Ok);
            return -1;
        }
        seqWorker->seqExec(false, ui->interValueSpinBox->value(), ui->interPeriodSpinBox->value());
        return 0;
    case sit_op:
        if (seqWorker->importCSV(actionModels[QString("stand")]))
        {
            QMessageBox::warning(this, tr("CSV error"),
                                 tr("格式不正确。"),
                                 QMessageBox::Ok);
            return -1;
        }
        seqWorker->reverseSeqExec(false, ui->interValueSpinBox->value(), ui->interPeriodSpinBox->value());
        return 0;
    default:
        break;
    }
    return 0;
}

/**
 * @brief 仿真数据运行状态显示
 * 
 * @param s 
 */
void OfflineSequenceControl::runStatus(QString s)
{
    deactivateAllMoveButtons();
    ui->messageLabel->setText(s);
}

/**
 * @brief 停止仿真时设置相关状态
 * 
 */
void OfflineSequenceControl::stopStatus()
{

    activateAllMoveButtons();
    ui->pauseButton->setEnabled(0);
    // ui->messageLabel->setText("执行结束。");
}

OfflineSequenceControl::~OfflineSequenceControl()
{
    delete ui;
}

void OfflineSequenceControl::keyPressEvent(QKeyEvent *e)
{
    // qDebug() << "You typed " << e->key();
    switch (e->key())
    {
    case Qt::Key_P:
    {
        if (ui->pauseButton->isEnabled())
            on_pauseButton_clicked();
        break;
    }
        // case Qt::Key_Up:{
        //     on_forwardButton_clicked();
        //     break;
        // }
        // case Qt::Key_Down:{
        //     on_backButton_clicked();
        //     break;
        // }
        // case Qt::Key_Left:{
        //     on_leftButton_clicked();
        //     break;
        // }
        // case Qt::Key_Right:{
        //     on_rightButton_clicked();
        //     break;
        // }
    }
}

/**
 * @brief 异常时执行行数，一般为让电机停止运动
 * 
 */
void OfflineSequenceControl::pausedWhenError()
{
    if (!ui->ignoreCheckBox->isEnabled())
    {
        on_pauseButton_clicked();
        ui->pauseButton->setText(QString("异常继续(P)"));
    }

    // if (ret == 1)
    // {
    //     ui->execSeqPushButton->setText("顺序异常继续");
    //     ui->execSeqPushButton->setStyleSheet("color:red;");
    // }
    // else if (ret == 2)
    // {
    //     ui->execReverseSeqPushButton->setText("逆序异常继续");
    //     ui->execReverseSeqPushButton->setStyleSheet("color:red;");
    // }
}
