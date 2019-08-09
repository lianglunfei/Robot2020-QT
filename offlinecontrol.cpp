#include "offlinecontrol.h"
#include "ui_offlinecontrol.h"

#include "debug.h"
#include <QFileDialog>

OfflineControl::OfflineControl(QWidget *parent) :
    QDialog(parent),
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
    ui->tableView->valueListUpdate(ui->nameLineEdit->text(),
                                   ui->interPeriodSpinBox->value(),
                                   ui->spdPosSelectComboBox->currentIndex());
    ui->tableView->addTableviewRow(ui->spdPosSelectComboBox->currentIndex(),
                                   ui->tableView->model->rowCount(), true);
}

void OfflineControl::on_hidePushButton_clicked()
{
    if(ui->hidePushButton->text().contains("展开数值")) {
        ui->hidePushButton->setText("隐藏数值");
        ui->tableView->hideTableviewData(true);
    }
    else {
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
    ui->execSeqPushButton->setText("顺序执行所选");
    ui->execReverseSeqPushButton->setText("逆序执行所选");
}

void OfflineControl::on_execSeqPushButton_clicked()
{
    ui->execReverseSeqPushButton->setText("逆序执行所选");
    int ret = ui->tableView->seqExec(ui->cycleCheckBox->isChecked(),
                           ui->interValueSpinBox->value(), ui->interPeriodSpinBox->value());
    if(ret==1)
        ui->execSeqPushButton->setText("顺序执行暂停");
    else
        ui->execSeqPushButton->setText("顺序执行继续");
}

void OfflineControl::on_execReverseSeqPushButton_clicked()
{
    ui->execSeqPushButton->setText("顺序执行所选");
    int ret = ui->tableView->reverseSeqExec(ui->cycleCheckBox->isChecked(),
                           ui->interValueSpinBox->value(), ui->interPeriodSpinBox->value());
    if(ret==1)
        ui->execReverseSeqPushButton->setText("逆序执行暂停");
    else
        ui->execReverseSeqPushButton->setText("逆序执行继续");
}

void OfflineControl::on_execSeqStopPushButton_clicked()
{
    ui->tableView->execStop();
    ui->execSeqPushButton->setText("顺序执行所选");
    ui->execReverseSeqPushButton->setText("逆序执行所选");
}

void OfflineControl::on_exportPushButton_clicked()
{
    QString fileName;
    fileName = QFileDialog::getSaveFileName(this, tr("Save File"),
                                            "g_fileDir/tst.csv",
                                            tr("Files (*.csv)"));
    if(fileName.isEmpty())
    {
        return;
    }
    ui->tableView->exportToCsv(fileName);
    g_fileDir = QFileInfo(fileName).absoluteFilePath();
    ui->fileNameLabel->setText(fileName+" saved");
}

void OfflineControl::on_importPushButton_clicked()
{
    QString  fileName;
    fileName = QFileDialog::getOpenFileName(this,
                                            tr("Open Csv"), "g_fileDir/tst.csv", tr("Files (*.csv)"));
    if(fileName.isEmpty())
    {
        return;
    }
    ui->tableView->importCsv(fileName);
    g_fileDir = QFileInfo(fileName).absoluteFilePath();
    ui->fileNameLabel->setText(fileName+" opened");
}
