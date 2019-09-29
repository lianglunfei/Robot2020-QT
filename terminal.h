/*
 * @Author: xingzhang.Wu 
 * @Date: 2019-09-29 10:05:17 
 * @Last Modified by:   xingzhang.Wu 
 * @Last Modified time: 2019-09-29 10:05:17 
 */
#ifndef TERMINAL_H
#define TERMINAL_H

#include <QWidget>

namespace Ui
{
class Terminal;
}

class Terminal : public QWidget
{
    Q_OBJECT

public:
    explicit Terminal(QWidget *parent = 0);
    ~Terminal();

private:
    Ui::Terminal *ui;
};

#endif // TERMINAL_H
