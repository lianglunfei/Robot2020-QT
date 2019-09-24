#include "convert.h"

Convert::Convert()
{

}

//-----------------------------------------------------
//参数：
//str：要转换的字符串
//data：储存转换过来的数据串
//len:数据长度
//函数功能：字符串转换为数据串
//-----------------------------------------------------
int Convert::strToData(unsigned char *str, unsigned char *data,int len,int flag)
{
    unsigned char cTmp=0;
    int i=0;
    for(int j=0;j<len;j++) {
        if(charToInt(str[i++],&cTmp))
            return 1;
        data[j]=cTmp;
        if(charToInt(str[i++],&cTmp))
            return 1;
        data[j]=(data[j]<<4)+cTmp;
        if(flag==1)
            i++;
    }
    return 0;
}

//-----------------------------------------------------
//参数：
//chr：要转换的字符
//cint：储存转换过来的数据
//函数功能：字符转换为数据
//-----------------------------------------------------
int Convert::charToInt(unsigned char chr, unsigned char *cint)
{
    char cTmp;
    cTmp=chr-48;
    if(cTmp>=0&&cTmp<=9) {
        *cint=cTmp;
        return 0;
    }
    cTmp=chr-65;
    if(cTmp>=0&&cTmp<=5) {
        *cint=(cTmp+10);
        return 0;
    }
    cTmp=chr-97;
    if(cTmp>=0&&cTmp<=5) {
        *cint=(cTmp+10);
        return 0;
    }
    return 1;
}
