/*
 * @Descripttion: 
 * @version: 
 * @Author: xingzhang.Wu
 * @Date: 2020-01-08 15:36:56
 * @LastEditors  : Qingmao Wei
 * @LastEditTime : 2020-01-15 14:28:17
 */

#include "offlinesequencecontrol.h"
#include "ui_offlinesequencecontrol.h"
#include "debug.h"
#include "drivers.h"

#include <QFileDialog>
#include <QKeyEvent>
#include <QMessageBox>
#include <QSpinBox>
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
    // ui->pauseButton->setShortcut(Qt::Key_P);
    ui->stopButton->setShortcut(QKeySequence(Qt::ALT + Qt::Key_A));
    seqWorker = &SequenceExcuteWorker::getInstance();
    movementPtrInit();
    deactivateAllMoveButtons();
    ui->pauseButton->setEnabled(0);
    connect(seqWorker, &SequenceExcuteWorker::execStatus, this, &OfflineSequenceControl::runStatus);
    connect(seqWorker, &SequenceExcuteWorker::stopThread, this, &OfflineSequenceControl::stopStatus);
    connect(ui->interPeriodSpinBox, static_cast<void(QSpinBox::*)(int)>(&QSpinBox::valueChanged), ui->tableView, &SequenceTableView::setInterPeriod);
    connect(ui->interValueSpinBox, static_cast<void(QSpinBox::*)(int)>(&QSpinBox::valueChanged), ui->tableView, &SequenceTableView::setInterValue);
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
    ui->tableView->setActionList(actionNames);

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
    ui->assetPathlineEdit->setText(dirpath);
    ui->messageLabel->setText(QString("加载完成"));
    activateAllMoveButtons();
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
}

void OfflineSequenceControl::on_pauseButton_clicked()
{
    ui->pauseButton->setDefault(1);
    UIWaitTask(pause_op);
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
}

int OfflineSequenceControl::UIWaitTask(move_ops mov)
{

    qDebug() << "move is " << mov;
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
                                 QMessageBox::Ok | QMessageBox::Cancel);
            return -1;
        }
        seqWorker->seqExec(false, ui->interValueSpinBox->value(), ui->interPeriodSpinBox->value());
        return 0;
    case sit_op:
        if (seqWorker->importCSV(actionModels[QString("stand")]))
        {
            QMessageBox::warning(this, tr("CSV error"),
                                 tr("格式不正确。"),
                                 QMessageBox::Ok | QMessageBox::Cancel);
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
