/*
 * @Author: xingzhang.Wu 
 * @Date: 2019-09-29 10:05:13 
 * @Last Modified by:   xingzhang.Wu 
 * @Last Modified time: 2019-09-29 10:05:13 
 */
#include "terminal.h"
#include "ui_terminal.h"

/**
 * @brief Construct a new Terminal:: Terminal object
 * 
 * @param parent 
 */
Terminal::Terminal(QWidget *parent) : QWidget(parent),
                                      ui(new Ui::Terminal)
{
    ui->setupUi(this);
    this->setWindowTitle(tr("Terminal"));
}

/**
 * @brief Destroy the Terminal:: Terminal object
 * 
 */
Terminal::~Terminal()
{
    delete ui;
}
