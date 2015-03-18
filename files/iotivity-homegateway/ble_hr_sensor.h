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

#ifndef SERVER_H_
#define SERVER_H_

#include <string>
#include <iostream>
#include <memory>
#include "ocstack.h"
#include "OCPlatform.h"
#include "OCApi.h"

#include "bluetooth.h"
using namespace std;
using namespace OC;

#define BLE_HR_SENSOR_RESOURCE_ENDPOINT "/sensor/heartrate"
#define BLE_HR_SENSOR_RESOURCE_TYPE "com.intel"

void bt_conn_state_changed_cb(bool connected,
				bt_device_connection_info_s *conn_info,
						void *user_data);
class BLE_hrSensor
{
    shared_ptr<PlatformConfig> m_platformConfig;
    OCRepresentation m_hrRepresentation;
    OCResourceHandle m_hrResource;
    ObservationIds m_hrObservers;

    OCRepresentation getRep();

    OCEntityHandlerResult hrEntityHandler(shared_ptr<OCResourceRequest>);
public:
    string m_address;
    bool m_connected;
    int m_hr;
    BLE_hrSensor();
    void createResource();
    void destroyResource();
    OCStackResult notify();
    virtual ~BLE_hrSensor();
};

#endif /* SERVER_H_ */
