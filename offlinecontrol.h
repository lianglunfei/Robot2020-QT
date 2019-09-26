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

public slots:
    void on_execSeqStopPushButton_clicked();

private:
    Ui::OfflineControl *ui;
    QString g_fileDir;

    void keyPressEvent(QKeyEvent *e) override;

private slots:
    void on_addRecordPushButton_clicked();
    void on_hidePushButton_clicked();
    void runStatus(QString s);
    void stopStatus();
    void on_execSeqPushButton_clicked();
    void on_execReverseSeqPushButton_clicked();
    void on_exportPushButton_clicked();
    void on_importPushButton_clicked();
    void on_initDriverPushButton_clicked();
    void on_emergencyStopPushButton_clicked();
    void on_synchronousPushButton_clicked();
    void on_caliPushButton_clicked();
};

#endif // OFFLINECONTROL_H
