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
#include "led_edison.h"
#include "ocsocket.h"

ledEdsn::ledEdsn() : presenceTimer(0), m_setting(0),  inPrecence(false)
{
	cout << "Running ledEdsn constructor" << endl;
	m_platformConfig = make_shared<PlatformConfig>(ServiceType::InProc,
						ModeType::Both, "0.0.0.0",
						5683, OC::QualityOfService::HighQos);
	OCPlatform::Configure(*m_platformConfig);

	m_ledRepresentation.setValue("ledColor", 0);
}

ledEdsn::~ledEdsn()
{
	cout << "Running ledEDSN destructor" << endl;
}

bool ledEdsn::createResource()
{

	uint8_t resourceFlag = OC_DISCOVERABLE | OC_OBSERVABLE;
	std::string resourceURI = LED_EDSN_RESOURCE_ENDPOINT; // URI of the resource
	std::string resourceTypeName = LED_EDSN_RESOURCE_TYPE; // resource type name.
	std::string resourceInterface = DEFAULT_INTERFACE; // resource interface.

	EntityHandler cb = bind(&ledEdsn::ledEntityHandler, this, placeholders::_1);
	OCStackResult result = OCPlatform::registerResource(m_ledResource, resourceURI, resourceTypeName,
								resourceInterface, cb, resourceFlag);
	if (result != OC_STACK_OK) {
		cerr << "Could not create " << m_ledResource << " resource" << endl;
		return false;
	} else {
		cout << "Successfully created " << m_ledResource << " resource" << endl;
		return true;
	}
}

void ledEdsn::destroyResource()
{
	cout << "Destroy BLE Heart Rate sensor resource" << endl;

	OCStackResult result = OCPlatform::unregisterResource(m_ledResource);
	if (result != OC_STACK_OK) {
		cerr << "Could not destroy: " << m_ledResource << endl;
		return;
	} else {
		cout << "Successfully destroy: " << m_ledResource<< endl;
	}
}

static void onRegister(const HeaderOptions& headerOptions,
		const OCRepresentation& rep, const int eCode)
{
	if(eCode == OC_STACK_OK || eCode == OC_STACK_RESOURCE_CREATED)
		cout << "Register Home Gateway discovery resource successfully" << endl;

	return;
}

static ledEdsn my_led;

static void foundDiscResource(shared_ptr<OCResource> resource)
{
	string resourceURI;
	string hostAddress;

	if (!resource) {
		cout << "Resource is invalid" << endl;
		return;
	}

	cout<<"DISCOVERED Resource:"<<endl;

	resourceURI = resource->uri();
	cout << "\tURI of the resource: " << resourceURI << endl;

	hostAddress = resource->host();
	cout << "\tHost address of the resource: " << hostAddress << endl;

	for(auto &resourceTypes : resource->getResourceTypes()) {
		cout << "resourceType" << resourceTypes << endl;
		if( resourceTypes == HG_DISCOVER_RESOUCE_TYPE) {

			OCRepresentation rep;
			uint8_t ifname[] = "eth0";
			uint8_t ipAddr[20];
			char address[64] = {0};

			OCGetInterfaceAddress(ifname, sizeof(ifname), AF_INET, ipAddr, sizeof(ipAddr));
			snprintf(address, sizeof(address), "coap://%s:5683/oc/core?rt=com.intel", (char*)ipAddr);

			rep.setValue("name", string("led"));
			rep.setValue("address", string(address));
			resource->put(rep, QueryParamsMap(), &onRegister);
			return;
		}
	}

	return;
}

void ledEdsn::registeration()
{
	OCPlatform::findResource("",
		"coap://224.0.1.187/oc/core?rt=gw.sensor", &foundDiscResource);
}

static void onGet(const HeaderOptions& headerOptions,
			const OCRepresentation& rep, const int eCode)
{
	int setting;

	if(eCode != OC_STACK_OK) {
		cerr << "Can't get configuration from Home Gateway" << endl;
		return;
	}

	rep.getValue("ledColor", setting);

	cout << "GET Fan configuration successfully" << endl;
	cout << "Setting led: " << setting << endl;

	my_led.set_led_setting(setting);

	if (my_led.createResource())
		my_led.registeration();
}

static void foundConfResource(shared_ptr<OCResource> resource)
{
	string resourceURI;
	string hostAddress;

	if (!resource) {
		cout << "Resource is invalid" << endl;
		return;
	}

	cout<<"DISCOVERED Resource:"<<endl;

	resourceURI = resource->uri();
	cout << "\tURI of the resource: " << resourceURI << endl;

	hostAddress = resource->host();
	cout << "\tHost address of the resource: " << hostAddress << endl;

	for(auto &resourceTypes : resource->getResourceTypes()) {
		cout << "resourceType" << resourceTypes << endl;
		if (resourceTypes == HG_CONFIGURATION_RESOUCE_TYPE) {
			QueryParamsMap test;
			resource->get(test, &onGet);
			return;
		}
	}

	return;
}

void ledEdsn::configuration()
{
	cout << "Start configure Edison Fan" << endl;

	OCPlatform::findResource("",
		"coap://224.0.1.187/oc/core?rt=gw.config", &foundConfResource);
}

OCRepresentation ledEdsn::getRep()
{ 
	m_ledRepresentation.setUri(LED_EDSN_RESOURCE_ENDPOINT);
	m_ledRepresentation.setValue("ledColor", m_setting);
	return m_ledRepresentation;
}

OCStackResult ledEdsn::notify()
{
	shared_ptr<OCResourceResponse> resourceResponse(
					new OCResourceResponse());

		resourceResponse->setErrorCode(200);

		resourceResponse->setResourceRepresentation(
					getRep(), DEFAULT_INTERFACE);

	return OCPlatform::notifyListOfObservers(m_ledResource,
					m_ledObservers, resourceResponse);
}

bool ledEdsn::set_led_setting(int setting)
{
	cout << "Set led Setting " << setting << endl;

	if (setting == m_setting)
		return true;

	m_setting = setting;
	turnon_led(setting);

	return true;
}


static gboolean do_presence(gpointer user_data)
{
	ledEdsn *_my_led = (ledEdsn *)user_data;

	if (!_my_led->inPrecence)
		return false;

	_my_led->notify();

	return true;
}

void ledEdsn::startPrecence()
{
	if (inPrecence)
		return;

	inPrecence = true;
	presenceTimer = g_timeout_add_seconds(PRESENCE_CYCLE, do_presence, this);
}

void ledEdsn::stopPrecence()
{
	inPrecence = false;
	presenceTimer = 0;
}

OCEntityHandlerResult ledEdsn::ledEntityHandler(shared_ptr<OCResourceRequest> Request)
{
	OCEntityHandlerResult result = OC_EH_ERROR;

	if (!Request)
		return result;

	string requestType = Request->getRequestType();
	cout << "requestType " << requestType << endl;

	int requestFlag = Request->getRequestHandlerFlag();
	cout << "requestFlag " << requestFlag << endl;

	if (requestFlag & RequestHandlerFlag::RequestFlag) {

		auto Response = std::make_shared<OC::OCResourceResponse>();

		Response->setRequestHandle(Request->getRequestHandle());
		Response->setResourceHandle(Request->getResourceHandle());

		if (requestType == "GET") {

			Response->setErrorCode(200);
			Response->setResponseResult(OC_EH_OK);
			Response->setResourceRepresentation(
					getRep());

			if (OCPlatform::sendResponse(Response) == OC_STACK_OK) {
				result = OC_EH_OK;
				cout << "SendResponse Successfully" << endl;
			}
			else
				cout << "SendResponse error" << endl;

		} else if(requestType == "PUT") {
			cout << "requestType : PUT" << endl;
			int setting;

			OCRepresentation rep = Request->getResourceRepresentation();

			rep.getValue("ledColor", setting);

			set_led_setting(setting);

			Response->setErrorCode(200);
			Response->setResponseResult(OC_EH_OK);
			Response->setResourceRepresentation(getRep());
			if(OC_STACK_OK == OCPlatform::sendResponse(Response))
				result = OC_EH_OK;
		} else {
			Response->setResponseResult(OC_EH_ERROR);

			OCPlatform::sendResponse(Response);

			cerr << "Unsupported request type" << endl;

			return result;
		}
	}

	if (requestFlag & RequestHandlerFlag::ObserverFlag) {

		ObservationInfo observationInfo = Request->getObservationInfo();

		if (ObserveAction::ObserveRegister == observationInfo.action) {

			cout << "Register observer" << observationInfo.obsId << endl;

			m_ledObservers.push_back(observationInfo.obsId);

			if(1 == m_ledObservers.size())
				my_led.startPrecence();

		}else if (ObserveAction::ObserveUnregister ==
						observationInfo.action) {
			m_ledObservers.erase(
				remove(m_ledObservers.begin(), m_ledObservers.end(),
				observationInfo.obsId),
				m_ledObservers.end());

			cout << "Unregister observer" << observationInfo.obsId << endl;

			if(0 == m_ledObservers.size())
				my_led.stopPrecence();
		}
	}

	return result;
}
void setupPins()
{
	my_led.red = mraa_gpio_init(RED);
	if (my_led.red != NULL) {
		mraa_gpio_dir(my_led.red, MRAA_GPIO_OUT);
		mraa_gpio_write(my_led.red, 1);
	}

	my_led.blue = mraa_gpio_init(BLUE);
	if (my_led.blue != NULL) {
		mraa_gpio_dir(my_led.blue, MRAA_GPIO_OUT);
		mraa_gpio_write(my_led.blue, 1);
	}

	my_led.green = mraa_gpio_init(GREEN);
	if (my_led.green != NULL) {
		mraa_gpio_dir(my_led.green, MRAA_GPIO_OUT);
		mraa_gpio_write(my_led.green, 1);
	}
}

void turnon_led(int color)
{
	switch (color){
		case RED:
			mraa_gpio_write(my_led.red, 0);
			mraa_gpio_write(my_led.blue, 1);
			mraa_gpio_write(my_led.green, 1);
			break;
		case BLUE:
			mraa_gpio_write(my_led.red, 1);
			mraa_gpio_write(my_led.blue, 0);
			mraa_gpio_write(my_led.green, 1);
			break;
		case GREEN:
			mraa_gpio_write(my_led.red, 1);
			mraa_gpio_write(my_led.blue, 1);
			mraa_gpio_write(my_led.green, 0);
			break;
		default:
			cout << "invalue command" << endl;
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
	cout << "Press Ctrl-C to quit...." << endl;

	setupPins();
	my_led.configuration();

	loop = g_main_loop_new(NULL, FALSE);

	g_main_loop_run(loop);

	return 0;
}
