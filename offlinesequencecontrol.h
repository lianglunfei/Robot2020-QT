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
};

#endif // OFFLINESEQUENCECONTROL_H
