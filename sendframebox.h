#ifndef SENDFRAMEBOX_H
#define SENDFRAMEBOX_H

#include <QGroupBox>

namespace Ui {
class SendFrameBox;
}

class HexIntegerValidator;
class HexStringValidator;
class QTimer;

class SendFrameBox : public QGroupBox
{
    Q_OBJECT

public:
    explicit SendFrameBox(QWidget *parent = 0);
    ~SendFrameBox();

private slots:
    void on_sendButton_clicked();
    void repeatSendProcess();

    void on_repeatSendCheckBox_stateChanged(int arg1);

    void on_receiveMsgCheckBox_stateChanged(int arg1);

private:
    void sendFrameData();

private:
    Ui::SendFrameBox *ui;

    HexIntegerValidator *m_hexIntegerValidator = nullptr;
    HexStringValidator *m_hexStringValidator = nullptr;
    QTimer *repeatTimer = nullptr;

    unsigned char currentData[8]={0,0,0,0,0,0,0,0};
    unsigned int currentId=0;
};

#endif // SENDFRAMEBOX_H
