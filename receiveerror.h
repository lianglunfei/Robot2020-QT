#ifndef RECEIVEERROR_H
#define RECEIVEERROR_H

#include <QDialog>
#include <QTimer>

#include "globaldata.h"

namespace Ui {
class ReceiveError;
}

class ReceiveError : public QDialog
{
    Q_OBJECT

public:
    explicit ReceiveError(QWidget *parent = nullptr);
    ~ReceiveError();

private:
    Ui::ReceiveError *ui;
    int cout=0;
    double lastJointPos[NODE_NUM]={0};
    QTimer *time=nullptr;

    void init();

private slots:
    void update();
    void on_pushButton_clicked();
};

#endif // RECEIVEERROR_H
