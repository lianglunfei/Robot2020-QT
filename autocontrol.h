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
    int moveLegSet(int leg, double changePos[], double v, int mode);
    int moveBody(double changePos[], double v);
    void reset();
    void stop();

public slots:
    int moveLeg();

private:
    QTimer *timer;
    int period;
    int flag;
    double l1, l2, l3;
    int leg;
    double changePos[3]={0,0,0};
    double v;
    int mode;
};

#endif // AUTOCONTROL_H
