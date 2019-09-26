#include "offlinecontrol.h"
#include "ui_offlinecontrol.h"

#include "debug.h"
#include "drivers.h"
#include <QFileDialog>
#include <QKeyEvent>

OfflineControl::OfflineControl(QWidget *parent) : QDialog(parent),
                                                  ui(new Ui::OfflineControl)
{
    ui->setupUi(this);
    this->setWindowTitle(tr("Offline Control"));

    connect(ui->tableView, &ControlTableView::execStatus, this, &OfflineControl::runStatus);
    connect(ui->tableView, &ControlTableView::stopThread, this, &OfflineControl::stopStatus);
}

OfflineControl::~OfflineControl()
{
    delete ui;
}

void OfflineControl::on_addRecordPushButton_clicked()
{
    ui->tableView->valueListSync(ui->nameLineEdit->text(),
                                 ui->interPeriodSpinBox->value());
    ui->tableView->addTableviewRow(ui->spdPosSelectComboBox->currentIndex(),
                                   ui->tableView->model->rowCount(), true);
}

void OfflineControl::on_synchronousPushButton_clicked()
{
    int row = ui->tableView->selectionModel()->currentIndex().row();
    if (row > -1)
        ui->tableView->syncTableviewRowData(row);
}

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

void OfflineControl::runStatus(QString s)
{
    ui->runStatus->setText(s);
}

void OfflineControl::stopStatus()
{
    ui->execSeqPushButton->setStyleSheet("color:black;");
    ui->execReverseSeqPushButton->setStyleSheet("color:black;");
    ui->execSeqPushButton->setText("顺序执行所选");
    ui->execReverseSeqPushButton->setText("逆序执行所选");
}

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

void OfflineControl::on_execSeqStopPushButton_clicked()
{
    ui->tableView->execStop();
    ui->execSeqPushButton->setStyleSheet("color:black;");
    ui->execReverseSeqPushButton->setStyleSheet("color:black;");
    ui->execSeqPushButton->setText("顺序执行所选");
    ui->execReverseSeqPushButton->setText("逆序执行所选");
}

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
    fileName = QDir::currentPath() + "/" + ui->fileNameLineEdit->text() + ".csv";
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

void OfflineControl::on_initDriverPushButton_clicked()
{
    on_execSeqStopPushButton_clicked();
    Drivers::initJoint();
}

void OfflineControl::on_emergencyStopPushButton_clicked()
{
    on_execSeqStopPushButton_clicked();
    Drivers::stopJoint();
}

void OfflineControl::on_caliPushButton_clicked()
{
    on_execSeqStopPushButton_clicked();
    Drivers::calJoint();
}

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
