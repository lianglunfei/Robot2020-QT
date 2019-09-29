/*
 * @Author: xingzhang.Wu 
 * @Date: 2019-09-29 10:01:37 
 * @Last Modified by:   xingzhang.Wu 
 * @Last Modified time: 2019-09-29 10:01:37 
 */
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
