#ifndef DRIVERS_H
#define DRIVERS_H


class Drivers
{
private:
    Drivers();

public:
    static void initJoint();
    static void stopJoint();
    static void initJoint(int nodeNum);
    static void stopJoint(int nodeNum);
};

#endif // DRIVERS_H
