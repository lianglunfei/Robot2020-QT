#ifndef AUTOCONTROL_H
#define AUTOCONTROL_H

#include <QObject>
#include <QTimer>

class AutoControl : public QObject
{
    Q_OBJECT

public:
    explicit AutoControl(QObject *parent = nullptr);

    void moveLegSet(int leg, double changePos[], double v, int mode);
    void moveBodySet(double changePos[], double v, int mode);
    void autoRunSet();
    void start();
    void stop();

public slots:
    void run();

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
    void autoRunFunc();
};

#endif // AUTOCONTROL_H
