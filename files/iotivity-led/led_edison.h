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
#include "mraa.h"

using namespace std;
using namespace OC;

#define LED_EDSN_RESOURCE_ENDPOINT "/led_edison"
#define LED_EDSN_RESOURCE_TYPE "com.intel"
#define HG_DISCOVER_RESOUCE_TYPE "gw.sensor"
#define HG_CONFIGURATION_RESOUCE_TYPE "gw.config"
#define PRESENCE_CYCLE 2

#define RED 9
#define BLUE 10
#define GREEN 11

void turnon_led(int color);
class ledEdsn
{
    guint presenceTimer;
    shared_ptr<PlatformConfig> m_platformConfig;
    OCRepresentation m_ledRepresentation;
    OCResourceHandle m_ledResource;
    ObservationIds m_ledObservers;

    OCResourceHandle m_hgConfResource;
    OCResourceHandle m_hgDiscResource;

    OCRepresentation getRep();

    OCEntityHandlerResult ledEntityHandler(shared_ptr<OCResourceRequest>);
public:
    mraa_gpio_context red, blue, green;
    int m_setting;
    bool inPrecence;
    ledEdsn();
    bool createResource();
    void registeration();
    void destroyResource();
    void configuration();
    OCStackResult notify();
    bool set_led_setting(int setting);
    int get_led_setting();
    void startPrecence();
    void stopPrecence();
    virtual ~ledEdsn();
};

#endif /* SERVER_H_ */
