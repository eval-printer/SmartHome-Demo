# IoTivity-Arduino-Sensors
- Build the iotivity for Arduino once so that it downloads the arduino-1.5.8 sdk and patches correctly.
  $ scons TARGET_OS=arduino TARGET_ARCH=avr BOARD=MEGA SHIELD=ETH
- Then use the following steps to build the apps
- Download the library for the Seeedstudio W5200 Ethernet Shield
  $ git clone https://github.com/Seeed-Studio/Ethernet_Shield_W5200.git EthernetV2
  $ move EthernetV2 folder to extlibs/arduino/arduino-1.5.8/libraries under the iotivity v0.9.1 folder
- Copy the files: fan.cpp, prisensor.cpp, gassensor.cpp, SConscript resource/csdk/stack/samples/arduino/SimpleClientServer/ocserver/ under iotivity v0.9.1 folder
- Change to the iotivity v0.9.1 folder and build
  $ scons TARGET_OS=arduino TARGET_ARCH=avr BOARD=mega SHIELD=ETH

- NOTES
- To build for arduino, iotivity will try to download the arduino-1.5.8 sdk to folder extlibs/arduino/ under iotivity v0.91 folder and patches it. In case of Linux, it will need the dos2unix tool. Please install it before hand or the build will brake.
- The .hex files will be located at out/arduino/avr/release/resource/csdk/stack/samples/arduino/SimpleClientServer/ocserver/ under the iotivity v0.9.1 folder
- Use the following command to flash an app to the mega2560 board. Current folder is v0.9.1
  $ sudo ./extlibs/arduino/arduino-1.5.8/hardware/tools/avr/bin/avrdude -C./extlibs/arduino/arduino-1.5.8/hardware/tools/avr/etc/avrdude.conf -v -v -v -v -patmega2560 -cwiring -P/dev/ttyACM0 -b115200 -D -Uflash:w:./out/arduino/avr/release/resource/csdk/stack/samples/arduino/SimpleClientServer/ocserver/fan.hex:i
