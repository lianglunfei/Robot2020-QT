#include "offlinesequencecontrol.h"
#include "ui_offlinesequencecontrol.h"

OfflineSequenceControl::OfflineSequenceControl(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::OfflineSequenceControl)
{
    ui->setupUi(this);
}

OfflineSequenceControl::~OfflineSequenceControl()
{
    delete ui;
}
