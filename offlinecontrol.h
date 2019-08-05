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
    virtual QString getCurrentName();
    virtual int getCurrentTime();
    virtual int getCurrentModeIndex();

private:
    Ui::OfflineControl *ui;

private slots:
    void on_addRecordPushButton_clicked();
};

#endif // OFFLINECONTROL_H
