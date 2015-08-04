inherit scons

SUMMARY = "Iotivity application for several sensors in the DollHouse demo"
DESCRIPTION = "Iotivity Sensors APP."
HOMEPAGE = "https://www.iotivity.org/"
DEPENDS = "iotivity"
SECTION = "apps"
LICENSE = "Apache-2.0"
LIC_FILES_CHKSUM = "file://iotivity-sensors.cpp;beginline=1;endline=19;md5=fc5a615cf1dc3880967127bc853b3e0c"

SRC_URI = "file://iotivity-sensors.tar.bz2 \
          "

S = "${WORKDIR}/iotivity-sensors"

IOTIVITY_BIN_DIR = "/opt/iotivity"
IOTIVITY_BIN_DIR_D = "${D}${IOTIVITY_BIN_DIR}"

do_install() {
    install -d ${IOTIVITY_BIN_DIR_D}/apps/iotivity-sensors
    install -c -m 555 ${S}/output/iotivity-sensors ${IOTIVITY_BIN_DIR_D}/apps/iotivity-sensors
}

FILES_${PN} = "${IOTIVITY_BIN_DIR}/apps/iotivity-sensors/iotivity-sensors"
FILES_${PN}-dbg = "${IOTIVITY_BIN_DIR}/apps/iotivity-sensors/.debug"
RDEPENDS_${PN} += "iotivity"
BBCLASSEXTEND = "native nativesdk"

