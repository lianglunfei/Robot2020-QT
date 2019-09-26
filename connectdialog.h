#ifndef CONNECTDIALOG_H
#define CONNECTDIALOG_H

#include <QDialog>

namespace Ui
{
class ConnectDialog;
}

#define CAN_BAUTE_1000 1000
#define CAN_BAUTE_800 800
#define CAN_BAUTE_666 666
#define CAN_BAUTE_500 500
#define CAN_BAUTE_400 400
#define CAN_BAUTE_250 250
#define CAN_BAUTE_200 200
#define CAN_BAUTE_125 125

class ConnectDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ConnectDialog(QWidget *parent = 0);
    ~ConnectDialog();

private:
    Ui::ConnectDialog *ui;
    void accept();
};

#endif // CONNECTDIALOG_H
