#******************************************************************
#
# Copyright 2014 Intel Mobile Communications GmbH All Rights Reserved.
#
#-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#      http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#
#-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

Import('env')

arduino_simplecs_env = env.Clone()
######################################################################
# Build flags
######################################################################
arduino_simplecs_env.AppendUnique(CCFLAGS = ['-fpermissive'])

arduino_simplecs_env.PrependUnique(CPPPATH = [
		env.get('BUILD_DIR') + '/resource/csdk/logger/include',
		env.get('BUILD_DIR') + '/resource/csdk/stack/include',
		env.get('BUILD_DIR') + '/resource/oc_logger/include',
		env.get('BUILD_DIR') + '/extlibs/Ethernet_Shield_W5200',
		env.get('BUILD_DIR') + '/extlibs/cjson'
		])

arduino_simplecs_env.AppendUnique(LIBPATH = [env.get('BUILD_DIR')])
arduino_simplecs_env.AppendUnique(CPPDEFINES = ['TB_LOG'])

arduino_simplecs_env.PrependUnique(LIBS = ['octbstack', 'connectivity_abstraction','coap'])

arduino_simplecs = arduino_simplecs_env.Program('fan', 'fan.cpp')
arduino_simplecs_env.Program('prisensor', 'prisensor.cpp')
arduino_simplecs_env.Program('gassensor', 'gassensor.cpp')
env.CreateBin('fan')
env.CreateBin('prisensor')
env.CreateBin('gassensor')

i_arduino_simplecs = arduino_simplecs_env.Install(env.get('BUILD_DIR'), arduino_simplecs)

Alias('arduino_simplecs', i_arduino_simplecs)
arduino_simplecs_env.Default('fan.hex')
arduino_simplecs_env.Default('prisensor.hex')
arduino_simplecs_env.Default('gassensor.hex')
