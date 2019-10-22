#ifndef AUTOCONTROL_H
#define AUTOCONTROL_H

#include <QObject>
#include <QTimer>

class AutoControl : public QObject
{
    Q_OBJECT

public:
    explicit AutoControl(QObject *parent = nullptr);

    int run();
    int moveLeg(int leg, double changePos[], double v);
    int moveBody(double changePos[], double v);

private:
    QTimer *timer;
    int period;
    double l1, l2, l3;
};

#endif // AUTOCONTROL_H
