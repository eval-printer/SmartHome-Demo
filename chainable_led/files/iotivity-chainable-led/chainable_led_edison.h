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

#ifndef CHAINABLE_LED_EDISON_H_
#define CHAINABLE_LED_EDISON_H_

#include <string>
#include <iostream>
#include <memory>
#include "ocstack.h"
#include "OCPlatform.h"
#include "OCApi.h"
#include "mraa.h"

using namespace std;
using namespace OC;

#define LED_EDSN_RESOURCE_ENDPOINT "/intel/chainable_led_edison"
#define LED_EDSN_RESOURCE_TYPE "intel.chainable_led_edison"
#define HG_DISCOVER_RESOUCE_TYPE "gw.sensor"
#define HG_CONFIGURATION_RESOUCE_TYPE "gw.config"
#define PRESENCE_CYCLE 2

#define RED 9
#define BLUE 10
#define GREEN 11
#define _CL_RED             0
#define _CL_GREEN           1
#define _CL_BLUE            2
#define _CLK_PULSE_DELAY    20
#define _CLK_PIN	    7
#define _DATA_PIN	    8
#define _NUM_LED	    4

void turnon_led(int color);
void clk(void);
void sendByte(uint8_t byte);
void sendColor(uint8_t red, uint8_t green, uint8_t blue);
void sendColorRGB(uint8_t led, uint8_t red, uint8_t green, uint8_t blue);
int initChainableLED(void);

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

    mraa_gpio_context clockPin, dataPin;
    uint8_t* ledState;
};

#endif /* CHAINABLE_LED_EDISON_H_ */
