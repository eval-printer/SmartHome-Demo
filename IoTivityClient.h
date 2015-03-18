#ifndef IOTIVITYCLIENT_H
#define IOTIVITYCLIENT_H

#include <string>
#include <memory>
#include <list>
#include "SensorListenerInterface.h"

namespace IoTivity
{
    class IoTivityClient
    {
    public:
        static IoTivityClient& Instance();
        ~IoTivityClient();

        void findResource(const std::string host, const std::string resourceType);

        void addSensorAddedListener(SensorAddedListener &listener);
        void addSensorRemovedListener(SensorRemovedListener &listener);
        void addSensorChangedListener(SensorChangedListener &listener);

        std::shared_ptr< OC::OCResource > lookupSensor(std::string sensorUri) const;

    private:
        friend void updateExistSensors(const std::string key, const std::string type, const OC::OCRepresentation &rep);
        friend void getSensors(const OC::OCRepresentation &rep);
        friend void foundResource(std::shared_ptr< OC::OCResource > resource);
        friend void onGet(const OC::HeaderOptions& headerOptions, const OC::OCRepresentation& rep, const int eCode);
        friend void onPut(const OC::HeaderOptions& headerOptions, const OC::OCRepresentation &rep, const int eCode);
        friend void onObserve(const OC::HeaderOptions headerOptions, const OC::OCRepresentation& rep,
                              const int& eCode, const int& sequenceNumber);

    private:
        IoTivityClient();

        /* Dont forget to declare these two. You want to make sure they
         * are unacceptable otherwise you may accidentally get copies of
         * your singleton appearing.
         */
        IoTivityClient(IoTivityClient const&);
        void operator=(IoTivityClient const&);

        void notifySensorAdded(std::shared_ptr< OC::OCResource > resource);
        void notifySensorRemoved(std::shared_ptr< OC::OCResource > resource);
        void notifySensorChanged(const OC::OCRepresentation &representation);

    private:
        std::list< std::shared_ptr< OC::OCResource > > m_sensorList;

        std::list< SensorAddedListener* > m_sensorAddedListener;
        std::list< SensorRemovedListener* > m_sensorRemovedListener;
        std::list< SensorChangedListener* > m_sensorChangedListener;
    };

} //namespace IoTivity

#endif // IOTIVITYCLIENT_H
