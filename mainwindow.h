/*
 * @Descripttion: 
 * @version: 
 * @Author: xingzhang.Wu
 * @Date: 2019-09-26 09:53:10
 * @LastEditors  : Qingmao Wei
 * @LastEditTime : 2020-01-10 15:14:24
 */
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class ConnectDialog;
class Plots;
class Terminal;
class JointControl;
class SingleJointControl;
class OfflineControl;
class ReceiveError;
class ReceiveWorkerThread;
class RemoteControl;
class OfflineSequenceControl;
class DataServer;
class ArmControl;
class ReconfigControl;

namespace Ui
{
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;

    ConnectDialog *m_connect_dialog = nullptr;
    Plots *m_joint_widget = nullptr;
    Plots *m_joint_speed_meas_widget = nullptr;
    Terminal *m_terminal = nullptr;
    JointControl *m_joint_control = nullptr;
    RemoteControl *m_remote_control = nullptr;
    SingleJointControl *m_single_joint_control = nullptr;
    OfflineControl *m_offline_control = nullptr;
    OfflineSequenceControl *m_offline_sequence_control = nullptr;
    ReceiveError *m_receive_error = nullptr;
    ReceiveWorkerThread *workerThread = nullptr;
    ArmControl *m_arm_control = nullptr;
    DataServer *m_data_server = nullptr;
    ReconfigControl *m_reconfig_control = nullptr;
    void connectInit();

private slots:
    void canConnectEvent();
};

#endif // MAINWINDOW_H
