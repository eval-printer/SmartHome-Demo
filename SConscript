Import('env')

arduino_simplecs_env = env.Clone()
######################################################################
# Build flags
######################################################################
arduino_simplecs_env.PrependUnique(CPPPATH = [
		'../../../../../ocsocket/include',
		'../../../../../logger/include',
		'../../../../../stack/include',
		'../../../../../../oc_logger/include',
		'../../../../../../../extlibs/cjson'
		])

arduino_simplecs_env.AppendUnique(LIBPATH = [env.get('BUILD_DIR')])
arduino_simplecs_env.PrependUnique(LIBS = ['octbstack', 'coap', 'm'])
arduino_simplecs_env.AppendUnique(CPPDEFINES = ['TB_LOG', 'ARDUINO_AVR_MEGA2560'])

arduino_simplecs_env.Program('gassensor', 'gassensor.cpp')
arduino_simplecs_env.Program('prisensor', 'prisensor.cpp')
arduino_simplecs_env.Program('fan', 'fan.cpp')
env.CreateBin('gassensor')
env.CreateBin('prisensor')
env.CreateBin('fan')

