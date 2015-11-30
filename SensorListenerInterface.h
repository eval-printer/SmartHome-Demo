#ifndef SENSORLISTENERINTERFACE_H
#define SENSORLISTENERINTERFACE_H

#include <memory>

namespace OC {
    class OCResource;
    class OCRepresentation;
}

class SensorAddedListener
{
public:
    ~SensorAddedListener() {}

    virtual void sensorAdded(std::shared_ptr< OC::OCResource > sensor) = 0;
};

class SensorRemovedListener
{
public:
    ~SensorRemovedListener() {}

    virtual void sensorRemoved(std::shared_ptr< OC::OCResource > sensor) = 0;
};

class SensorChangedListener
{
public:
    ~SensorChangedListener() {}

    virtual void sensorChanged(const OC::OCRepresentation &sensor) = 0;
};

#endif // SENSORLISTENERINTERFACE_H
