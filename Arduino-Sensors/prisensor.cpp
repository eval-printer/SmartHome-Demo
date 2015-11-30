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

// Do not remove the include below
#include "Arduino.h"

#include "logger.h"
#include "ocstack.h"
#include "cJSON.h"
#include <string.h>

#include <SPI.h>
#ifdef ARDUINOWIFI
// Arduino WiFi Shield
#include <WiFi.h>
#include <WiFiUdp.h>
#else
// Arduino Ethernet Shield
#include <EthernetServerV2_0.h>
#include <EthernetV2_0.h>
#include <DnsV2_0.h>
#include <EthernetClientV2_0.h>
#include <utilV2_0.h>
#include <EthernetUdpV2_0.h>
#include <DhcpV2_0.h>
#endif

#define MAX_NUM_OBSERVATIONS 3
#define DEFAULT_CONTEXT_VALUE 0x99

PROGMEM const char TAG[] = "PriServer";
#define PIR_PIN 2//Use pin 2 to receive the signal from the module 
#define PIR_INT 0 //the interrupt id of the pin 2 on mega
#define SENSE_SMOOTH 1 //the minium latency between 2 signals in ms.

int gPriUnderObservation = 0;
void createPriResource();
OCStackResult registerPriResource();
void *ChangePriRepresentation();
unsigned long last_pir = 0;

/* Structure to represent a Pri resource */
typedef struct PRIRESOURCE{
    OCResourceHandle handle;
    bool state;
} PriResource;

static PriResource Pri;

/* Structure to represent the observers */
typedef struct {
    OCObservationId observationId;
    bool            valid;
    OCResourceHandle resourceHandle;
} Observers;
Observers interestedObservers[MAX_NUM_OBSERVATIONS];

static const char * GATEWAT_DISCOVERY_QUERY = "coap://192.168.1.100:5683/oc/core?rt=gw.sensor";
static const char * GATEWAT_REGISTER_QUERY = "coap://192.168.1.100:5683/gw/sensor";

/// This is the port which Arduino Server will use for all unicast communication with it's peers
static unsigned int OC_WELL_KNOWN_PORT = 5683;

#ifdef ARDUINOWIFI
// Arduino WiFi Shield
// Note : Arduino WiFi Shield currently does NOT support multicast and therefore
// this server will NOT be listening on 224.0.1.187 multicast address.

/// WiFi Shield firmware with Intel patches
static const char INTEL_WIFI_SHIELD_FW_VER[] = "1.2.0";

/// WiFi network info and credentials
char ssid[] = "mDNSAP";
char pass[] = "letmein9";

int ConnectToNetwork()
{
    char *fwVersion;
    int status = WL_IDLE_STATUS;
    // check for the presence of the shield:
    if (WiFi.status() == WL_NO_SHIELD)
    {
        OC_LOG(ERROR, TAG, PCF("WiFi shield not present"));
        return -1;
    }

    // Verify that WiFi Shield is running the firmware with all UDP fixes
    fwVersion = WiFi.firmwareVersion();
    OC_LOG_V(INFO, TAG, "WiFi Shield Firmware version %s", fwVersion);
    if ( strncmp(fwVersion, INTEL_WIFI_SHIELD_FW_VER, sizeof(INTEL_WIFI_SHIELD_FW_VER)) !=0 )
    {
        OC_LOG(DEBUG, TAG, PCF("!!!!! Upgrade WiFi Shield Firmware version !!!!!!"));
        return -1;
    }

    // attempt to connect to Wifi network:
    while (status != WL_CONNECTED)
    {
        OC_LOG_V(INFO, TAG, "Attempting to connect to SSID: %s", ssid);
        status = WiFi.begin(ssid,pass);

        // wait 10 seconds for connection:
        delay(10000);
    }
    OC_LOG(DEBUG, TAG, PCF("Connected to wifi"));

    IPAddress ip = WiFi.localIP();
    OC_LOG_V(INFO, TAG, "IP Address:  %d.%d.%d.%d", ip[0], ip[1], ip[2], ip[3]);
    return 0;
}
#else
// Arduino Ethernet Shield
int ConnectToNetwork()
{
    // Note: ****Update the MAC address here with your shield's MAC address****
    byte ETHERNET_MAC[] = {0x90, 0xA2, 0xDA, 0x36, 0xE1, 0x32};
    Serial.println("Connecting to ethernt...");
    int error = Ethernet.begin(ETHERNET_MAC);
    if (error  == 0)
    {
    	Serial.println("Failed to configure Ethernet using DHCP");
        return -1;
    }

    IPAddress ip = Ethernet.localIP();
    Serial.println(ip);
    return 0;
}
#endif //ARDUINOWIFI

// On Arduino Atmel boards with Harvard memory architecture, the stack grows
// downwards from the top and the heap grows upwards. This method will print
// the distance(in terms of bytes) between those two.
// See here for more details :
// http://www.atmel.com/webdoc/AVRLibcReferenceManual/malloc_1malloc_intro.html
void PrintArduinoMemoryStats()
{
    #ifdef ARDUINO_AVR_MEGA2560
    //This var is declared in avr-libc/stdlib/malloc.c
    //It keeps the largest address not allocated for heap
    extern char *__brkval;
    //address of tmp gives us the current stack boundry
    int tmp;
    OC_LOG_V(INFO, TAG, "Stack: %u         Heap: %u", (unsigned int)&tmp, (unsigned int)__brkval);
    OC_LOG_V(INFO, TAG, "Unallocated Memory between heap and stack: %u",
            ((unsigned int)&tmp - (unsigned int)__brkval));
    #endif
}

void ProcessObserveRegister (OCEntityHandlerRequest *ehRequest)
{
    OC_LOG_V (INFO, TAG, "Received registration request with observation Id %d",
            ehRequest->obsInfo.obsId);
    for (uint8_t i = 0; i < MAX_NUM_OBSERVATIONS; i++)
    {
        if (interestedObservers[i].valid == false)
        {
            interestedObservers[i].observationId = ehRequest->obsInfo.obsId;
            interestedObservers[i].valid = true;
            gPriUnderObservation = 1;
            break;
        }
    }
}

void ProcessObserveDeregister (OCEntityHandlerRequest *ehRequest)
{
    bool clientStillObserving = false;

    OC_LOG_V (INFO, TAG, "Received observation deregistration request for observation Id %d",
            ehRequest->obsInfo.obsId);
    Serial.println("Received observation deregistration");
    for (uint8_t i = 0; i < MAX_NUM_OBSERVATIONS; i++)
    {
        if (interestedObservers[i].observationId == ehRequest->obsInfo.obsId)
        {
            interestedObservers[i].valid = false;
        }
        if (interestedObservers[i].valid == true)
        {
            // Even if there is one single client observing we continue notifying entity handler
            clientStillObserving = true;
        }
    }
    if (clientStillObserving == false)
        gPriUnderObservation = 0;
}
//This function takes the request as an input and returns the response
//in JSON format.
char* constructJsonResponse (OCEntityHandlerRequest *ehRequest)
{
    cJSON *json = cJSON_CreateObject();
    cJSON *format;
    char *state;
    char *jsonResponse;

    cJSON_AddStringToObject(json,"href", "/sensor/pri");
    cJSON_AddItemToObject(json, "rep", format=cJSON_CreateObject());
    cJSON_AddBoolToObject(format, "motion", Pri.state);

    jsonResponse = cJSON_Print(json);
    cJSON_Delete(json);

    return jsonResponse;
}

// This is the entity handler for the registered resource.
// This is invoked by OCStack whenever it recevies a request for this resource.
OCEntityHandlerResult OCEntityHandlerCb(OCEntityHandlerFlag flag, OCEntityHandlerRequest * entityHandlerRequest )
{
    OCEntityHandlerResult ehRet = OC_EH_OK;
    OCEntityHandlerResponse response = {0};
    // MAX_RESPONSE_LENGTH for arduino is 256, for demo 64 is enough
    char payload[64] = {0};

    if(entityHandlerRequest && (flag & OC_REQUEST_FLAG))
    {
        OC_LOG (INFO, TAG, PCF("Flag includes OC_REQUEST_FLAG"));

        if(OC_REST_GET == entityHandlerRequest->method)
        {
            char *getResp = constructJsonResponse(entityHandlerRequest);
            size_t responsePayloadGetLength = strlen(getResp);
            if (responsePayloadGetLength < (sizeof(payload) - 1))
            {
                strncpy(payload, getResp, responsePayloadGetLength);
            }
            else
            {
                ehRet = OC_EH_ERROR;
            }

            free(getResp);
        }
        else if(OC_REST_PUT == entityHandlerRequest->method)
        {
            //Do something with the 'put' payload
            char *putResp = constructJsonResponse(entityHandlerRequest);

            size_t responsePayloadPutLength = strlen(putResp);
            if (responsePayloadPutLength < (sizeof(payload) - 1))
            {
                strncpy((char *)payload, putResp, responsePayloadPutLength);
            }
            else
            {
                ehRet = OC_EH_ERROR;
            }

            free(putResp);
        }

        if (ehRet == OC_EH_OK)
        {
            // Format the response.  Note this requires some info about the request
            response.requestHandle = entityHandlerRequest->requestHandle;
            response.resourceHandle = entityHandlerRequest->resource;
            response.ehResult = ehRet;
            response.payload = (unsigned char *)payload;
            response.payloadSize = strlen(payload);
            response.numSendVendorSpecificHeaderOptions = 0;
            memset(response.sendVendorSpecificHeaderOptions, 0, sizeof response.sendVendorSpecificHeaderOptions);
            memset(response.resourceUri, 0, sizeof response.resourceUri);
            // Indicate that response is NOT in a persistent buffer
            response.persistentBufferFlag = 0;

            // Send the response
            if (OCDoResponse(&response) != OC_STACK_OK)
            {
                OC_LOG(ERROR, TAG, "Error sending response");
                ehRet = OC_EH_ERROR;
            }
        }
    }

    if (entityHandlerRequest && (flag & OC_OBSERVE_FLAG))
    {
        if (OC_OBSERVE_REGISTER == entityHandlerRequest->obsInfo.action)
        {
            OC_LOG (INFO, TAG, PCF("Received OC_OBSERVE_REGISTER from client"));
            ProcessObserveRegister(entityHandlerRequest);
        }
        else if (OC_OBSERVE_DEREGISTER == entityHandlerRequest->obsInfo.action)
        {
            OC_LOG (INFO, TAG, PCF("Received OC_OBSERVE_DEREGISTER from client"));
            ProcessObserveDeregister(entityHandlerRequest);
        }
    }

    return ehRet;
}

boolean check_pir(int pir_pin)
{
    unsigned long t = millis();
    if((digitalRead(pir_pin)==HIGH) && (t-last_pir)>200)
    {
        last_pir = t;
        return true;
    }
    return false;
}

// This method is used to display 'Observe' functionality of OC Stack.
void *ChangePriRepresentation ()
{
    uint8_t j = 0;
    OCObservationId obsNotify[MAX_NUM_OBSERVATIONS];
    OCStackResult result = OC_STACK_ERROR;

    Pri.state = check_pir(PIR_PIN);
    if (gPriUnderObservation)
    {
        OC_LOG_V(INFO, TAG, " ===> Notifying stack of new Pri state %d\n", Pri.state);
        // Notify list of observers. Alternate observers on the list will be notified.
        j = 0;
        for (uint8_t i = 0; i < MAX_NUM_OBSERVATIONS; i++)
        {
            if (interestedObservers[i].valid == true)
            {
                obsNotify[j] = interestedObservers[i].observationId;
                j++;
            }
        }
        // MAX_RESPONSE_LENGTH for arduino is 256, for demo 64 is enough
        char obsResp[64] = {0};

        char *getResp = constructJsonResponse(NULL);
        size_t responsePayloadGetLength = strlen(getResp);
        strncpy(obsResp, getResp, responsePayloadGetLength);
        free(getResp);

        result = OCNotifyListOfObservers(Pri.handle, obsNotify, j,
                        (unsigned char *)obsResp, OC_NA_QOS);

        if ((OC_STACK_NO_OBSERVERS == result) || (OC_STACK_CONTINUE == result))
        {
            gPriUnderObservation = 0;
        }
    }

    return NULL;
}
#define W5200_CS 10
#define SDCARD_CS 4

//The setup function is called once at startup of the sketch
void setup()
{
    // Add your initialization code here
    // Note : This will initialize Serial port on Arduino at 115200 bauds
    OC_LOG_INIT();

    pinMode(SDCARD_CS, OUTPUT);
    digitalWrite(SDCARD_CS, HIGH); //Deselect the SD card

    pinMode(PIR_PIN, INPUT);
    delay(1000);
    Serial.println("OCServer is starting...");
    unsigned int port = OC_WELL_KNOWN_PORT;

    // Connect to Ethernet or WiFi network
    if (ConnectToNetwork() != 0)
    {
        Serial.println("Unable to connect to network");
        return;
    }

    Serial.println("Connected to Ethernet network");
    // Initialize the OC Stack in Server mode
    if (OCInit(NULL, port, OC_SERVER) != OC_STACK_OK)
    {
        Serial.println("OCStack init error");
        return;
    }

    Serial.println("OC Initialized");
    // Declare and create the resource: Pri
    createPriResource();
}

// The loop function is called in an endless loop
void loop()
{
    // This artificial delay is kept here to avoid endless spinning
    // of Arduino microcontroller. Modify it as per specfic application needs.
    delay(1000);

    // This call displays the amount of free SRAM available on Arduino
    PrintArduinoMemoryStats();
    if (OCProcess() != OC_STACK_OK)
    {
        Serial.println("OCStack process error");
        return;
    }

    ChangePriRepresentation();
}

void createPriResource()
{
    OCStackResult ret = OCCreateResource(&Pri.handle,
            "com.intel",
            "oc.mi.def",
            "/sensor/pri",
            OCEntityHandlerCb,
            OC_DISCOVERABLE | OC_OBSERVABLE);

    if (ret != OC_STACK_OK)
    {
        OC_LOG(ERROR, TAG, "Created Pri resource error");
	return;
    }

    Serial.println("Pri resource created");
    ret = registerPriResource();

    if (ret != OC_STACK_OK)
    {
    	Serial.println("Register Pri resource created");
    }
}

// This is a function called back when our resource is regestered at homegateway
OCStackApplicationResult registerReqCB(void* ctx, OCDoHandle handle,
        OCClientResponse * clientResponse)
{

    if (ctx == (void*) DEFAULT_CONTEXT_VALUE)
    {
        Serial.println("resource registered correctly");
    }

    if (clientResponse)
    {
        if (clientResponse->result == OC_STACK_OK)
        {
            Serial.println("clientResponse ok");

 	}
    }

    return OC_STACK_DELETE_TRANSACTION;
}


// This is a function called back when a homegateway is discovered
OCStackApplicationResult discoveryReqCB(void* ctx, OCDoHandle handle,
        OCClientResponse * clientResponse)
{

    char resource_addr[64] = {0};
    if (ctx == (void*) DEFAULT_CONTEXT_VALUE)
    {
        Serial.println("gw discovered correctly");
    }

    if (clientResponse)
    {
        if (clientResponse->result == OC_STACK_OK)
        {
            Serial.println("clientResponse ok");

            OCStackResult ret;
            OCDoHandle handle;
            cJSON *root = cJSON_CreateObject();
            cJSON *rep = cJSON_CreateArray();
            cJSON *format_rep, *format;
            char *jsonPayload;
            IPAddress ip = Ethernet.localIP();

            snprintf(resource_addr, sizeof(resource_addr), "coap://%d.%d.%d.%d:5683/oc/core?rt=com.intel", ip[0], ip[1], ip[2], ip[3]);
            cJSON_AddItemToObject(root, "oc", rep);
            cJSON_AddItemToArray(rep, format_rep=cJSON_CreateObject());
            cJSON_AddItemToObject(format_rep, "rep", format=cJSON_CreateObject());
            cJSON_AddStringToObject(format, "name", (char *)"pri");
            cJSON_AddStringToObject(format, "address", (char *)resource_addr);
            jsonPayload = cJSON_Print(root);
            cJSON_Delete(root);

            Serial.println(jsonPayload);
            OCCallbackData cbData;
            cbData.cb = registerReqCB;
            cbData.context = (void*)DEFAULT_CONTEXT_VALUE;
            cbData.cd = NULL;

            ret = OCDoResource(&handle, OC_REST_PUT, GATEWAT_REGISTER_QUERY, 0,
                       jsonPayload, OC_IPV4,
                       OC_LOW_QOS, &cbData, NULL, 0);

           if (ret != OC_STACK_OK)
           {
               Serial.println("register error");
               Serial.println(ret);
           }

           free(jsonPayload);
        }
    }

    return OC_STACK_DELETE_TRANSACTION;
}

OCStackResult registerPriResource()
{
    OCStackResult ret;
    OCCallbackData cbData;
    OCDoHandle handle;
    /* Start a homegateway discovery query*/
    Serial.println("Start Discovery ...");

    cbData.cb = discoveryReqCB;
    cbData.context = (void*)DEFAULT_CONTEXT_VALUE;
    cbData.cd = NULL;
    ret = OCDoResource(&handle, OC_REST_GET, GATEWAT_DISCOVERY_QUERY, 0, 0, OC_IPV4, OC_LOW_QOS, &cbData, NULL, 0);
    if (ret != OC_STACK_OK)
    {
    	Serial.println("Discovery failed");
        OC_LOG(ERROR, TAG, "OCStack resource error");
    }

    return ret;
}
