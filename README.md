# IoTivity-Arduino-Sensors
Use the following steps to build the apps
- Download the library for the Seeedstudio W5200 Ethernet Shield
 $ git clone https://github.com/Seeed-Studio/Ethernet_Shield_W5200.git 
 $ move Ethernet_Shield_W5200 folder to path of iotivity v0.9.1>/extlibs directory
- Copy the all the files: fan.cpp, prisensor.cpp, gassensor.cpp, SConscript to <path to iotivity v0.9.1>/resource/csdk/stack/samples/arduino/SimpleClientServer/ocserver/
- Move to the iotivity v0.9.1 folder and build
 $ cd <path to iotivity v0.91>
 $ scons TARGET_OS=arduino TARGET_ARCH=avr BOARD=mega SHIELD=ETH

NOTES
- To build for arduino, iotivity will try to download the arduino-1.5.8 sdk to <path to iotiity v0.9.1>/extlibs/arduino/ and patches it. In case of Linux, it will need the dos2unix tool. Please install it before hand or the build will brake.
- The .hex files will be located at <path to iotivity v0.9.1>/out/arduino/avr/release/resource/csdk/stack/samples/arduino/SimpleClientServer/ocserver/
- Use the following command to flash an app to the mega2560 board, ie:
 $ sudo <path to iotivity v0.9.1>/extlibs/arduino/arduino-1.5.8/hardware/tools/avr/bin/avrdude -C<path to iotivity v0.9.1>/extlibs/arduino/arduino-1.5.8/hardware/tools/avr/etc/avrdude.conf -v -v -v -v -patmega2560 -cwiring -P/dev/ttyACM0 -b115200 -D -Uflash:w:<path to iotivity v0.9.1>/out/arduino/avr/release/resource/csdk/stack/samples/arduino/SimpleClientServer/ocserver/fan.hex:i
