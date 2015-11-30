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

#ifndef GATEWAY_H_
#define GATEWAY_H_

#include <chrono>
#include <iostream>
#include <thread>
#include <stdexcept>
#include <map>
#include <vector>
#include <stdio.h>
#include <glib.h>
#include "ble_hr_sensor.h"
#include "bluetooth.h"

#include "OCPlatform.h"
#include "OCApi.h"

#include "resource.h"

#define DEFAULT_TIMEOUT 5
#define ACTIVE_INTERVAL 4

#define CONFIG_RESOURCE_ENDPOINT "/gw/con"
#define CONFIG_RESOURCE_TYPE "gw.config"
#define RULES_RESOURCE_ENDPOINT "/gw/rul"
#define RULES_RESOURCE_TYPE "gw.rule"
#define SENSOR_RESOURCE_ENDPOINT "/gw/sensor"
#define SENSOR_RESOURCE_TYPE "gw.sensor"

#define RED 9
#define BLUE 10
#define GREEN 11

typedef std::map<std::string, std::string> SensorMap;
typedef std::map<std::string, std::string>::iterator SensorMapIter;
typedef std::vector<std::string> SensorVector;

using namespace OC;
namespace PH = std::placeholders;

struct sensor_data
{
	std::string s_name;
	std::shared_ptr<OCResource> s_resource;
	bool s_active;
};

#endif /* GATEWAY_H_ */
