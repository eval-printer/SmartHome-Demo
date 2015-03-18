#ifndef IOTIVITY_H
#define IOTIVITY_H

#include <memory>
#include "OCApi.h"

namespace IoTivity {
    const int DEMO_WIDTH = 750;
    const int DEMO_HEIGHT = 800;

    const int TITLE_FONT_SIZE = 30;
    const int RULE_FONT_SIZE = 20;
    const int HEART_RATE_FONT_SIZE = 25;

    /* Button background color, that uses to harmony Icon color */
    const int BUTTON_BACKGROUND_COLOR_R = 255;
    const int BUTTON_BACKGROUND_COLOR_G = 255;
    const int BUTTON_BACKGROUND_COLOR_B = 255;

    const std::string FAN_STATIC_ICON = ":/images/static.png";
    const std::string FAN_WIND_ICON = ":/images/wind.png";
    const std::string HEART_RATE_ICON = ":/images/heartrate.png";
    const std::string GAS_ICON = ":/images/gas.jpg";
    const std::string FIRE_ICON = ":/images/fire.png";
    const std::string LED_RED_ICON = ":/images/red-lightbulb-md.png";
    const std::string LED_BLUE_ICON = ":/images/blue-lightbulb-md.png";
    const std::string LED_GREEN_ICON = ":/images/green-lightbulb-md.png";

    /*
     *       Width Scale
     *     |<--------->|
     * |<------------------------------------->|
     *
     * +-------------------+-------------------+
     * |                   |                   |
     * |   +-----------+   |   +-----------+   |
     * |   |   Cell    |   |   |   Cell    |   |
     * |   +-----------+   |   +-----------+   |
     * |                   |                   |
     * +-------------------+-------------------+
     *
     */
    const float SENSOR_CELL_MIN_WIDHT_SCALE = 1.0f;
    const float SENSOR_CELL_MIN_HEIGHT_SCALE = 1.0f;

    /*
     *      RULE_ITEM_MIN_WIDTH_SCALE
     *         |<--------------->|
     * |<------------------------------------->|
     *
     * +---------------------------------------+
     * |                                       |
     * |       +-----------------+ +----+      |
     * |       |Enable Kitch Gas | |    |      |
     * |       +-----------------+ +----+      |
     * |                                       |
     * +---------------------------------------+
     */
    const float RULE_ITEM_MIN_WIDTH_SCALE = 1.0f / 2.0f;
    const float RULE_ITEM_MIN_HEIGHT_SCALE = 1.0f / 2.0f;

    const std::string IOTIVITY_FAN = "/a/fan";
    const std::string IOTIVITY_HEARTRATE = "/sensor/heartrate";
    const std::string IOTIVITY_GAS = "/sensor/gas";
    const std::string IOTIVITY_MOTION ="/sensor/pri";
    const std::string IOTIVITY_LED = "/led_edison";
    const std::string IOTIVITY_GATWAY = "/gw/sensor";

    const std::string IOTIVITY_FAN_KEY = "fanstate";
    const std::string IOTIVITY_LED_KEY = "ledColor";

    const std::string IOTIVITY_KITCH_RULE = "kitchenMonitor";
    const std::string IOTIVITY_CRAZY_JUMPING_RULE = "crazyJumping";

    void extern foundResource(std::shared_ptr< OC::OCResource > resource);
    void extern onGet(const OC::HeaderOptions& headerOptions, const OC::OCRepresentation &rep, const int eCode);
    void extern onPut(const OC::HeaderOptions& headerOptions, const OC::OCRepresentation &rep, const int eCode);
    void extern onObserve(const OC::HeaderOptions headerOptions, const OC::OCRepresentation& rep,
                          const int& eCode, const int& sequenceNumber);

    int extern densityDefineValue;

} //namespace IoTivity

#endif // IOTIVITY_H
