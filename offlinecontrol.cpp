/*
 * @Author: xingzhang.Wu 
 * @Date: 2020-01-06 11:18:27 
 * @Last Modified by: Qingmao.Wei
 * @Last Modified time: 2020-01-06 13:50:45
 */
#include "offlinecontrol.h"
#include "ui_offlinecontrol.h"

#include "debug.h"
#include "drivers.h"
#include <QFileDialog>
#include <QKeyEvent>

/**
 * @brief Construct a new Offline Control:: Offline Control object
 * 
 * @param parent 
 */
OfflineControl::OfflineControl(QWidget *parent) : QDialog(parent),
                                                  ui(new Ui::OfflineControl)
{
    ui->setupUi(this);
    this->setWindowTitle(tr("Offline Control"));

    connect(ui->tableView, &ControlTableView::execStatus, this, &OfflineControl::runStatus);
    connect(ui->tableView, &ControlTableView::stopThread, this, &OfflineControl::stopStatus);
}

/**
 * @brief Destroy the Offline Control:: Offline Control object
 * 
 */
OfflineControl::~OfflineControl()
{
    delete ui;
}

/**
 * @brief 新增命令
 * 
 */
void OfflineControl::on_addRecordPushButton_clicked()
{
    ui->tableView->valueListSync(ui->nameLineEdit->text(),
                                 ui->interPeriodSpinBox->value());
    ui->tableView->addTableviewRow(ui->spdPosSelectComboBox->currentIndex(),
                                   ui->tableView->model->rowCount(), true);
}

/**
 * @brief 同步命令，将当前接收到的CAN信息同步到所选的table行中
 * 
 */
void OfflineControl::on_synchronousPushButton_clicked()
{
    int row = ui->tableView->selectionModel()->currentIndex().row();
    if (row > -1)
        ui->tableView->syncTableviewRowData(row);
}

/**
 * @brief 展开/隐藏数据
 * 
 */
void OfflineControl::on_hidePushButton_clicked()
{
    if (ui->hidePushButton->text().contains("展开数值"))
    {
        ui->hidePushButton->setText("隐藏数值");
        ui->tableView->hideTableviewData(true);
    }
    else
    {
        ui->hidePushButton->setText("展开数值");
        ui->tableView->hideTableviewData(false);
    }
}

/**
 * @brief 仿真数据运行状态显示
 * 
 * @param s 
 */
void OfflineControl::runStatus(QString s)
{
    ui->runStatus->setText(s);
}

/**
 * @brief 停止仿真时设置相关状态
 * 
 */
void OfflineControl::stopStatus()
{
    ui->execSeqPushButton->setStyleSheet("color:black;");
    ui->execReverseSeqPushButton->setStyleSheet("color:black;");
    ui->execSeqPushButton->setText("顺序执行所选");
    ui->execReverseSeqPushButton->setText("逆序执行所选");
}

/**
 * @brief 仿真数据顺序执行
 * 
 */
void OfflineControl::on_execSeqPushButton_clicked()
{
    ui->execSeqPushButton->setStyleSheet("color:black;");
    ui->execReverseSeqPushButton->setStyleSheet("color:black;");
    ui->execReverseSeqPushButton->setText("逆序执行所选");
    int ret = ui->tableView->seqExec(ui->cycleCheckBox->isChecked(),
                                     ui->interValueSpinBox->value(), ui->interPeriodSpinBox->value());
    if (ret == 1)
        ui->execSeqPushButton->setText("顺序执行暂停");
    else
        ui->execSeqPushButton->setText("顺序执行继续");
}

/**
 * @brief 仿真数据逆序执行
 * 
 */
void OfflineControl::on_execReverseSeqPushButton_clicked()
{
    ui->execSeqPushButton->setStyleSheet("color:black;");
    ui->execReverseSeqPushButton->setStyleSheet("color:black;");
    ui->execSeqPushButton->setText("顺序执行所选");
    int ret = ui->tableView->reverseSeqExec(ui->cycleCheckBox->isChecked(),
                                            ui->interValueSpinBox->value(), ui->interPeriodSpinBox->value());
    if (ret == 1)
        ui->execReverseSeqPushButton->setText("逆序执行暂停");
    else
        ui->execReverseSeqPushButton->setText("逆序执行继续");
}

/**
 * @brief 异常时执行行数，一般为让电机停止运动
 * 
 */
void OfflineControl::pausedWhenError()
{
    int ret = ui->tableView->execPause();
    if (ret == 1)
    {
        ui->execSeqPushButton->setText("顺序异常继续");
        ui->execSeqPushButton->setStyleSheet("color:red;");
    }
    else if (ret == 2)
    {
        ui->execReverseSeqPushButton->setText("逆序异常继续");
        ui->execReverseSeqPushButton->setStyleSheet("color:red;");
    }
}

/**
 * @brief 顺序执行停止
 * 
 */
void OfflineControl::on_execSeqStopPushButton_clicked()
{
    ui->tableView->execStop();
    ui->execSeqPushButton->setStyleSheet("color:black;");
    ui->execReverseSeqPushButton->setStyleSheet("color:black;");
    ui->execSeqPushButton->setText("顺序执行所选");
    ui->execReverseSeqPushButton->setText("逆序执行所选");
}

/**
 * @brief 将当前Table中的数据导出值CSV文件中
 * TODO: 目前打开文件的窗口在ubuntu中有问题
 */
void OfflineControl::on_exportPushButton_clicked()
{
    QString fileName;
    // fileName = QFileDialog::getSaveFileName(this, tr("Save File"),
    //                                         "g_fileDir/tst.csv",
    //                                         tr("Files (*.csv)"));
    // if(fileName.isEmpty())
    // {
    //     return;
    // }
    if (ui->fileNameLineEdit->text().isEmpty())
    {
        ui->fileNameLabel->setText("file name is empty!");
        return;
    }
#ifdef Q_OS_WIN
    // fileName = QDir::currentPath() + "/" + ui->fileNameLineEdit->text() + ".csv";
    fileName = ui->fileNameLineEdit->text();
#else
    char *path = getenv("HOME");
    fileName = QString(path) + "/csv/" + ui->fileNameLineEdit->text() + ".csv";
#endif
    ui->fileNameLabel->setText(fileName + " opening");
    if (ui->tableView->exportToCsv(fileName) != -1)
    {
        g_fileDir = QFileInfo(fileName).absoluteFilePath();
        ui->fileNameLabel->setText(fileName + " saved");
    }
    else
    {
        ui->fileNameLabel->setText("export " + fileName + " failed!");
    }
}

/**
 * @brief 打开系统自带资源浏览器来选择csv文件
 * TODO: export
 */
void OfflineControl::on_fileSelectorButton_clicked()
{
    qDebug() << "In file explorer.";
    //定义文件对话框类
    QFileDialog *fileDialog = new QFileDialog(this);
    //定义文件对话框标题
    fileDialog->setWindowTitle(tr("选择csv文件"));
    //设置默认文件路径
    fileDialog->setDirectory(".");
    //设置文件过滤器
    fileDialog->setNameFilter(tr("CSV File(*.csv)"));
    //设置可以选择多个文件,默认为只能选择一个文件QFileDialog::ExistingFiles
    // fileDialog->setFileMode(QFileDialog::ExistingFiles);
    //设置视图模式
    fileDialog->setViewMode(QFileDialog::Detail);
    //打印所有选择的文件的路径
    QStringList fileNames;
    if (fileDialog->exec())
    {
        fileNames = fileDialog->selectedFiles();
    }

    // 如果没有选择任何文件
    if (fileNames.length() < 1)
        return;
    QString fileName = fileNames[0];
    ui->fileNameLineEdit->setText(fileName);
    ui->fileNameLabel->setText(fileName + " opening");
    if (ui->tableView->importCsv(fileName) != -1)
    {
        g_fileDir = QFileInfo(fileName).absoluteFilePath();
        ui->fileNameLabel->setText(fileName + " opened");
    }
    else
    {
        ui->fileNameLabel->setText("import " + fileName + " failed!");
    }
}

/**
 * @brief 将CSV文件的数据导入到Table中
 * TODO: 目前打开文件的窗口在ubuntu中有问题
 */
void OfflineControl::on_importPushButton_clicked()
{
    QString fileName;
    // fileName = QFileDialog::getOpenFileName(this,
    //                                         tr("Open Csv"), "g_fileDir/tst.csv", tr("Files (*.csv)"));
    // if(fileName.isEmpty())
    // {
    //     return;
    // }
    if (ui->fileNameLineEdit->text().isEmpty())
    {
        ui->fileNameLabel->setText("file name is empty!");
        return;
    }
#ifdef Q_OS_WIN
    fileName = QDir::currentPath() + "/" + ui->fileNameLineEdit->text() + ".csv";
#else
    char *path = getenv("HOME");
    fileName = QString(path) + "/csv/" + ui->fileNameLineEdit->text() + ".csv";
#endif
    ui->fileNameLabel->setText(fileName + " opening");
    if (ui->tableView->importCsv(fileName) != -1)
    {
        g_fileDir = QFileInfo(fileName).absoluteFilePath();
        ui->fileNameLabel->setText(fileName + " opened");
    }
    else
    {
        ui->fileNameLabel->setText("import " + fileName + " failed!");
    }
}

/**
 * @brief 初始化所有电机
 * 
 */
void OfflineControl::on_initDriverPushButton_clicked()
{
    on_execSeqStopPushButton_clicked();
    Drivers::initJoint();
}

/**
 * @brief 所有电机紧急停止
 * 
 */
void OfflineControl::on_emergencyStopPushButton_clicked()
{
    on_execSeqStopPushButton_clicked();
    Drivers::stopJoint();
}

/**
 * @brief 校准所有电机
 * ! 只对老版本的电机有效
 * 
 */
void OfflineControl::on_caliPushButton_clicked()
{
    on_execSeqStopPushButton_clicked();
    Drivers::calJoint();
}

/**
 * @brief 按键快捷键设置
 * 
 * @param e 
 */
void OfflineControl::keyPressEvent(QKeyEvent *e)
{
    switch (e->key())
    {
    case Qt::Key_A:
    {
        if (QApplication::keyboardModifiers() == Qt::AltModifier)
        {
            on_emergencyStopPushButton_clicked();
        }
        break;
    }
    }
}
