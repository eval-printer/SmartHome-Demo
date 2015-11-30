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

#include "sensor_resource.h"
#include "ble_hr_sensor.h"

SensorResource::SensorResource(RulesResource *rr) : m_sensorName(""), m_sensorAddr(""), m_observation(0)
{
	std::string resourceURI = SENSOR_RESOURCE_ENDPOINT;
	std::string resourceTypeName = SENSOR_RESOURCE_TYPE;
	std::string resourceInterface = DEFAULT_INTERFACE;
	EntityHandler cb = std::bind(&SensorResource::entityHandler, this,PH::_1);

	m_rr = rr;

	uint8_t resourceProperty = OC_DISCOVERABLE | OC_OBSERVABLE;
	OCStackResult result = OCPlatform::registerResource(m_resourceHandle,
	resourceURI,
	resourceTypeName,
	resourceInterface,
	cb,
	resourceProperty);

	if(OC_STACK_OK != result) {
		throw std::runtime_error(
			std::string("Sensors Resource failed to start")+std::to_string(result));
	}
}

bool SensorResource::status_check(sensor_data &sensor)
{
	SensorMapIter iter;
	iter = m_sensorMap.find(sensor.s_name);
	if ((sensor.s_active == false) && (sensor.s_resource != nullptr)) {
		std::cout << "Sensor: " << sensor.s_name << " is offline" <<std::endl;
		if (iter != m_sensorMap.end()) {
			m_sensorMap.erase(iter);
			sensor.s_resource = nullptr;
		}

		return true;
	}
	else if ((sensor.s_active == true) && (sensor.s_resource == nullptr)) {
		//should not have this case, maybe there is a long delay
		//of notification packet, we need to refind the resource
		std::string address;
		if (iter != m_sensorMap.end()) {
			address = std::string(iter->second);
			StartMonitor(address);
		}
		sensor.s_active = false;
	}

	return false;
}

void SensorResource::activeCheck()
{
	bool sensor_changed = false;

	sensor_changed = status_check(m_gas) || status_check(m_fan)
			|| status_check(m_led) || status_check(m_pri);

	if (sensor_changed) {
		ChangeSensorRepresentation();
	}
}

void SensorResource::resetStatus()
{
	std::cout << "Reset sensor active status" <<std::endl;
	m_fan.s_active = false;
	m_gas.s_active = false;
	m_led.s_active = false;
	m_pri.s_active = false;
}

void SensorResource::registerBle(char *url)
{/*
	uint8_t ifname[] = "eth0";
	uint8_t ipAddr[20];
	char address[64] = {0};

	OCGetInterfaceAddress(ifname, sizeof(ifname), AF_INET, ipAddr, sizeof(ipAddr));
	snprintf(address, sizeof(address), "coap://%s:8888/oc/core?rt=com.intel", (char*)ipAddr);

	std::cout << "BLE heart rate address" << address <<std::endl;
*/
	m_sensorMap.insert(SensorMap::value_type("heartRate", BLE_HR_SENSOR_RESOURCE_TYPE));
	ChangeSensorRepresentation();
}

void SensorResource::unregisterBle()
{
	m_sensorMap.erase("heartRate");

	ChangeSensorRepresentation();
}

void SensorResource::executeJumpingRule(int heartRate)
{
	std::cout << "Crazy Jumping enable flag: " << m_rr->m_crazyJumping << "current heartRate: " << heartRate << std::endl;
	if (m_rr->m_crazyJumping && m_led.s_active && m_led.s_resource) {
		OCRepresentation ledrep;
		PutCallback p (std::bind(&SensorResource::onPut, this, PH::_1, PH::_2, PH::_3));
		if (heartRate >= m_rr->m_heartRate){
			ledrep.setValue("ledColor", RED);
		} else {
			ledrep.setValue("ledColor", GREEN);

		}

		m_led.s_resource->put(ledrep, QueryParamsMap(), p);
	}
}

void SensorResource::onPut(const HeaderOptions& headerOptions, const OCRepresentation& rep, const int eCode)
{
	try {
		if(eCode == OC_STACK_OK) {
			if (rep.hasAttribute("fanstate")) {
				std::string state = rep.getValue<std::string>("fanstate");
				m_fanState = (state == "on" ? true:false);
				std::cout << "\tfanstate: " << m_fanState << std::endl;
			}
		}
		else {
			std::cout << "onPut Response error: " << eCode << std::endl;
		}
	}
	catch(std::exception& e) {
		std::cout << "Exception: " << e.what() << " in onPut" << std::endl;
	}
}

void SensorResource::onGet(const HeaderOptions& headerOptions, const OCRepresentation& rep, const int eCode)
{
	try {
		if(eCode == OC_STACK_OK) {
			std::cout << "Resource URI: " << rep.getUri() << std::endl;

			if (rep.hasAttribute("fanstate")) {
				std::string state = rep.getValue<std::string>("fanstate");
				m_fanState = (state == "on" ? true:false);
				std::cout << "\tfanstate: " << m_fanState << std::endl;
			}
		}
		else {
			std::cout << "onGET Response error: " << eCode << std::endl;
		}
	}
	catch(std::exception& e) {
		std::cout << "Exception: " << e.what() << " in onGet" << std::endl;
	}
}

void SensorResource::executeGasRule(int density)
{
	if (m_fan.s_active && m_fan.s_resource) {
		OCRepresentation fanrep;
		PutCallback p (std::bind(&SensorResource::onPut, this, PH::_1, PH::_2, PH::_3));
		if ((density > m_rr->m_density) && (m_fanState == false)) {
			std::cout << "Turning on the fan..."<<std::endl;

			fanrep.setValue("fanstate", std::string("on"));
			m_fan.s_resource->put(fanrep, QueryParamsMap(), p);
		}
		else if ((density <= m_rr->m_density) && (m_fanState == true)) {
			std::cout << "Turning off the fan..."<<std::endl;

			fanrep.setValue("fanstate", std::string("off"));
			m_fan.s_resource->put(fanrep, QueryParamsMap(), p);
		}
	}
}

void SensorResource::onObserve(const HeaderOptions headerOptions, const OCRepresentation& rep,
	const int& eCode, const int& sequenceNumber)
{
	int density = 0;
	int ledColor = 0;
	bool motion = false;
	try
	{
		if(eCode == OC_STACK_OK) {
			if (rep.hasAttribute("density")) {
				m_gas.s_active = true;
				rep.getValue("density", density);
				std::cout << "\tdensity: " << density << std::endl;

				if (m_rr->m_kitchenMonitor) {
					std::cout << "Kitchen Monitor is enabled" << std::endl;
					executeGasRule(density);
				}
			}

			if (rep.hasAttribute("fanstate")) {
				m_fan.s_active = true;
				std::string state = rep.getValue<std::string>("fanstate");
				m_fanState = (state == "on" ? true:false);
				std::cout << "\tfanstate: " << m_fanState << std::endl;
			}

			if (rep.hasAttribute("ledColor")) {
				m_led.s_active = true;
				rep.getValue("ledColor", ledColor);
				std::cout << "\tledColor: " << ledColor << std::endl;
			}
			if (rep.hasAttribute("motion")) {
				m_pri.s_active = true;
				rep.getValue("motion", motion);
				std::cout << "\tmotion: " << motion << std::endl;
				PutCallback p (std::bind(&SensorResource::onPut, this, PH::_1, PH::_2, PH::_3));
				if ((motion == true) && (m_led.s_active == true)) {
					OCRepresentation ledrep;
					ledrep.setValue("ledColor", BLUE);
					m_led.s_resource->put(ledrep, QueryParamsMap(), p);
				}
			}
		}
		else {
			std::cout << "onObserve Response error: " << eCode << std::endl;
		}
	}
	catch(std::exception& e) {
		std::cout << "Exception: " << e.what() << " in onObserve" << std::endl;
	}
}

// Callback to found resources
void SensorResource::foundResource(std::shared_ptr<OCResource> resource)
{
	std::string resourceURI;
	std::string hostAddress;
	std::lock_guard<std::mutex> lock(m_resourceLock);
	try {
		if(resource) {
			std::cout<<"DISCOVERED Resource:"<<std::endl;
			resourceURI = resource->uri();
			std::cout << "\tURI of the resource: " << resourceURI << std::endl;

			hostAddress = resource->host();
			std::cout << "\tHost address of the resource: " << hostAddress << std::endl;

			std::cout << "\tList of resource types: " << std::endl;
			for(auto &resourceTypes : resource->getResourceTypes()) {
				std::cout << "\t\t" << resourceTypes << std::endl;
			}

			std::cout << "\tList of resource interfaces: " << std::endl;
			for(auto &resourceInterfaces : resource->getResourceInterfaces()) {
				std::cout << "\t\t" << resourceInterfaces << std::endl;
			}

			GetCallback g (std::bind(&SensorResource::onGet, this, PH::_1, PH::_2, PH::_3));
			ObserveCallback o (std::bind(&SensorResource::onObserve, this, PH::_1, PH::_2, PH::_3, PH::_4));
			if (resourceURI == "/sensor/gas") {
				std::cout << "\tFound Gas Sensor." << std::endl;
				resource->observe(ObserveType::Observe, QueryParamsMap(), o);

				m_gas.s_resource = resource;
				m_gas.s_active = true;
			}
			else if (resourceURI == "/a/fan") {
				std::cout << "\tFound Fan Device." << std::endl;
				resource->observe(ObserveType::Observe, QueryParamsMap(), o);

				QueryParamsMap params;
				resource->get(params, g);

				m_fan.s_resource = resource;
				m_fan.s_active = true;
			}
			else if (resourceURI == "/intel/chainable_led_edison") {
				std::cout << "\tFound Edison Led Device." << std::endl;
				resource->observe(ObserveType::Observe, QueryParamsMap(), o);

				QueryParamsMap params;
				resource->get(params, g);

				m_led.s_resource = resource;
				m_led.s_active = true;
			}
			else if (resourceURI == "/sensor/pri") {
				std::cout << "\tFound Pri Device." << std::endl;
				resource->observe(ObserveType::Observe, QueryParamsMap(), o);

				m_pri.s_resource = resource;
				m_pri.s_active = true;
			}
			else {
				std::cout << "Resource unknown." << std::endl;
			}
		}
		else {
			std::cout << "Resource is invalid" << std::endl;
		}
	}
	catch(std::exception& e) {
		//log(e.what());
	}
}

void SensorResource::StartMonitor(std::string address)
{
	std::ostringstream resourceURI;
	try {
		resourceURI << OC_MULTICAST_DISCOVERY_URI << "?rt=" << address;
		FindCallback f (std::bind(&SensorResource::foundResource, this, PH::_1));
		OCPlatform::findResource("", resourceURI.str(), OC_ALL, f);
	}
	catch (OC::OCException& e) {
		std::cout << "Exception: " << e.what() << " in StartMonitor" << std::endl;
	}
}

OCRepresentation SensorResource::get()
{
	OCRepresentation get_rep;
	get_rep.setUri(SENSOR_RESOURCE_ENDPOINT);
	SensorMapIter iter;
	for (iter = m_sensorMap.begin(); iter != m_sensorMap.end(); iter++) {
		get_rep.setValue(iter->first, std::string(iter->second));
	}

	return get_rep;
}

void SensorResource::put(OCRepresentation rep)
{
	if (rep.hasAttribute("name")) {
		m_sensorName = rep.getValue<std::string>("name");
	}

	if (rep.hasAttribute("address")) {
		m_sensorAddr = rep.getValue<std::string>("address");
	}

	std::cout << "Registered sensor name : " << m_sensorName << " address : " << m_sensorAddr<< std::endl;
	if ((!m_sensorName.empty()) && (!m_sensorAddr.empty())) {
		if (m_sensorMap.find(m_sensorName) == m_sensorMap.end()) {
			m_sensorMap.insert(SensorMap::value_type(m_sensorName, m_sensorAddr));
		}
	}
	else {
		return;
	}

	if (m_sensorName == "gas") {
		m_gas.s_name = m_sensorName;
	}
	else if (m_sensorName == "fan") {
		m_fan.s_name = m_sensorName;
	}
	else if (m_sensorName == "led") {
		m_led.s_name = m_sensorName;
	}
	else if (m_sensorName == "pri") {
		m_pri.s_name = m_sensorName;
	}

	StartMonitor(m_sensorAddr);
}

// ChangeSensorRepresentaion is an observation function,
// which notifies any changes to the sensors to stack
// via notifyObservers
void SensorResource::ChangeSensorRepresentation()
{
	if (m_observation) {
		std::cout << "Notifying observers with resource handle: " << m_resourceHandle << endl;

		OCStackResult result = OC_STACK_OK;

		std::shared_ptr<OCResourceResponse> resourceResponse(new OCResourceResponse());

		resourceResponse->setErrorCode(200);
		resourceResponse->setResourceRepresentation(get(), DEFAULT_INTERFACE);

		result = OCPlatform::notifyListOfObservers(m_resourceHandle,
					m_interestedObservers,
					resourceResponse);

		if (OC_STACK_NO_OBSERVERS == result) {
			std::cout << "No More observers, stopping notifications" << endl;
			m_observation = 0;
		}
	}
}

OCEntityHandlerResult SensorResource::entityHandler(std::shared_ptr<OCResourceRequest> request)
{
	std::cout << "EH of sensor resource invoked " << std::endl;
	OCEntityHandlerResult ehResult = OC_EH_ERROR;

	if(request) {
		int requestFlag = request->getRequestHandlerFlag();
		std::cout << "In entity handler for sensors, URI is : "
			<< request->getResourceUri() << std::endl;

		if(requestFlag & RequestHandlerFlag::RequestFlag) {
			auto pResponse = std::make_shared<OC::OCResourceResponse>();
			pResponse->setRequestHandle(request->getRequestHandle());
			pResponse->setResourceHandle(request->getResourceHandle());

			if(request->getRequestType() == "GET") {
				// Return all registered sensors
				std::cout<< " Sensors Get Request"<<std::endl;
				pResponse->setErrorCode(200);
				pResponse->setResourceRepresentation(get(), "");
				if(OC_STACK_OK == OCPlatform::sendResponse(pResponse)) {
					ehResult = OC_EH_OK;
				}
			}
			else if(request->getRequestType() == "PUT") {
				// Register new sensor address
				std::cout << " Sensors Put Request"<<std::endl;
				put(request->getResourceRepresentation());
				pResponse->setErrorCode(200);
				pResponse->setResourceRepresentation(get(),"");
				if(OC_STACK_OK == OCPlatform::sendResponse(pResponse)) {
					ehResult = OC_EH_OK;
					if (!m_sensorMap.empty()) {
						ChangeSensorRepresentation();
					}
				}
			}
			else {
				std::cout <<" Sensors unsupported request type "
				<< request->getRequestType() << std::endl;
				pResponse->setResponseResult(OC_EH_ERROR);
				OCPlatform::sendResponse(pResponse);
				ehResult = OC_EH_ERROR;
			}
		}

		if (requestFlag & RequestHandlerFlag::ObserverFlag) {
			ObservationInfo observationInfo = request->getObservationInfo();
			if (ObserveAction::ObserveRegister == observationInfo.action) {
				cout << "Starting observer for registered sensors" << endl;
				m_interestedObservers.push_back(observationInfo.obsId);
				m_observation = 1;
			}
			else if (ObserveAction::ObserveUnregister == observationInfo.action) {
				m_interestedObservers.erase(std::remove(
								m_interestedObservers.begin(),
								m_interestedObservers.end(),
								observationInfo.obsId),
								m_interestedObservers.end());
			}

			ehResult = OC_EH_OK;
		}
	}

	return ehResult;
}
