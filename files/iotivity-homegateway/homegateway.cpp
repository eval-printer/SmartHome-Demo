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

/// The purpose of this server is to simulate a homegateway that contains a config resource for
/// its description, a light resource for the internal light, and 2 door resources for the purpose

#include "homegateway.h"
#include "config_resource.h"
#include "sensor_resource.h"
#include "rules_resource.h"

gboolean active_check_cb(gpointer user_data)
{
	SensorResource *sensor = (SensorResource *)user_data;
	sensor->activeCheck();

	return false;
}

gboolean sensor_expire_cb(gpointer user_data)
{
	SensorResource *sensor = (SensorResource *)user_data;
	sensor->resetStatus();

	//by default we reset the status every 5 seconds and 
	//the sensor will send active notification every 1.5 seconds
	//the active check interval is 4 seconds, then we may have
	//at lease receive two active notificaiton form the sensor
	g_timeout_add_seconds(ACTIVE_INTERVAL, active_check_cb, sensor);

	return true;
}

GMainLoop *loop = NULL;

void handle_signal(int signal)
{
	g_main_loop_quit(loop);
}

int main ()
{
	struct sigaction sa;
	sigfillset(&sa.sa_mask);
	sa.sa_flags = 0;
	sa.sa_handler = handle_signal;
	sigaction(SIGINT, &sa, NULL);
	cout << "Press Ctrl-C to quit...." << endl;

	bt_initialize();

	PlatformConfig cfg
	{
		ServiceType::InProc,
		ModeType::Both,
		"0.0.0.0", // By setting to "0.0.0.0", it binds to all available interfaces
		8888,         // Uses 8888 for homegateway
		QualityOfService::HighQos
	};

	std::cout << "Initializing gateway platform config" << endl;
	OCPlatform::Configure(cfg);

	ConfigResource config;
	RulesResource rule;
	SensorResource sensor(&rule);

	rule.registerResource();

	bt_device_set_connection_state_changed_cb(bt_conn_state_changed_cb, &sensor);

	loop = g_main_loop_new(NULL, FALSE);

	g_timeout_add_seconds(DEFAULT_TIMEOUT, sensor_expire_cb, &sensor);
	g_main_loop_run(loop);

	return 0;
}
