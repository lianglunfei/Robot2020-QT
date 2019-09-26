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
