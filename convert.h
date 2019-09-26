#ifndef CONVERT_H
#define CONVERT_H

class Convert
{
private:
    Convert();

public:
    static int charToInt(unsigned char chr, unsigned char *cint);
    static int strToData(unsigned char *str, unsigned char *data, int len, int flag);
};

#endif // CONVERT_H
