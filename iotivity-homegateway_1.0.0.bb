inherit scons

SUMMARY = "Iotivity Homegateway"
DESCRIPTION = "Iotivity Homegateway Daemon."
HOMEPAGE = "https://www.iotivity.org/"
DEPENDS = "iotivity"
SECTION = "apps"
LICENSE = "Apache-2.0"
LIC_FILES_CHKSUM = "file://homegateway.cpp;beginline=1;endline=19;md5=fc5a615cf1dc3880967127bc853b3e0c"

SRC_URI = "file://iotivity-homegateway.tar.bz2 \
          "

S = "${WORKDIR}/iotivity-homegateway"

IOTIVITY_BIN_DIR = "/opt/iotivity"
IOTIVITY_BIN_DIR_D = "${D}${IOTIVITY_BIN_DIR}"

do_install() {
    install -d ${IOTIVITY_BIN_DIR_D}/apps/iotivity-homegateway
    install -c -m 555 ${S}/output/homegateway ${IOTIVITY_BIN_DIR_D}/apps/iotivity-homegateway
}

FILES_${PN} = "${IOTIVITY_BIN_DIR}/apps/iotivity-homegateway/homegateway"
FILES_${PN}-dbg = "${IOTIVITY_BIN_DIR}/apps/iotivity-homegateway/.debug"
RDEPENDS_${PN} += "iotivity"
BBCLASSEXTEND = "native nativesdk"

