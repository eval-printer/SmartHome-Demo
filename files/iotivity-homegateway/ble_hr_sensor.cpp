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
#include <signal.h>
#include <glib.h>
#include <thread>
#include <functional>
#include "sensor_resource.h"
#include "ble_hr_sensor.h"

static SensorResource *sensor = NULL;

BLE_hrSensor::BLE_hrSensor() : m_address("NO CONNECTION"), m_connected(false), m_hr(0)
{
	cout << "Running BLE_hrSensor constructor" << endl;

	m_hrRepresentation.setValue("address", "NO CONNECTION");
	m_hrRepresentation.setValue("heartRate", 0);
}

BLE_hrSensor::~BLE_hrSensor()
{
    cout << "Running BLE_hrSensor destructor" << endl;
}

void BLE_hrSensor::createResource()
{
	uint8_t resourceFlag = OC_DISCOVERABLE | OC_OBSERVABLE;
	std::string resourceURI = BLE_HR_SENSOR_RESOURCE_ENDPOINT; // URI of the resource
	std::string resourceTypeName = BLE_HR_SENSOR_RESOURCE_TYPE; // resource type name.
	std::string resourceInterface = DEFAULT_INTERFACE; // resource interface.

	EntityHandler cb = bind(&BLE_hrSensor::hrEntityHandler, this, placeholders::_1);
	OCStackResult result = OCPlatform::registerResource(m_hrResource, resourceURI, resourceTypeName,
                                                                resourceInterface, cb, resourceFlag);
	if (result != OC_STACK_OK)
		cerr << "Could not create " << m_hrResource << " resource" << endl;
	else {
		cout << "Successfully created " << m_hrResource << " resource" << endl;
		//register resource server on Home Gateway;
		if (sensor != NULL) {
			cout << "register resource " << sensor << endl;
			sensor->registerBle(BLE_HR_SENSOR_RESOURCE_ENDPOINT);
		}
	}
}

void BLE_hrSensor::destroyResource()
{
	cout << "Destroy BLE Heart Rate sensor resource" << endl;

	OCStackResult result = OCPlatform::unregisterResource(m_hrResource);
	if (result != OC_STACK_OK) {
		cerr << "Could not destroy: " << m_hrResource << endl;
		return;
	} else {
		m_hrObservers.clear();
		cout << "Successfully destroy: " << m_hrResource<< endl;
		//ungister resource server on Home Gateway;
		if (sensor != NULL)
			sensor->unregisterBle();
	}
}

OCRepresentation BLE_hrSensor::getRep()
{
        m_hrRepresentation.setUri(BLE_HR_SENSOR_RESOURCE_ENDPOINT);
        m_hrRepresentation.setValue("address", m_address);
        m_hrRepresentation.setValue("heartRate", m_hr);
	return m_hrRepresentation;
}

OCStackResult BLE_hrSensor::notify()
{
	shared_ptr<OCResourceResponse> resourceResponse(
					new OCResourceResponse());

        resourceResponse->setErrorCode(200);

        resourceResponse->setResourceRepresentation(
					getRep(), DEFAULT_INTERFACE);

	return OCPlatform::notifyListOfObservers(m_hrResource,
					m_hrObservers, resourceResponse);
}

static BLE_hrSensor hrSensor;
static bt_gatt_attribute_h att_hr_hdl = NULL;

static void _gatt_characteristic_changed_cb(
					bt_gatt_attribute_h characteristic,
					unsigned char *value,
					int value_length,
					void *user_data)
{
	int i;

	//cout << "Heart Rate Value: " << (int) value[1] << endl;

	hrSensor.m_hr = (int) value[1];

	hrSensor.notify();

	if (sensor != NULL)
		sensor->executeJumpingRule(hrSensor.m_hr);
}

static void start_observe()
{
	cout << "Start observer for BLE Heart Rate sensor" << endl;
	hrSensor.m_hr = 80;

	hrSensor.notify();
	char * kk = (char *)att_hr_hdl;
	cout << "hdl is" << kk << endl;
	bt_gatt_set_characteristic_changed_cb(att_hr_hdl,
				_gatt_characteristic_changed_cb, NULL);
}

static void stop_observe()
{
	cout << "Stop observer for BLE Heart Rate sensor" << endl;
	char * kk = (char *)att_hr_hdl;
	cout << "hdl is" << kk << endl;

	bt_gatt_unset_characteristic_changed_cb(att_hr_hdl);
}

OCEntityHandlerResult BLE_hrSensor::hrEntityHandler(shared_ptr<OCResourceRequest> Request)
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

			m_hrObservers.push_back(observationInfo.obsId);

			if(1 == m_hrObservers.size())
				start_observe();

		}else if (ObserveAction::ObserveUnregister ==
						observationInfo.action) {
			m_hrObservers.erase(
				remove(m_hrObservers.begin(), m_hrObservers.end(),
				observationInfo.obsId),
				m_hrObservers.end());

			cout << "Unregister observer" << observationInfo.obsId << endl;

			if(0 == m_hrObservers.size())
				stop_observe();
		}
	}

	return result;
}

#define HEART_RATE_UUID "0000180d-0000-1000-8000-00805f9b34fb"

bool bt_gatt_pri_src_cb(bt_gatt_attribute_h service, void *user_data)
{
	bt_device_connection_info_s *conn_info =
		(bt_device_connection_info_s *)user_data;
	char *uuid;

	bt_gatt_get_service_uuid(service, &uuid);

	cout << "UUID: " << uuid << endl;

	if (!g_strcmp0(uuid, HEART_RATE_UUID)) {
		att_hr_hdl = service;
		hrSensor.createResource();
		hrSensor.m_address = conn_info->remote_address;
		hrSensor.m_connected = true;
		g_free(uuid);
		return false;
	}

	g_free(uuid);

	return true;
}

void get_hr_att_hdl(bt_device_connection_info_s *conn_info)
{
	att_hr_hdl = NULL;

	cout<<"get_hr_att_hdl"<<endl;

//	if (conn_info->link != BT_DEVICE_CONNECTION_LINK_LE)
//		return;

	bt_gatt_foreach_primary_services(conn_info->remote_address,
					bt_gatt_pri_src_cb, conn_info);

	return;
}


void bt_conn_state_changed_cb(bool connected,
				bt_device_connection_info_s *conn_info,
							void *user_data)
{
	if(connected){
		cout << "Device " << conn_info->remote_address << "connected" << endl;
		sensor = (SensorResource *) user_data;
		cout << "gateway sensor handle " << sensor << endl;

		if (hrSensor.m_connected) {
			cout << "Device " << hrSensor.m_address << "alread connected" << endl;
			return;
		}

		get_hr_att_hdl(conn_info);
        	
	} else {
		cout << "Device " << conn_info->remote_address << "disconnected"<< endl;

		if (hrSensor.m_address.compare(conn_info->remote_address))
			return;

		
        	hrSensor.destroyResource();
		hrSensor.m_address = "NO CONNECTION";
		hrSensor.m_connected = false;
		stop_observe();

		sensor = NULL;
	}
}
