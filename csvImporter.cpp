/*
 * @Descripttion: 
 * @version: 
 * @Author: xingzhang.Wu
 * @Date: 2020-01-10 14:43:27
 * @LastEditors  : Qingmao Wei
 * @LastEditTime : 2020-01-10 15:46:16
 */

#include <QtCore/QFile>
#include <QtCore/QTextStream>
#include <QtCore/QString>
#include "csvImporter.h"

int csvImporter(Qstring filename, int * csvFormatChecker()){
    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        return -1;
    }

    QList<QStringList> qlist;
    QStringList strlist;
    QTextStream in(&file);
}