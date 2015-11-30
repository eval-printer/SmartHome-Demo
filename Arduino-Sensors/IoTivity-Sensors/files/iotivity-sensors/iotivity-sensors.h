//******************************************************************
//
// Copyright 2014 Intel Corporation.
//
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//      http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

#ifndef IOTIVITY_SENSORS_H_
#define IOTIVITY_SENSORS_H_

#include <string>
#include <iostream>
#include <memory>
#include "ocstack.h"
#include "OCPlatform.h"
#include "OCApi.h"
#include "mraa.h"

using namespace std;
using namespace OC;

#define FAN_RESOURCE_URI "/a/fan"
#define FAN_RESOURCE_TYPE "intel.fan"
#define GAS_RESOURCE_URI "/sensor/gas"
#define GAS_RESOURCE_TYPE "intel.gas"
#define PIR_RESOURCE_URI "/sensor/pri"
#define PIR_RESOURCE_TYPE "intel.pir"
#define HG_DISCOVER_RESOURCE_TYPE "gw.sensor"
#define HG_CONFIGURATION_RESOURCE_TYPE "gw.config"
#define PRESENCE_CYCLE 2

#define FANPIN 9
#define GASPIN 0
#define PIRPIN 2

class Resource
{
public:
	Resource();
	OCStackResult notify(void);
	bool inPresence;
	guint presenceTimer;
	void startPresence(unsigned int interval);
	void stopPresence(void);
protected:
	OCResourceHandle m_resourceHandle;
	OCRepresentation m_rep;
	ObservationIds m_interestedObservers;
	OCEntityHandlerResult entityHandler(std::shared_ptr<OCResourceRequest> request);
	virtual OCRepresentation get(void) = 0;
	virtual void put(OCRepresentation& rep) = 0;
};

class FanResource : public Resource
{
public:
	FanResource();
	~FanResource();
	bool setup_hardware(void);
	void createResource();
private:
	bool m_fanState;
	mraa_gpio_context m_pin;
protected:
	OCRepresentation get(void);
	void put(OCRepresentation& rep); 
};

class GasResource : public Resource
{
public:
	GasResource();
	~GasResource();
	bool setup_hardware(void);
	void createResource();
private:
	int m_density;
	mraa_aio_context m_pin;
protected:
	OCRepresentation get(void);
	void put(OCRepresentation& rep){};
};

class PirResource : public Resource
{
public:
	PirResource();
	~PirResource();
	bool setup_hardware(void);
	void createResource();
private:
	bool m_motion;
	mraa_gpio_context m_pin;
protected:
	OCRepresentation get(void);
	void put(OCRepresentation& rep){};
};


#endif /* IOTIVITY_SENSORS_H_ */
