/*
 * @Author: xingzhang.Wu 
 * @Date: 2019-09-29 10:03:48 
 * @Last Modified by:   xingzhang.Wu 
 * @Last Modified time: 2019-09-29 10:03:48 
 */
#ifndef PACKAGE_H
#define PACKAGE_H

class Package
{
private:
    Package();

public:
    static bool unpackOperate();
    static bool packOperate(unsigned int id, double data, int type);
    static bool packOperateMulti(unsigned int *id, double *data, int num, int type);
};

#endif // PACKAGE_H
