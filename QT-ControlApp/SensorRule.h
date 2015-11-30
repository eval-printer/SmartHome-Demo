#ifndef SENSORRULE_H
#define SENSORRULE_H

#include <memory>
#include <QWidget>
#include <QEvent>

#include "OCResource.h"
#include "SensorListenerInterface.h"

class QCheckBox;
class QComboBox;

class SensorRule : public QWidget, public SensorAddedListener, public SensorRemovedListener, public SensorChangedListener
{
    Q_OBJECT
public:
    explicit SensorRule(QWidget *parent = 0);

    void setObjectName(const QString &name);

    virtual void sensorAdded(std::shared_ptr< OC::OCResource > sensor);
    virtual void sensorRemoved(std::shared_ptr< OC::OCResource > sensor);
    virtual void sensorChanged(const OC::OCRepresentation &sensor);

private:
    bool event(QEvent *event);
    void updateRuleView(const OC::OCRepresentation &rep);
    void updateKitchMonitorState(bool value);
    void updateCrazyJumpingState(bool value);
    void updateDefineDensity(int value);
    void updateDefineHeartRate(int value);

private:
    class RuleChangedEvent : public QEvent
    {
    public:
        RuleChangedEvent(QEvent::Type type);

        void setSensorRepresentation(const OC::OCRepresentation &rep);

    private:
        friend class SensorRule;

    private:
        OC::OCRepresentation m_rep;
    };

signals:

public slots:
    void onKitchChecked(bool checkedState);
    void onCrazyJumpingChecked(bool checkedState);
    void onDensitySelectChanged(QString value);
    void onHeartRateSelectChanged(QString value);

private:
    QCheckBox *m_kitchGas, *m_crazyJumping;
    QComboBox *m_densitySelect, *m_heartRateSelect;

    std::shared_ptr< OC::OCResource > m_resource;
};

#endif // SENSORRULE_H
