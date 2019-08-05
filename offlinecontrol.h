#ifndef OFFLINECONTROL_H
#define OFFLINECONTROL_H

#include <QDialog>

namespace Ui {
class OfflineControl;
}

class QStandardItemModel;

class OfflineControl : public QDialog
{
    Q_OBJECT

public:
    explicit OfflineControl(QWidget *parent = 0);
    ~OfflineControl();

private:
    void headerDataInit();
    void valueListInit();
    void valueListUpdate();
    void modelInit();
    void tableViewInit();
    void addModelItemData(int row);
    void addTableviewRowWidget(int mode, int row, bool checkState, bool complete);
    void addTableviewRow(int mode, int row, bool hasWidget);
    void getModelRowValue(double* value, int row, int len);
    void updateTableRowProperty(int row, int property);
    void updateTablePropertyAfterLine(int row, int offset);

private:
    Ui::OfflineControl *ui;
    QStandardItemModel *model;
    QStringList headerData;
    QStringList valueList;

private slots:
    void tableClickButton();
    void on_addRecordPushButton_clicked();
};

#endif // OFFLINECONTROL_H
