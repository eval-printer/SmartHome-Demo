#ifndef SENSORCELL_H
#define SENSORCELL_H

#include <memory>
#include <QWidget>
#include <QEvent>

#include "OCResource.h"
#include "SensorListenerInterface.h"

class SensorCell : public QWidget, public SensorAddedListener, public SensorRemovedListener, public SensorChangedListener
{
    Q_OBJECT
public:
    explicit SensorCell(QWidget *parent = 0);

    void setObjectName(const QString &name);

    virtual void sensorAdded(std::shared_ptr< OC::OCResource > sensor);
    virtual void sensorRemoved(std::shared_ptr< OC::OCResource > sensor);
    virtual void sensorChanged(const OC::OCRepresentation &sensor);

private:
    bool event(QEvent *event);
    void updateSensorCell(const OC::OCRepresentation &rep);

    void updateFanCell(const OC::OCRepresentation &rep);
    void updateHeatRateCell(const OC::OCRepresentation &rep);
    void updateGasCell(const OC::OCRepresentation &rep);
    void updateMotionCell(const OC::OCRepresentation &rep);
    void updateLEDCell(const OC::OCRepresentation &rep);
    void updateMinowmaxCell(const OC::OCRepresentation &rep);

private:
    class SensorChangedEvent : public QEvent
    {
    public:
        SensorChangedEvent(QEvent::Type type);

        void setSensorRepresentation(const OC::OCRepresentation &rep);

    private:
        friend class SensorCell;

    private:
        OC::OCRepresentation m_rep;
    };

signals:

public slots:
    void onFanCheckBoxClicked(bool checkedState);
    void onLEDColorChanged(QString value);
    void onButtonClicked();

private:
    std::shared_ptr< OC::OCResource > m_sensor;
    OC::OCRepresentation m_representation;
};

#endif // SENSORCEL_H
