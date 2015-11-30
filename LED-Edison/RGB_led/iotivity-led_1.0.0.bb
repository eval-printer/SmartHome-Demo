inherit scons

SUMMARY = "Iotivity LED for Edison"
DESCRIPTION = "Iotivity Led Simple APP."
HOMEPAGE = "https://www.iotivity.org/"
DEPENDS = "iotivity"
SECTION = "apps"
LICENSE = "Apache-2.0"
LIC_FILES_CHKSUM = "file://led_edison.cpp;beginline=1;endline=19;md5=fc5a615cf1dc3880967127bc853b3e0c"

SRC_URI = "file://iotivity-led.tar.bz2 \
          "

S = "${WORKDIR}/iotivity-led"

IOTIVITY_BIN_DIR = "/opt/iotivity"
IOTIVITY_BIN_DIR_D = "${D}${IOTIVITY_BIN_DIR}"

do_install() {
    install -d ${IOTIVITY_BIN_DIR_D}/apps/iotivity-led
    install -c -m 555 ${S}/output/iotivity-led ${IOTIVITY_BIN_DIR_D}/apps/iotivity-led
}

FILES_${PN} = "${IOTIVITY_BIN_DIR}/apps/iotivity-led/iotivity-led"
FILES_${PN}-dbg = "${IOTIVITY_BIN_DIR}/apps/iotivity-led/.debug"
RDEPENDS_${PN} += "iotivity"
BBCLASSEXTEND = "native nativesdk"

