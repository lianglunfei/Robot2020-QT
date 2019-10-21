#ifndef AUTOCONTROL_H
#define AUTOCONTROL_H

class AutoControl
{
public:
    AutoControl();

    int run();

private:
    int moveLeg(int leg, double changePos[], double v);
    int moveBody(double changePos[], double v);
};

#endif // AUTOCONTROL_H
