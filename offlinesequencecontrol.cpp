/*
 * @Descripttion: 
 * @version: 
 * @Author: xingzhang.Wu
 * @Date: 2020-01-08 15:36:56
 * @LastEditors  : Qingmao Wei
 * @LastEditTime : 2020-01-13 09:59:42
 */
/*
 * @Descripttion: 
 * @version: 
 * @Author: xingzhang.Wu
 * @Date: 2020-01-08 15:36:56
 * @LastEditors  : Qingmao Wei
 * @LastEditTime : 2020-01-10 15:52:38
 */
#include "offlinesequencecontrol.h"
#include "ui_offlinesequencecontrol.h"
#include "debug.h"
#include "drivers.h"
#include <QFileDialog>
#include <QKeyEvent>
// #include <QWidget>

OfflineSequenceControl::OfflineSequenceControl(QWidget *parent) : QDialog(parent),
                                                                  ui(new Ui::OfflineSequenceControl)
{
    setFocusPolicy(Qt::StrongFocus);
    ui->setupUi(this);
    ui->forwardButton->setShortcut(Qt::Key_Up);
    ui->leftButton->setShortcut(Qt::Key_Left);
    ui->backButton->setShortcut(Qt::Key_Down);
    ui->rightButton->setShortcut(Qt::Key_Right);
    ui->stopButton->setShortcut(QKeySequence(Qt::ALT + Qt::Key_A));
}

/**
 * @msg: 获取路径下所有.csv的文件名
 * @param path 要获取的路径
 * @return: 文件名组成的QstringList
 */
QStringList OfflineSequenceControl::getFileNames(const QString &path)
{
    QDir dir(path);
    QStringList nameFilters;
    nameFilters << "*.csv";
    QStringList files = dir.entryList(nameFilters, QDir::Files | QDir::Readable, QDir::Name);
    return files;
}


/**
 * @msg: 读取动作库
 */
void OfflineSequenceControl::on_assetBrowseButton_clicked()
{
    qDebug() << "In file explorer.";

    //定义文件对话框类
    QFileDialog *fileDialog = new QFileDialog(this);
    QString dirpath = fileDialog->getExistingDirectory(this, QString("选择动作库文件夹"), QString("./"), QFileDialog::ShowDirsOnly);
    // 检查路径是否为空（即 未选择）
    if (dirpath.isEmpty()){
        qDebug() << "Nothing.";
        return;
    }
    // 获取路径下所有文件名
    QStringList fileNames = getFileNames(dirpath); 
    qDebug() << fileNames;

    // 存入 动作名 数组
    for(int iter = 0; iter < fileNames.size(); iter++){
        QFileInfo info(dirpath + '/'+ fileNames[iter]);
        if (info.exists()){
            qDebug()<<info.baseName();
            actionNames.append(info.baseName());
        }
    }
    ui->backComboBox->addItems(actionNames);
    ui->forwardComboBox->addItems(actionNames);
    ui->leftComboBox->addItems(actionNames);
    ui->rightComboBox->addItems(actionNames);
    
    


    // TODO: 读取这些文件 检查csv文件格式

    // 存入 文件指针 数组 
    
    

}


void OfflineSequenceControl::on_leftButton_clicked(){
    ui->leftButton->setDefault(1);
}
void OfflineSequenceControl::on_forwardButton_clicked(){
    ui->forwardButton->setDefault(1);
}
void OfflineSequenceControl::on_backButton_clicked(){
    ui->backButton->setDefault(1);
}
void OfflineSequenceControl::on_rightButton_clicked(){
    ui->rightButton->setDefault(1);
}
void OfflineSequenceControl::on_stopButton_clicked(){
    ui->stopButton->setDefault(1);
}

OfflineSequenceControl::~OfflineSequenceControl()
{
    delete ui;
}

// void OfflineSequenceControl::keyPressEvent(QKeyEvent *e)
// {
//     qDebug() << "You typed " << e->key();
//     switch (e->key())
//     {
//     case Qt::Key_A:
//     {
//         if (QApplication::keyboardModifiers() == Qt::AltModifier)
//         {
//             on_stopButton_clicked();
//         }
//         break;
//     }
//     case Qt::Key_Up:{
//         on_forwardButton_clicked();
//         break;
//     }
//     case Qt::Key_Down:{
//         on_backButton_clicked();
//         break;
//     }
//     case Qt::Key_Left:{
//         on_leftButton_clicked();
//         break;
//     }
//     case Qt::Key_Right:{
//         on_rightButton_clicked();
//         break;
//     }
//     }
// }
