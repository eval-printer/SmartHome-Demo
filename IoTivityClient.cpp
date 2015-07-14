#include <mutex>
#include "OCPlatform.h"
#include "IoTivity.h"
#include "IoTivityClient.h"
#include "octypes.h"

namespace IoTivity
{
    std::mutex resourceAddedMutex;

    int densityDefineValue = 15;

    void updateExistSensors(const std::string key, const std::string type, const OC::OCRepresentation &rep)
    {
        std::string sensorUri;
        std::shared_ptr< OC::OCResource > resource;

        resource = IoTivityClient::Instance().lookupSensor(type);
        if (rep.hasAttribute(key))
        {
            if (resource == nullptr)
            {
                /* 'key' is added */
                std::cout << "Added resource " << type << std::endl;
                rep.getValue(key, sensorUri);
                OC::OCPlatform::findResource("", sensorUri, ::OC_ALL, foundResource);
            }
        }
        else
        {
            if (resource != nullptr)
            {
                std::cout << "Rmove resource " << resource->uri() << std::endl;
                /* 'key' is removed */
                IoTivityClient::Instance().notifySensorRemoved(resource);
            }
        }
    }

    void updateSensors(const OC::OCRepresentation &rep)
    {
        updateExistSensors("fan", IOTIVITY_FAN, rep);
        updateExistSensors("gas", IOTIVITY_GAS, rep);
        updateExistSensors("led", IOTIVITY_LED, rep);
        updateExistSensors("pri", IOTIVITY_MOTION, rep);
        updateExistSensors("heartRate", IOTIVITY_HEARTRATE, rep);
    }

    void onObserve(const OC::HeaderOptions headerOptions, const OC::OCRepresentation& rep,
                   const int& eCode, const int& sequenceNumber)
    {
        try
        {
            if(eCode == OC_STACK_OK)
            {
                std::cout << "Observe Resource representation " << rep.getUri() << std::endl;

                if (rep.getUri() == IOTIVITY_GATWAY)
                    updateSensors(rep);
                else
                    IoTivityClient::Instance().notifySensorChanged(rep);
            }
            else
            {
                std::cout << "onObserve Response error: " << eCode << std::endl;
            }
        }
        catch (std::exception& e)
        {
             std::cout << "Exception: " << e.what() << " in onObserve" << std::endl;
        }
    }

    void onPut(const OC::HeaderOptions& headerOptions, const OC::OCRepresentation &rep, const int eCode)
    {
        try
        {
            if(eCode == OC_STACK_OK)
            {
                std::cout << "Observe Resource representation " << rep.getUri() << std::endl;

                IoTivityClient::Instance().notifySensorChanged(rep);
            }
            else
            {
                std::cout << "onPut Response error: " << eCode << std::endl;
            }
        }
        catch (std::exception& e)
        {
             std::cout << "Exception: " << e.what() << " in onPut" << std::endl;
        }
    }

    void onGet(const OC::HeaderOptions& headerOptions, const OC::OCRepresentation &rep, const int eCode)
    {
        try
        {
            if(eCode == OC_STACK_OK)
            {
                std::cout << "Get Resource Represention" << rep.getUri() << std::endl;

                if (rep.getUri() == IOTIVITY_GATWAY)
                    updateSensors(rep);
                else
                    IoTivityClient::Instance().notifySensorChanged(rep);
            }
            else
            {
                std::cout << "onGET Response error: " << eCode << std::endl;
            }
        }
        catch (std::exception& e)
        {
            std::cout << "Get Resource failed " << e.what() << std::endl;
        }
    }

    void foundResource(std::shared_ptr< OC::OCResource > resource)
    {
        std::unique_lock< std::mutex > resourceAddedLock(resourceAddedMutex);

        try
        {
            if (resource)
            {
                std::cout << "foundResource URI: " << resource->uri() << " hostAddress: " <<  resource->host() << std::endl;

                for (auto &resourceTypes : resource->getResourceTypes())
                {
                    std::cout << "resourceTypes: " << resourceTypes << std::endl;
                }

                for(auto &resourceInterfaces : resource->getResourceInterfaces())
                {
                    std::cout << "resourceInterfaces: " << resourceInterfaces << std::endl;
                }

                OC::QueryParamsMap test;
                resource->get(test, onGet);

                if (IoTivityClient::Instance().lookupSensor(resource->uri()) == nullptr)
                {
                    resource->observe(OC::ObserveType::Observe, OC::QueryParamsMap(), onObserve);

                    IoTivityClient::Instance().notifySensorAdded(resource);
                }
            }
        }
        catch (std::exception& e)
        {
            std::cout << "Resource Invalid " << e.what() << std::endl;
        }
    }

    IoTivityClient& IoTivityClient::Instance()
    {
        static IoTivityClient instance;

        return instance;
    }

    IoTivityClient::IoTivityClient()
    {
        OC::PlatformConfig cfg {
            OC::ServiceType::InProc,
            OC::ModeType::Client,
            "0.0.0.0",
            0,
            OC::QualityOfService::LowQos
        };

        OC::OCPlatform::Configure(cfg);
    }

    IoTivityClient::~IoTivityClient()
    {
        std::list< std::shared_ptr< OC::OCResource > >::iterator iter;

        for (iter = m_sensorList.begin(); iter != m_sensorList.end(); ++iter)
        {
            std::shared_ptr< OC::OCResource > resource = *iter;
            resource->cancelObserve(OC::QualityOfService::HighQos);
        }
    }

    void IoTivityClient::findResource(const std::string host, const std::string resourceType)
    {
        std::ostringstream requestURI;
        try
        {
            // makes it so that all boolean values are printed as 'true/false' in this stream
            std::cout.setf(std::ios::boolalpha);
            // Find all resources
            requestURI << OC_MULTICAST_DISCOVERY_URI << "?rt=" << resourceType;
            OC::OCPlatform::findResource("", requestURI.str(), OC_ALL, &foundResource);
            std::cout<< "Finding Resource... " <<std::endl;
        }
        catch (OC::OCException& e)
        {

        }
    }

    std::shared_ptr< OC::OCResource > IoTivityClient::lookupSensor(std::string sensorUri) const
    {
        std::list< std::shared_ptr< OC::OCResource > >::const_iterator iter;

        for (iter = m_sensorList.begin(); iter != m_sensorList.end(); ++iter)
        {
            const std::shared_ptr< OC::OCResource > resource = *iter;
            if (resource->uri() == sensorUri)
                return resource;
        }

        return nullptr;
    }

    void IoTivityClient::notifySensorAdded(std::shared_ptr< OC::OCResource > resource)
    {
        std::list< SensorAddedListener* >::iterator iter;

        m_sensorList.push_back(resource);

        for (iter = m_sensorAddedListener.begin(); iter != m_sensorAddedListener.end(); ++iter)
        {
            SensorAddedListener *listener = *iter;
            listener->sensorAdded(resource);
        }
    }

    void IoTivityClient::notifySensorRemoved(std::shared_ptr< OC::OCResource > resource)
    {
        std::list< SensorRemovedListener* >::iterator iter;
        std::list< std::shared_ptr< OC::OCResource > >::iterator sensorIter;

        for (iter = m_sensorRemovedListener.begin(); iter != m_sensorRemovedListener.end(); ++iter)
        {
            SensorRemovedListener *listener = *iter;
            listener->sensorRemoved(resource);
        }

        /* Remove resource from sensorList */
        for (sensorIter = m_sensorList.begin(); sensorIter != m_sensorList.end(); ++sensorIter)
        {
            std::shared_ptr< OC::OCResource > res = *sensorIter;
            if (res->uri() == resource->uri())
                m_sensorList.erase(sensorIter++);
        }
    }

    void IoTivityClient::notifySensorChanged(const OC::OCRepresentation &representation)
    {
        std::list< SensorChangedListener* >::iterator iter;

        for (iter = m_sensorChangedListener.begin(); iter != m_sensorChangedListener.end(); ++iter)
        {
            SensorChangedListener *listener = *iter;
            listener->sensorChanged(representation);
        }
    }

    void IoTivityClient::addSensorAddedListener(SensorAddedListener &listener)
    {
        m_sensorAddedListener.push_back(&listener);
    }

    void IoTivityClient::addSensorRemovedListener(SensorRemovedListener &listener)
    {
        m_sensorRemovedListener.push_back(&listener);
    }

    void IoTivityClient::addSensorChangedListener(SensorChangedListener &listener)
    {
        m_sensorChangedListener.push_back(&listener);
    }

} //namespace IoTivity
