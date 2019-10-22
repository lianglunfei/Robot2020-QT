#ifndef REMOTECONTROL_H
#define REMOTECONTROL_H

#include <QDialog>
#include <QQmlComponent>
#include <QQmlApplicationEngine>

#include "autocontrol.h"

class RemoteControl : public QDialog
{
    Q_OBJECT

public:
    RemoteControl(QDialog *parent = nullptr);
    ~RemoteControl();

public slots:
    void show();

private:
    QObject *qmlControl1 = nullptr;
    QObject *qmlControl2 = nullptr;
    QQmlApplicationEngine engine;
    AutoControl *ac = nullptr;

private slots:
    void leftJoySlot(const double &speed);
    void rightJoySlot(const double &angle);
    void qmlStartSlot();
    void qmlL1Slot();
    void qmlASlot(bool press);
    void qmlBSlot(bool press);
};

#endif // REMOTECONTROL_H
