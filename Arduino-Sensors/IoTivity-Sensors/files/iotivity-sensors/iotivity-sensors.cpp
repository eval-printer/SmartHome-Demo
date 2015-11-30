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
#include <signal.h>
#include <glib.h>
#include <thread>
#include <functional>
#include "iotivity-sensors.h"
#include <unistd.h>

Resource::Resource() : inPresence(false), presenceTimer(0)
{

}

static gboolean do_presence(gpointer user_data)
{
	Resource *_my_resource = (Resource *) user_data;
	if (_my_resource != NULL && _my_resource->inPresence)
	{
		_my_resource->notify();
		return true;
	}
	return false;
}

void Resource::startPresence(unsigned int interval)
{
	if (inPresence)
		return;
	
	inPresence = true;
	presenceTimer = g_timeout_add_seconds(interval, do_presence, this);
}

void Resource::stopPresence(void)
{
	inPresence = false;
	presenceTimer = 0;
}

OCEntityHandlerResult Resource::entityHandler(std::shared_ptr<OCResourceRequest> request)
{
	OCEntityHandlerResult ehResult = OC_EH_ERROR;
	cout << "\tIn Resource entity handler:\n";

	if (request) {
		std::string requestType = request->getRequestType();
		int requestFlag = request->getRequestHandlerFlag();

		if (requestFlag & RequestHandlerFlag::RequestFlag) {
			cout << "\t\trequestFlag : Request\n";
			
			if (requestType == "GET") {
				cout << "\t\t\trequestType : GET\n";
			}
			else if (requestType == "PUT") {
				cout << "\t\t\trequestType : PUT\nh";
				OCRepresentation rep = request->getResourceRepresentation();
				put(rep);
			}
		}

		if (requestFlag & RequestHandlerFlag::ObserverFlag) {
			cout << "\t\trequestFlag : Observer\n";
			ObservationInfo observationInfo = request->getObservationInfo();
			if (ObserveAction::ObserveRegister == observationInfo.action) {
				cout << "\t\t\trequestType : Register Observer; ID = " << observationInfo.obsId << endl;
				if (std::find(m_interestedObservers.begin(),
						m_interestedObservers.end(),
						observationInfo.obsId) == m_interestedObservers.end())
					m_interestedObservers.push_back(observationInfo.obsId);
				if (1 == m_interestedObservers.size())
					startPresence(PRESENCE_CYCLE);
			}
			else if (ObserveAction::ObserveUnregister == observationInfo.action) {
				cout << "\t\t\trequestType : UNregister Observer; ID = " << observationInfo.obsId << endl;
				m_interestedObservers.erase(std::remove(
									m_interestedObservers.begin(),
									m_interestedObservers.end(),
									observationInfo.obsId),
									m_interestedObservers.end());
				if (0 == m_interestedObservers.size())
					stopPresence();
			}
		}
		
		auto pResponse = std::make_shared<OC::OCResourceResponse>();
		if (pResponse) {
			pResponse->setRequestHandle(request->getRequestHandle());
			pResponse->setResourceHandle(request->getResourceHandle());
			pResponse->setErrorCode(200);
			pResponse->setResponseResult(OC_EH_OK);
			pResponse->setResourceRepresentation(get());
			if (OC_STACK_OK == OCPlatform::sendResponse(pResponse)) {
				ehResult = OC_EH_OK;
				cout << "\t\t\tsendResponse successfully\n";
			}
		}
	}

	return ehResult;
}

OCStackResult Resource::notify()
{
	shared_ptr<OCResourceResponse> resourceResponse(
					new OCResourceResponse());
	resourceResponse->setErrorCode(200);
	resourceResponse->setResourceRepresentation(get(), DEFAULT_INTERFACE);

	return OCPlatform::notifyListOfObservers(m_resourceHandle,
						m_interestedObservers,
						resourceResponse,
						QualityOfService::LowQos);
}

FanResource::FanResource() : 
	m_fanState(false)
{
	m_rep.setUri(string(FAN_RESOURCE_URI));
	m_rep.setValue("name", string("fan"));
	m_rep.setValue("fanstate", "off");
}

FanResource::~FanResource()
{
	if (m_pin != NULL){
		mraa_gpio_write(m_pin, 0);
		mraa_gpio_close(m_pin);
	}
}

void FanResource::createResource()
{
	uint8_t resourceFlag = OC_DISCOVERABLE | OC_OBSERVABLE;
	std::string resourceUri = FAN_RESOURCE_URI;
	std::string resourceType = FAN_RESOURCE_TYPE;
	std::string resourceInterface = DEFAULT_INTERFACE;

	EntityHandler cb = bind(&FanResource::entityHandler, this, placeholders::_1);
	OCStackResult result = OCPlatform::registerResource(m_resourceHandle,
								resourceUri,
								resourceType,
								resourceInterface,
								cb,
								resourceFlag);

	if (result != OC_STACK_OK) {
		throw std::runtime_error(
			std::string("Failed to register Fan Resource")+std::to_string(result));
	} else {
		cout << "Successfully created Fan resource " << m_resourceHandle << endl;
	}
}

OCRepresentation FanResource::get(void)
{
	m_rep.setValue("fanstate", m_fanState ? string("on") : string("off"));
	return m_rep;
}

void FanResource::put(OCRepresentation& rep)
{
	string fanState;
	
	try {
		rep.getValue("fanstate", fanState);
		m_fanState = fanState == "on" ? true : false;
	}
	catch (OC::OCException& e) {
		cout << "Exception in put: " << e.what() << endl;
	}

	if (m_pin != NULL) {
		if (m_fanState)
			mraa_gpio_write(m_pin, 1);
		else
			mraa_gpio_write(m_pin, 0);
	}
}

bool FanResource::setup_hardware(void)
{
	m_pin = mraa_gpio_init(FANPIN);
	if (m_pin != NULL) {
		mraa_gpio_dir(m_pin, MRAA_GPIO_OUT);
		mraa_gpio_write(m_pin, 0);
		return true;
	}
	return false;
}

GasResource::GasResource() : 
	m_density(0)
{
	m_rep.setUri(GAS_RESOURCE_URI);
	m_rep.setValue("name", string("gas"));
	m_rep.setValue("density", (int)m_density);
}

GasResource::~GasResource()
{
	if (m_pin != NULL)
		mraa_aio_close(m_pin);
}

void GasResource::createResource()
{
	uint8_t resourceFlag = OC_DISCOVERABLE | OC_OBSERVABLE;
	std::string resourceUri = GAS_RESOURCE_URI;
	std::string resourceType = GAS_RESOURCE_TYPE;
	std::string resourceInterface = DEFAULT_INTERFACE;

	EntityHandler cb = bind(&GasResource::entityHandler, this, placeholders::_1);
	OCStackResult result = OCPlatform::registerResource(m_resourceHandle,
								resourceUri,
								resourceType,
								resourceInterface,
								cb,
								resourceFlag);

	if (result != OC_STACK_OK) {
		throw std::runtime_error(
			std::string("Failed to register Gas Resource")+std::to_string(result));
	} else {
		cout << "Successfully created Gas resource " << m_resourceHandle << endl;
	}
}

OCRepresentation GasResource::get(void)
{
	if (m_pin != NULL)
		m_density = mraa_aio_read(m_pin) * 500 / 1024;
	m_rep.setValue("density", m_density);
	return m_rep;
}

bool GasResource::setup_hardware(void)
{
	m_pin = mraa_aio_init(GASPIN);
	if (m_pin != NULL) {
		mraa_aio_set_bit(m_pin, 10);
		return true;
	}
	return false;
}

PirResource::PirResource() : m_motion(false)
{
	m_rep.setUri(PIR_RESOURCE_URI);
	m_rep.setValue("name", string("pri"));
	m_rep.setValue("motion", m_motion);
}

PirResource::~PirResource()
{
	if (m_pin != NULL)
		mraa_gpio_close(m_pin);
}

void PirResource::createResource()
{
	uint8_t resourceFlag = OC_DISCOVERABLE | OC_OBSERVABLE;
	std::string resourceUri = PIR_RESOURCE_URI;
	std::string resourceType = PIR_RESOURCE_TYPE;
	std::string resourceInterface = DEFAULT_INTERFACE;

	EntityHandler cb = bind(&PirResource::entityHandler, this, placeholders::_1);
	OCStackResult result = OCPlatform::registerResource(m_resourceHandle,
								resourceUri,
								resourceType,
								resourceInterface,
								cb,
								resourceFlag);

	if (result != OC_STACK_OK) {
		throw std::runtime_error(
			std::string("Failed to register Motion Resource")+std::to_string(result));
	} else {
		cout << "Successfully created Motion resource " << m_resourceHandle << endl;
	}
}

OCRepresentation PirResource::get(void)
{
	if (m_pin != NULL) {
		if (mraa_gpio_read(m_pin) > 0)
			m_motion = true;
		else
			m_motion = false;
	}
	m_rep.setValue("motion", m_motion);
	return m_rep;
}

bool PirResource::setup_hardware(void)
{
	m_pin = mraa_gpio_init(PIRPIN);
	if (m_pin != NULL) {
		mraa_gpio_dir(m_pin, MRAA_GPIO_IN);
		return true;
	}
	return true;
}

static void onFanRegister(const HeaderOptions& headerOptions,
			const OCRepresentation& rep, const int eCode)
{
	if (eCode == OC_STACK_OK) {
		cout << "Successfully regitered Fan resource.\n";
	}
}

static void onGasRegister(const HeaderOptions& headerOptions,
			const OCRepresentation& rep, const int eCode)
{
	if (eCode == OC_STACK_OK) {
		cout << "Successfully regitered Gas resource.\n";
	}
}

static void onPirRegister(const HeaderOptions& headerOptions,
			const OCRepresentation& rep, const int eCode)
{
	if (eCode == OC_STACK_OK) {
		cout << "Successfully regitered Motion resource.\n";
	}
}

static void foundHG(std::shared_ptr<OCResource> resource)
{
	string resourceURI;
	string hostAddress;
	try {
		if (resource) {
			cout << "Discovered resource: \n";
			resourceURI = resource->uri();
			cout << "\tURI: " << resourceURI << endl;

			hostAddress = resource->host();
			cout << "\tAddress: " << hostAddress << endl;

			for (auto &resourceTypes : resource->getResourceTypes()) {
				cout << "\tType: " << resourceTypes << endl;
				if (resourceTypes == HG_DISCOVER_RESOURCE_TYPE) {
					OCRepresentation fanRep, gasRep, pirRep;
					fanRep.setValue("name", string("fan"));
					fanRep.setValue("address", string(FAN_RESOURCE_TYPE));
					resource->put(fanRep, QueryParamsMap(), &onFanRegister);
					
					gasRep.setValue("name", string("gas"));
					gasRep.setValue("address", string(GAS_RESOURCE_TYPE));
					resource->put(gasRep, QueryParamsMap(), &onGasRegister);
					
					pirRep.setValue("name", string("pri"));
					pirRep.setValue("address", string(PIR_RESOURCE_TYPE));
					resource->put(pirRep, QueryParamsMap(), &onPirRegister);
				}
			}
		}
	}
	catch (OC::OCException& e) {
		cout << "Exception in foundHG: " << e.what() << endl;
	}
}

GMainLoop *loop;

void handle_signal(int signal)
{
	g_main_loop_quit(loop);
}

int main()
{
	struct sigaction sa;
	sigfillset(&sa.sa_mask);
	sa.sa_flags = 0;
	sa.sa_handler = handle_signal;
	sigaction(SIGINT, &sa, NULL);
	cout << "Press Ctrl-C to quit....\n";

	PlatformConfig cfg
	{
		ServiceType::InProc,
		ModeType::Both,
		"0.0.0.0",
		5683,
		QualityOfService::LowQos
	};
	OCPlatform::Configure(cfg);
	
	FanResource fan;
	if (!fan.setup_hardware())
		cerr << "Failed to setup fan pin." << endl;
	fan.createResource();

	GasResource gas;
	if (!gas.setup_hardware())
		cerr << "Failed to setup gas pin." << endl;
	gas.createResource();
	
	PirResource pir;
	if (!pir.setup_hardware())
		cerr << "Failed to setup motion pin." << endl;
	pir.createResource();
	
	try {
		std::ostringstream hgURI;
		hgURI << OC_MULTICAST_DISCOVERY_URI << "?rt=" << HG_DISCOVER_RESOURCE_TYPE;
		OCPlatform::findResource("", hgURI.str(), OC_ALL, &foundHG);
		cout << "Finding HomeGateway.....\n";
	}
	catch (OC::OCException& e) {
		cout << "Exception in main: " << e.what() << endl;
	}
	
	loop = g_main_loop_new(NULL, FALSE);
	g_main_loop_run(loop);

	return 0;
}
