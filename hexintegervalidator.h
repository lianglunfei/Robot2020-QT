/*
 * @Author: xingzhang.Wu 
 * @Date: 2019-09-29 10:02:31 
 * @Last Modified by:   xingzhang.Wu 
 * @Last Modified time: 2019-09-29 10:02:31 
 */
#ifndef HEXINTEGERVALIDATOR_H
#define HEXINTEGERVALIDATOR_H

#include <QValidator>

enum
{
    MaxStandardId = 0x7FF,
    MaxExtendedId = 0x10000000
};

class HexIntegerValidator : public QValidator
{
    Q_OBJECT
public:
    explicit HexIntegerValidator(QObject *parent = nullptr);
    QValidator::State validate(QString &input, int &) const;

    void setMaximum(uint maximum);

private:
    uint m_maximum = 0;
};

#endif // HEXINTEGERVALIDATOR_H
