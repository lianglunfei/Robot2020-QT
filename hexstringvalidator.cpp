#include "hexstringvalidator.h"

HexStringValidator::HexStringValidator(QObject *parent) : QValidator(parent),
                                                          m_maxLength(MaxPayload)
{
}

QValidator::State HexStringValidator::validate(QString &input, int &pos) const
{
    const int maxSize = 2 * m_maxLength;
    const QChar space = QLatin1Char(' ');
    QString data = input;
    data.remove(space);

    if (data.isEmpty())
        return Intermediate;

    // limit maximum size and forbid trailing spaces
    if ((data.size() > maxSize) || (data.size() == maxSize && input.endsWith(space)))
        return Invalid;

    // check if all input is valid
    const QRegularExpression re(QStringLiteral("^[[:xdigit:]]*$"));
    if (!re.match(data).hasMatch())
        return Invalid;

    // insert a space after every two hex nibbles
    const QRegularExpression insertSpace(QStringLiteral("(?:[[:xdigit:]]{2} )*[[:xdigit:]]{3}"));
    if (insertSpace.match(input).hasMatch())
    {
        input.insert(input.size() - 1, space);
        pos = input.size();
    }

    return Acceptable;
}

void HexStringValidator::setMaxLength(int maxLength)
{
    m_maxLength = maxLength;
}
