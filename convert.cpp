/*
 * @Author: xingzhang.Wu 
 * @Date: 2019-09-29 10:01:33 
 * @Last Modified by:   xingzhang.Wu 
 * @Last Modified time: 2019-09-29 10:01:33 
 */
#include "convert.h"

/**
 * @brief Construct a new Convert:: Convert object
 * 
 */
Convert::Convert()
{
}

/**
 * @brief 字符串转换为数据串
 * 
 * @param str 要转换的字符串
 * @param data 要转换的字符串
 * @param len 数据长度
 * @param flag 
 * @return int 
 */
int Convert::strToData(unsigned char *str, unsigned char *data, int len, int flag)
{
    unsigned char cTmp = 0;
    int i = 0;
    for (int j = 0; j < len; j++)
    {
        if (charToInt(str[i++], &cTmp))
            return 1;
        data[j] = cTmp;
        if (charToInt(str[i++], &cTmp))
            return 1;
        data[j] = (data[j] << 4) + cTmp;
        if (flag == 1)
            i++;
    }
    return 0;
}

/**
 * @brief 字符转换为数据
 * 
 * @param chr 要转换的字符
 * @param cint 储存转换过来的数据
 * @return int 
 */
int Convert::charToInt(unsigned char chr, unsigned char *cint)
{
    char cTmp;
    cTmp = chr - 48;
    if (cTmp >= 0 && cTmp <= 9)
    {
        *cint = cTmp;
        return 0;
    }
    cTmp = chr - 65;
    if (cTmp >= 0 && cTmp <= 5)
    {
        *cint = (cTmp + 10);
        return 0;
    }
    cTmp = chr - 97;
    if (cTmp >= 0 && cTmp <= 5)
    {
        *cint = (cTmp + 10);
        return 0;
    }
    return 1;
}
