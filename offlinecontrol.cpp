#include "offlinecontrol.h"
#include "ui_offlinecontrol.h"

#include "debug.h"

OfflineControl::OfflineControl(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::OfflineControl)
{
    ui->setupUi(this);
    this->setWindowTitle(tr("Offline Control"));
}

QString OfflineControl::getCurrentName()
{
    return ui->nameLineEdit->text();
}

int OfflineControl::getCurrentTime()
{
    return ui->timeIntervalSpinBox->value();
}

int OfflineControl::getCurrentModeIndex()
{
    return ui->spdPosSelectComboBox->currentIndex();
}

void OfflineControl::on_addRecordPushButton_clicked()
{
    ui->tableView->valueListUpdate(ui->nameLineEdit->text(),
                                   ui->timeIntervalSpinBox->value(),
                                   ui->spdPosSelectComboBox->currentIndex());
    ui->tableView->addTableviewRow(ui->spdPosSelectComboBox->currentIndex(),
                                   ui->tableView->model->rowCount(), true);
}

OfflineControl::~OfflineControl()
{
    delete ui;
}
