//******************************************************************
//
// Copyright 2014 Intel Mobile Communications GmbH All Rights Reserved.
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

#ifndef SENSOR_RESOURCE_H_
#define SENSOR_RESOURCE_H_
#include "resource.h"
#include "rules_resource.h"
#include "homegateway.h"

class SensorResource : public Resource
{
	public:
	SensorMap m_sensorMap;
	std::string m_sensorName, m_sensorAddr;
	sensor_data m_gas, m_fan, m_led, m_pri;
	RulesResource *m_rr;
	bool m_fanState;
	std::mutex m_resourceLock;
	int m_observation;
	ObservationIds m_interestedObservers;
	SensorResource(RulesResource *rr);
	bool status_check(sensor_data &sensor);
	void activeCheck();
	void resetStatus();
	void registerBle(char *url);
	void unregisterBle();
	void executeJumpingRule(int heartRate);
	private:
	void onPut(const HeaderOptions& headerOptions, const OCRepresentation& rep, const int eCode);
	void onGet(const HeaderOptions& headerOptions, const OCRepresentation& rep, const int eCode);
	void executeGasRule(int density);
	void onObserve(const HeaderOptions headerOptions, const OCRepresentation& rep,
		const int& eCode, const int& sequenceNumber);
	void foundResource(std::shared_ptr<OCResource> resource);
	void StartMonitor(std::string address);
	OCRepresentation get();
	void put(OCRepresentation rep);
	void ChangeSensorRepresentation();
	protected:
	virtual OCEntityHandlerResult entityHandler(std::shared_ptr<OCResourceRequest> request);
};

#endif /* SENSOR_RESOURCE_H_ */
