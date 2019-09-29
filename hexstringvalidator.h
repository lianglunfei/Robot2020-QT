/*
 * @Author: xingzhang.Wu 
 * @Date: 2019-09-29 10:02:38 
 * @Last Modified by:   xingzhang.Wu 
 * @Last Modified time: 2019-09-29 10:02:38 
 */
#ifndef HEXSTRINGVALIDATOR_H
#define HEXSTRINGVALIDATOR_H

#include <QValidator>

enum
{
    MaxPayload = 8,
    MaxPayloadFd = 64
};

class HexStringValidator : public QValidator
{
    Q_OBJECT

public:
    explicit HexStringValidator(QObject *parent = nullptr);

    QValidator::State validate(QString &input, int &pos) const;

    void setMaxLength(int maxLength);

private:
    int m_maxLength = 0;
};

#endif // HEXSTRINGVALIDATOR_H
