#ifndef AUTOCONTROL_H
#define AUTOCONTROL_H

#include <QObject>
#include <QTimer>

class AutoControl : public QObject
{
    Q_OBJECT

public:
    explicit AutoControl(QObject *parent = nullptr);

    void run();
    void moveLegSet(int leg, double changePos[], double v, int mode);
    void moveBodySet(double changePos[], double v, int mode);
    void reset();
    void stop();

public slots:
    void moveFunc();

private:
    QTimer *timer;
    int period;
    int flag;
    double l1, l2, l3;
    int leg;
    double changePos[3] = {0, 0, 0};
    double v;
    int mode;
    int useBody;
    int isSetting;

    void moveLeg();
    void moveBody();
};

#endif // AUTOCONTROL_H
