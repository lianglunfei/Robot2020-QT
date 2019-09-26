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
