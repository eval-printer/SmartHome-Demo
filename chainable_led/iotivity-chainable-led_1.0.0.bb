inherit scons

SUMMARY = "Iotivity chainable LED for Edison"
DESCRIPTION = "Iotivity Chainable Led Simple APP."
HOMEPAGE = "https://www.iotivity.org/"
DEPENDS = "iotivity"
SECTION = "apps"
LICENSE = "Apache-2.0"
LIC_FILES_CHKSUM = "file://chainable_led_edison.cpp;beginline=1;endline=19;md5=fc5a615cf1dc3880967127bc853b3e0c"

SRC_URI = "file://iotivity-chainable-led.tar.bz2 \
          "

S = "${WORKDIR}/iotivity-chainable-led"

IOTIVITY_BIN_DIR = "/opt/iotivity"
IOTIVITY_BIN_DIR_D = "${D}${IOTIVITY_BIN_DIR}"

do_install() {
    install -d ${IOTIVITY_BIN_DIR_D}/apps/iotivity-chainable-led
    install -c -m 555 ${S}/output/iotivity-chainable-led ${IOTIVITY_BIN_DIR_D}/apps/iotivity-chainable-led
}

FILES_${PN} = "${IOTIVITY_BIN_DIR}/apps/iotivity-chainable-led/iotivity-chainable-led"
FILES_${PN}-dbg = "${IOTIVITY_BIN_DIR}/apps/iotivity-chainable-led/.debug"
RDEPENDS_${PN} += "iotivity"
BBCLASSEXTEND = "native nativesdk"

