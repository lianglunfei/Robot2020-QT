#ifndef CONTROLTABLEVIEW_H
#define CONTROLTABLEVIEW_H

#include <QTableView>
#include <QStandardItemModel>

class ControlTableView : public QTableView
{
    Q_OBJECT
public:
    explicit ControlTableView(QWidget *parent = 0);

    void valueListUpdate(QString currentName, int currentPeriod, int currentType);
    void valueListUpdate(int row);
    void addTableviewRow(int mode, int row, bool hasWidget);
    void hideTableviewData(bool is_hide);

    QStandardItemModel *model;

private:
    QStringList headerData;
    QStringList valueList;

    void headerDataInit();
    void valueListInit();
    void modelInit();
    void tableViewInit();
    void addModelItemData(int row);
    void addTableviewRowWidget(int mode, int row, bool checkState, bool complete);
    void getModelRowValue(double* value, int row, int len);
    void updateTableRowProperty(int row, int property);
    void updateTablePropertyAfterLine(int row, int offset);

private slots:
    void tableClickButton();
};

#endif // CONTROLTABLEVIEW_H
