/*
 * @Descripttion: 
 * @version: 
 * @Author: xingzhang.Wu
 * @Date: 2020-01-08 15:36:56
 * @LastEditors  : Qingmao Wei
 * @LastEditTime : 2020-01-13 09:56:21
 */
#ifndef OFFLINESEQUENCECONTROL_H
#define OFFLINESEQUENCECONTROL_H

#include <QDialog>

namespace Ui {
class OfflineSequenceControl;
}

class OfflineSequenceControl : public QDialog
{
    Q_OBJECT

public:
    explicit OfflineSequenceControl(QWidget *parent = nullptr);
    

    ~OfflineSequenceControl();

private:
    Ui::OfflineSequenceControl *ui;
    QStringList actionNames;

    // void keyPressEvent(QKeyEvent *e) override;
    QStringList getFileNames(const QString &path);
    
    
private slots:
    void on_assetBrowseButton_clicked();
    void on_leftButton_clicked();
    void on_forwardButton_clicked();
    void on_backButton_clicked();
    void on_rightButton_clicked();
    void on_stopButton_clicked();

};

#endif // OFFLINESEQUENCECONTROL_H
