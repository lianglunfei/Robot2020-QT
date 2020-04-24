/*
 * @Descripttion: 
 * @version: 
 * @Author: xingzhang.Wu
 * @Date: 2020-01-08 15:36:56
 * @LastEditors  : Qingmao Wei
 * @LastEditTime : 2020-01-16 14:32:49
 */
#ifndef OFFLINESEQUENCECONTROL_H
#define OFFLINESEQUENCECONTROL_H

#include "sequence.h"
#include <QDialog>
#include <QComboBox>




namespace Ui {
class OfflineSequenceControl;
}

class OfflineSequenceControl : public QDialog
{
    Q_OBJECT

public:
    explicit OfflineSequenceControl(QWidget *parent = nullptr);
    void deactivateAllMoveButtons();
    void activateAllMoveButtons();

    ~OfflineSequenceControl();

private:
    enum move_ops{forword_op=0, back_op, left_op, right_op, pause_op, stand_op, sit_op};
    enum move_status{RUNNING, PAUSE};
    QPushButton* movementButtons[7];
    QComboBox* movementComboxes[4];
    Ui::OfflineSequenceControl *ui;
    QStringList actionNames;
    QMap<QString,QString> actionModels;
    // QStringList actionModels;
    SequenceExcuteWorker *seqWorker;
    void readAssetDir(const QString &dirpath);

    void keyPressEvent(QKeyEvent *e) override;
    void movementPtrInit();
    
    QStringList getFileNames(const QString &path);
    int UIWaitTask(move_ops mov);
    
    
private slots:
    void on_assetBrowseButton_clicked();
    void on_leftButton_clicked();
    void on_forwardButton_clicked();
    void on_backButton_clicked();
    void on_rightButton_clicked();
    void on_standButton_clicked();
    void on_sitButton_clicked();
    void on_stopButton_clicked();
    void on_pauseButton_clicked();
    void on_playButton_clicked();
    void on_resetButton_clicked();
    void on_openValveButton_clicked();
    void on_resetActionButton_clicked();
    void runStatus(QString s);
    void stopStatus();

    void transDirLine();

public slots:
    void pausedWhenError();

};

#endif // OFFLINESEQUENCECONTROL_H
