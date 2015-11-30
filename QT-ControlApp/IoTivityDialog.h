#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>

class QLabel;
class QGroupBox;
class QVBoxLayout;
class SensorCell;
class SensorRule;

namespace IoTivity {
    class IoTivityClient;
}

class IoTivityDialog : public QDialog
{
    Q_OBJECT

public:
    IoTivityDialog(QWidget *parent = 0);
    ~IoTivityDialog();

private:
    void changeStyle(const QString &styleName);
    QGroupBox *createSensorsGroupBox();
    QGroupBox *createRuleGroupBox();

private:
    QLabel *m_title;
    QGroupBox *m_sensorsGroupBox;
    QGroupBox *m_ruleGroupBox;

    SensorCell *m_fanCell, *m_heartRateCell, *m_gasCell, *m_ledCell;

    SensorRule *m_ruleView;

    QVBoxLayout *m_mainLayout;

    IoTivity::IoTivityClient *m_pIoTivityClient;
};

#endif // DIALOG_H
