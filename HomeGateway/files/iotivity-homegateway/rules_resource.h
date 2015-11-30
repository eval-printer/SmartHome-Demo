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

#ifndef RULES_RESOURCE_H_
#define RULES_RESOURCE_H_
#include "resource.h"

class RulesResource : public Resource
{
	public:
	bool m_crazyJumping, m_kitchenMonitor;
	int m_density, m_heartRate;
	RulesResource();

	void registerResource();
	private:
	OCRepresentation get();
	void put(OCRepresentation rep);
	protected:
	virtual OCEntityHandlerResult entityHandler(std::shared_ptr<OCResourceRequest> request);
};

#endif /* RULES_RESOURCE_H_ */
