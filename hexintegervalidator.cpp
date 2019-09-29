/*
 * @Author: xingzhang.Wu 
 * @Date: 2019-09-29 10:02:28 
 * @Last Modified by:   xingzhang.Wu 
 * @Last Modified time: 2019-09-29 10:02:28 
 */
#include "hexintegervalidator.h"

HexIntegerValidator::HexIntegerValidator(QObject *parent) : QValidator(parent),
                                                            m_maximum(MaxStandardId)
{
}

QValidator::State HexIntegerValidator::validate(QString &input, int &) const
{
    bool ok;
    uint value = input.toUInt(&ok, 16);

    if (!value)
        return Intermediate;

    if (!ok || value > m_maximum)
        return Invalid;

    return Acceptable;
}

void HexIntegerValidator::setMaximum(uint maximum)
{
    m_maximum = maximum;
}
