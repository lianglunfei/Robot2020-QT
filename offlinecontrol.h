#ifndef OFFLINECONTROL_H
#define OFFLINECONTROL_H

#include <QDialog>

namespace Ui {
class OfflineControl;
}

class OfflineControl : public QDialog
{
    Q_OBJECT

public:
    explicit OfflineControl(QWidget *parent = 0);
    ~OfflineControl();

private:
    Ui::OfflineControl *ui;

private slots:
    void on_addRecordPushButton_clicked();
    void on_hidePushButton_clicked();
    void runStatus(QString s);
    void stopStatus();
    void on_execSeqPushButton_clicked();
    void on_execReverseSeqPushButton_clicked();
    void on_execSeqStopPushButton_clicked();
};

#endif // OFFLINECONTROL_H
