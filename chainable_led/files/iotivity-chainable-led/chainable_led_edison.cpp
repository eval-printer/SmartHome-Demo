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
#include "chainable_led_edison.h"
#include <unistd.h>

ledEdsn::ledEdsn() : presenceTimer(0), m_setting(0), inPrecence(false), m_ledResource(nullptr)
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
	if (m_ledResource != nullptr)
		return false;

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
	cout << "Destroy Edison chainable LED resource" << endl;
	if (m_ledResource) {
		OCStackResult result = OCPlatform::unregisterResource(m_ledResource);
		if (result != OC_STACK_OK) {
			cerr << "Could not destroy: " << m_ledResource << endl;
			return;
		} else {
			m_ledResource = nullptr;
			cout << "Successfully destroy: " << m_ledResource<< endl;
		}
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

	try {
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
			cout << "resourceType: " << resourceTypes << endl;
			if( resourceTypes == HG_DISCOVER_RESOUCE_TYPE) {
				OCRepresentation rep;
				rep.setValue("name", string("led"));
				rep.setValue("address", string(LED_EDSN_RESOURCE_TYPE));
				resource->put(rep, QueryParamsMap(), &onRegister);
				return;
			}
		}
	}
	catch (OC::OCException& e) {
		cout << "Exception in foundDiscResource: " << e.what() << endl;
	}
}

void ledEdsn::registeration()
{
    try {
	OCPlatform::findResource("",
		"/oc/core?rt=gw.sensor", OC_ALL, &foundDiscResource);
    }
    catch (OCException& e){
	cout << "Exception in findResource: " << e.what();
    }
}

static void onGet(const HeaderOptions& headerOptions,
			const OCRepresentation& rep, const int eCode)
{
	int setting;

	if(eCode != OC_STACK_OK) {
		cerr << "Can't get configuration from Home Gateway" << endl;
		return;
	}

	try {
		rep.getValue("ledColor", setting);

		cout << "GET LED configuration successfully" << endl;
		cout << "Setting led: " << setting << endl;

		my_led.set_led_setting(setting);
	}
	catch (OC::OCException& e) {
		cout << "Exception getting ledColor value in onGet: " << e.what() << endl;
		return;
	}

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
		cout << "resourceType: " << resourceTypes << endl;
		if (resourceTypes == HG_CONFIGURATION_RESOUCE_TYPE) {
			QueryParamsMap params;
			resource->get(params, &onGet);
			return;
		}
	}

	return;
}

void ledEdsn::configuration()
{
	cout << "Start configure Edison LED" << endl;

	try {
		OCPlatform::findResource("",
			"/oc/core?rt=gw.config", OC_ALL, &foundConfResource);

	}
	catch (OCException& e) {
		cout << "Exception in findResource of configuration: " << e.what();
	}
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

void clk(){
	mraa_gpio_write(my_led.clockPin, 0);
	usleep(_CLK_PULSE_DELAY);
	mraa_gpio_write(my_led.clockPin, 1);
	usleep(_CLK_PULSE_DELAY);
}

void sendByte(uint8_t b){
	// send one bit at a time
	for (int i=0; i<8; i++){
		if ((b & 0x80) != 0)
			mraa_gpio_write(my_led.dataPin, 1);
		else
			mraa_gpio_write(my_led.dataPin, 0);
		clk();
		b <<= 1;
	}
}

void sendColor(uint8_t red, uint8_t green, uint8_t blue){
	// start by sending a byte with the format "1 1 /B7 /B6 /G7 /G6 /R7 /R6"
	uint8_t prefix = 0b11000000;
	if ((blue & 0x80) == 0) prefix |= 0b00100000;
	if ((blue & 0x40) == 0) prefix |= 0b00010000;
	if ((green & 0x80) == 0) prefix |= 0b00001000;
	if ((green & 0x40) == 0) prefix |= 0b00000100;
	if ((red & 0x80) == 0) prefix |= 0b00000010;
	if ((red & 0x40) == 0) prefix |= 0b00000001;
	sendByte(prefix);

	sendByte(blue);
	sendByte(green);
	sendByte(red);
}

void sendColorRGB(uint8_t led, uint8_t red, uint8_t green, uint8_t blue){
	// send prefix 32 x "0"
	sendByte(0x00);
	sendByte(0x00);
	sendByte(0x00);
	sendByte(0x00);
	
	for (uint8_t i = 0; i < _NUM_LED; i++){
		if (i == led){
			my_led.ledState[i*3 + _CL_RED] = red;
			my_led.ledState[i*3 + _CL_GREEN] = green;
			my_led.ledState[i*3 + _CL_BLUE] = blue;
		}
		sendColor(my_led.ledState[i*3 + _CL_RED],
				my_led.ledState[i*3 + _CL_GREEN],
				my_led.ledState[i*3 + _CL_BLUE]);
	}

	// terminate data frame
	sendByte(0x00);
	sendByte(0x00);
	sendByte(0x00);
	sendByte(0x00);
}

void turnon_led(int color)
{
	switch (color){
		case RED:
			for (uint8_t i=0; i<_NUM_LED; i++){
				sendColorRGB(i, 255, 0, 0);
			}
			break;
		case BLUE:
			for (uint8_t i=0; i<_NUM_LED; i++){
				sendColorRGB(i, 0, 0, 255);
			}
			break;
		case GREEN:
			for (uint8_t i=0; i<_NUM_LED; i++){
				sendColorRGB(i, 0, 255, 0);
			}
			break;
		default:
			cout << "invalue command" << endl;
	}
}

int initChainableLED(){
	my_led.clockPin = mraa_gpio_init(_CLK_PIN);
	if (my_led.clockPin != NULL){
		mraa_gpio_dir(my_led.clockPin, MRAA_GPIO_OUT);
	}
	else {
		cerr << "Could not initialize clock pin" << endl;
		return 1;
	}
	my_led.dataPin = mraa_gpio_init(_DATA_PIN);
	if (my_led.dataPin != NULL){
		mraa_gpio_dir(my_led.dataPin, MRAA_GPIO_OUT);
	}
	else {
		cerr << "Could not initialize data pin" << endl;
		return 1;
	}
	my_led.ledState = (uint8_t*) calloc(_NUM_LED * 3, sizeof(uint8_t));
	if (my_led.ledState == NULL){
		cerr << "Failed to initialize LED data" << endl;
		return 1;
	}
	
	// initialize the LEDs
	for (uint8_t i=0; i<_NUM_LED; i++){
		sendColorRGB(i, 0, 0, 0);
	}
	return 0;
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

	if (initChainableLED() != 0) {
		cerr << "Failed initializing the LED" << endl;
	}
	my_led.configuration();

	loop = g_main_loop_new(NULL, FALSE);

	g_main_loop_run(loop);

	return 0;
}
