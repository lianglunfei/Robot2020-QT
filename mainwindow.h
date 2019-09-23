#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class ConnectDialog;
class JointPlot;
class JointSpeedMeas;
class Terminal;
class JointControl;
class SingleJointControl;
class OfflineControl;
class ReceiveError;
class ReceiveWorkerThread;

namespace Ui {
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
    JointPlot *m_joint_widget = nullptr;
    JointSpeedMeas *m_joint_speed_meas_widget = nullptr;
    Terminal *m_terminal = nullptr;
    JointControl *m_joint_control = nullptr;
    SingleJointControl *m_single_joint_control = nullptr;
    OfflineControl *m_offline_control = nullptr;
    ReceiveError *m_receive_error = nullptr;
    ReceiveWorkerThread *workerThread = nullptr;
    void connectInit();

private slots:
    void canConnectEvent();
};

#endif // MAINWINDOW_H
