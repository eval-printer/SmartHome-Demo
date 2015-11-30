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

#include "rules_resource.h"
#include "homegateway.h"

RulesResource::RulesResource() : m_crazyJumping(false), m_kitchenMonitor(false), m_density(70), m_heartRate(95)
{
}

void RulesResource::registerResource()
{
	std::string resourceURI = RULES_RESOURCE_ENDPOINT;
	std::string resourceTypeName = RULES_RESOURCE_TYPE;
	std::string resourceInterface = DEFAULT_INTERFACE;
	EntityHandler cb = std::bind(&RulesResource::entityHandler, this,PH::_1);
	uint8_t resourceProperty = OC_DISCOVERABLE;
	OCStackResult result = OCPlatform::registerResource(m_resourceHandle,
	resourceURI,
	resourceTypeName,
	resourceInterface,
	cb,
	resourceProperty);

	if(OC_STACK_OK != result) {
		throw std::runtime_error(
		std::string("Config Resource failed to start")+std::to_string(result));
	}
}

OCRepresentation  RulesResource::get()
{
	m_rep.setUri(RULES_RESOURCE_ENDPOINT);
	m_rep.setValue("crazyJumping", m_crazyJumping);
	m_rep.setValue("kitchenMonitor", m_kitchenMonitor);
	m_rep.setValue("density", m_density);
	m_rep.setValue("heartRate", m_heartRate);
	return m_rep;
}

void  RulesResource::put(OCRepresentation rep)
{
	if (rep.hasAttribute("crazyJumping"))
		rep.getValue("crazyJumping", m_crazyJumping);
	if (rep.hasAttribute("kitchenMonitor"))
		rep.getValue("kitchenMonitor", m_kitchenMonitor);
	if (rep.hasAttribute("density"))
		rep.getValue("density", m_density);
	if (rep.hasAttribute("heartRate"))
		rep.getValue("heartRate", m_heartRate);
}

OCEntityHandlerResult  RulesResource::entityHandler(std::shared_ptr<OCResourceRequest> request)
{
	OCEntityHandlerResult ehResult = OC_EH_ERROR;
	if(request) {
		std::cout << "In entity handler for Rule Resource, URI is : "
			<< request->getResourceUri() << std::endl;

		if(request->getRequestHandlerFlag() == RequestHandlerFlag::RequestFlag) {
			auto pResponse = std::make_shared<OC::OCResourceResponse>();
			pResponse->setRequestHandle(request->getRequestHandle());
			pResponse->setResourceHandle(request->getResourceHandle());

			if(request->getRequestType() == "GET") {
				std::cout<<"Rule Get Request"<<std::endl;
				pResponse->setErrorCode(200);
				pResponse->setResourceRepresentation(get(), "");
				if(OC_STACK_OK == OCPlatform::sendResponse(pResponse)) {
					ehResult = OC_EH_OK;
				}
			}
			else if(request->getRequestType() == "PUT") {
				std::cout <<"Rule Put Request"<<std::endl;
				put(request->getResourceRepresentation());
				pResponse->setErrorCode(200);
				pResponse->setResourceRepresentation(get(), "");
				if(OC_STACK_OK == OCPlatform::sendResponse(pResponse)) {
					ehResult = OC_EH_OK;
				}
			}
			else {
				std::cout << "Rule unsupported request type"
				<< request->getRequestType() << std::endl;
				pResponse->setResponseResult(OC_EH_ERROR);
				OCPlatform::sendResponse(pResponse);
				ehResult = OC_EH_ERROR;
			}
		}
		else {
			std::cout << "Rule unsupported request flag" <<std::endl;
		}
	}

	return ehResult;
}
