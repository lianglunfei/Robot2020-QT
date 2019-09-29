/*
 * @Author: xingzhang.Wu 
 * @Date: 2019-09-29 10:05:13 
 * @Last Modified by:   xingzhang.Wu 
 * @Last Modified time: 2019-09-29 10:05:13 
 */
#include "terminal.h"
#include "ui_terminal.h"

Terminal::Terminal(QWidget *parent) : QWidget(parent),
                                      ui(new Ui::Terminal)
{
    ui->setupUi(this);
    this->setWindowTitle(tr("Terminal"));
}

Terminal::~Terminal()
{
    delete ui;
}
