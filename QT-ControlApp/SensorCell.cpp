#include <QApplication>
#include <QPushButton>
#include <QCheckBox>
#include <QLabel>
#include <QComboBox>
#include <QVBoxLayout>
#include <QResizeEvent>
#include <QPainter>
#include <QPagedPaintDevice>

#include "IoTivity.h"
#include "SensorCell.h"

#define SUBWIDGET_NAME          "buttonCellSubWidget"

SensorCell::SensorCell(QWidget *parent) :
    QWidget(parent),
    m_sensor(nullptr)
{
}

/* Create Button and set style, Icon */
void SensorCell::setObjectName(const QString &name)
{
    QWidget::setObjectName(name);

    QPushButton *commonButton;
    QVBoxLayout *layout;

    layout = new QVBoxLayout(this);

    commonButton = new QPushButton(this);
    commonButton->setObjectName(name);
    commonButton->setEnabled(false);

    commonButton->setMinimumWidth(
                parentWidget()->parentWidget()->geometry().width() / 2 * IoTivity::SENSOR_CELL_MIN_WIDHT_SCALE);
    commonButton->setMinimumHeight(
                parentWidget()->parentWidget()->geometry().height() / 3 * IoTivity::SENSOR_CELL_MIN_HEIGHT_SCALE);
    layout->addWidget(commonButton);

    if (objectName().toStdString() == IoTivity::IOTIVITY_FAN)
    {

        commonButton->setIcon(QIcon(QString::fromStdString(IoTivity::FAN_STATIC_ICON)));
        commonButton->setIconSize(commonButton->size());
    }
    else if (objectName().toStdString() == IoTivity::IOTIVITY_HEARTRATE)
    {
        QFont heartRateFont;
        heartRateFont = commonButton->font();
        heartRateFont.setPointSize(IoTivity::HEART_RATE_FONT_SIZE);
        heartRateFont.setKerning(true);
        commonButton->setFont(heartRateFont);

        QPixmap pixmap;
        pixmap = QPixmap(QString::fromStdString(IoTivity::HEART_RATE_ICON));

        QLabel *label = new QLabel(commonButton);
        label->setPixmap(pixmap);
    }
    else if (objectName().toStdString() == IoTivity::IOTIVITY_GAS)
    {
        commonButton->setIcon(QIcon(QString::fromStdString(IoTivity::GAS_ICON)));
        commonButton->setIconSize(commonButton->size());
    }
    else if (objectName().toStdString() == IoTivity::IOTIVITY_MOTION)
    {
    }
    else if (objectName().toStdString() == IoTivity::IOTIVITY_LED)
    {
        commonButton->setIcon(QIcon(QString::fromStdString(IoTivity::LED_RED_ICON)));
        commonButton->setIconSize(commonButton->size());
    }
    else if (objectName().toStdString() == IoTivity::IOTIVITY_GATWAY)
    {
    }
    else
        qDebug("Don't Support type %s", objectName().toStdString().c_str());

    /* Set All Group Brush Color */
    QPalette palette = QPalette(Qt::white);

    palette.setBrush(QPalette::BrightText, Qt::white);
    palette.setBrush(QPalette::Base, Qt::white);
    palette.setBrush(QPalette::Highlight, Qt::white);
    /*
    setTexture(palette, QPalette::Button, buttonImage);
    setTexture(palette, QPalette::Mid, midImage);
    setTexture(palette, QPalette::Window, backgroundImage);
    */

    /* Set background brush color */
    QBrush brush = palette.background();
    brush.setColor(QColor(IoTivity::BUTTON_BACKGROUND_COLOR_R,
                          IoTivity::BUTTON_BACKGROUND_COLOR_R,
                          IoTivity::BUTTON_BACKGROUND_COLOR_R));

    /* Set Disabled state color */
    palette.setBrush(QPalette::Disabled, QPalette::WindowText, brush);
    palette.setBrush(QPalette::Disabled, QPalette::Text, brush);
    palette.setBrush(QPalette::Disabled, QPalette::ButtonText, brush);
    palette.setBrush(QPalette::Disabled, QPalette::Base, brush);
    palette.setBrush(QPalette::Disabled, QPalette::Button, brush);
    palette.setBrush(QPalette::Disabled, QPalette::Mid, brush);

    commonButton->setPalette(palette);

    connect(commonButton, SIGNAL(clicked()), this, SLOT(onButtonClicked()));

    setLayout(layout);
}

void SensorCell::updateSensorCell(const OC::OCRepresentation &rep)
{
    if (objectName().toStdString() == IoTivity::IOTIVITY_FAN)
    {
        updateFanCell(rep);
    }
    else if (objectName().toStdString() == IoTivity::IOTIVITY_HEARTRATE)
    {
        updateHeatRateCell(rep);
    }
    else if (objectName().toStdString() == IoTivity::IOTIVITY_GAS)
    {
        updateGasCell(rep);
    }
    else if (objectName().toStdString() == IoTivity::IOTIVITY_MOTION)
    {
        updateMotionCell(rep);
    }
    else if (objectName().toStdString() == IoTivity::IOTIVITY_LED)
    {
        updateLEDCell(rep);
    }
    else if (objectName().toStdString() == IoTivity::IOTIVITY_GATWAY)
    {
        updateMinowmaxCell(rep);
    }
    else
        qDebug("Unknown type");
}

void SensorCell::sensorAdded(std::shared_ptr< OC::OCResource > sensor)
{
    QPushButton *button;

    if (objectName().toStdString() != sensor->uri())
        return;

    m_sensor = sensor;

    button = findChild< QPushButton* >(objectName());

    button->setEnabled(true);
}

void SensorCell::sensorRemoved(std::shared_ptr< OC::OCResource > sensor)
{
    QPushButton *button;

    if (objectName().toStdString() != sensor->uri())
        return;

    button = findChild< QPushButton* >(objectName());

    button->setEnabled(false);
    button->update();

    m_sensor = nullptr;
}

void SensorCell::sensorChanged(const OC::OCRepresentation &sensor)
{
    SensorChangedEvent *event;

    if (sensor.getUri() != objectName().toStdString())
        return;

    event = new SensorChangedEvent(static_cast< QEvent::Type >(QEvent::User + 1));
    event->setSensorRepresentation(sensor);
    QApplication::instance()->postEvent(this, event);
}

bool SensorCell::event(QEvent *event)
{
    SensorChangedEvent *sensorEvent;

    if (event->type() != QEvent::User + 1)
        return QWidget::event(event);

    sensorEvent = dynamic_cast< SensorChangedEvent* >(event);
    updateSensorCell(sensorEvent->m_rep);

    return true;
}

SensorCell::SensorChangedEvent::SensorChangedEvent(QEvent::Type type)
    : QEvent(type)
{
}

void SensorCell::SensorChangedEvent::setSensorRepresentation(const OC::OCRepresentation &rep)
{
    m_rep = rep;
}

void SensorCell::onButtonClicked()
{
    QPushButton *pButton;

    if (m_sensor == nullptr)
        return;

    pButton = dynamic_cast< QPushButton* >(sender());
    if (pButton->objectName().toStdString() == IoTivity::IOTIVITY_FAN)
    {
        std::string state, newState;
        OC::OCRepresentation rep;

        m_representation.getValue(IoTivity::IOTIVITY_FAN_KEY, state);
        if (state == "off")
            newState = "on";
        else
            newState = "off";

        rep.setValue(IoTivity::IOTIVITY_FAN_KEY, newState);
        m_sensor->put(rep, OC::QueryParamsMap(), IoTivity::onPut);
    }
    else if (objectName().toStdString() == IoTivity::IOTIVITY_LED)
    {
        int color, newColor;
        OC::OCRepresentation rep;

        m_representation.getValue(IoTivity::IOTIVITY_LED_KEY, color);
        if (color == 11)
            newColor = 9;
        else
            newColor = color + 1;

        rep.setValue(IoTivity::IOTIVITY_LED_KEY, newColor);

        m_sensor->put(rep, OC::QueryParamsMap(), IoTivity::onPut);
    }
}

void SensorCell::onFanCheckBoxClicked(bool checkedState)
{
    std::string state;
    OC::OCRepresentation rep;

    state = checkedState == Qt::Unchecked ? "off" : "on";

    rep.setValue(IoTivity::IOTIVITY_FAN_KEY, state);

    m_sensor->put(rep, OC::QueryParamsMap(), IoTivity::onPut);
}

void SensorCell::onLEDColorChanged(QString value)
{
    int color;
    OC::OCRepresentation rep;

    if (value == "Red")
        color = 9;
    else if (value == "Blue")
        color = 10;
    else if (value == "Green")
        color = 11;

    rep.setValue(IoTivity::IOTIVITY_LED_KEY, color);

    m_sensor->put(rep, OC::QueryParamsMap(), IoTivity::onPut);
}

void SensorCell::updateFanCell(const OC::OCRepresentation &rep)
{
    std::string value;
    QPushButton *pFanButton;

    m_representation = rep;

    /* Change Icon */
    rep.getValue(IoTivity::IOTIVITY_FAN_KEY, value);

    /* Set Button content */
    pFanButton = findChild< QPushButton* >(QString::fromStdString(IoTivity::IOTIVITY_FAN));

    if (value == "off")
        pFanButton->setIcon(QIcon(QString::fromStdString(IoTivity::FAN_STATIC_ICON)));
    else
        pFanButton->setIcon(QIcon(QString::fromStdString(IoTivity::FAN_WIND_ICON)));
}

void SensorCell::updateHeatRateCell(const OC::OCRepresentation &rep)
{
    int rate;
    QPushButton *pButton;

    m_representation = rep;

    rep.getValue("heartRate", rate);

    /* Set Button content */
    pButton = findChild< QPushButton* >(QString::fromStdString(IoTivity::IOTIVITY_HEARTRATE));
    pButton->setText(QString::fromStdString("            " + std::to_string(rate)));
}

void SensorCell::updateGasCell(const OC::OCRepresentation &rep)
{
    int value;
    QPushButton *pGasButton;

    m_representation = rep;

    rep.getValue("density", value);

    /* Change gas Icon */
    pGasButton = findChild< QPushButton* >(QString::fromStdString(IoTivity::IOTIVITY_GAS));

    if (value > IoTivity::densityDefineValue)
        pGasButton->setIcon(QIcon(QString::fromStdString(IoTivity::FIRE_ICON)));
    else
        pGasButton->setIcon(QIcon(QString::fromStdString(IoTivity::GAS_ICON)));
}

void SensorCell::updateMotionCell(const OC::OCRepresentation &rep)
{
    m_representation = rep;
}

void SensorCell::updateLEDCell(const OC::OCRepresentation &rep)
{
    int color;
    QPushButton *pLEDButton;

    m_representation = rep;

    rep.getValue("ledColor", color);

    pLEDButton = findChild< QPushButton* >(QString::fromStdString(IoTivity::IOTIVITY_LED));
    if (color == 9)
        pLEDButton->setIcon(QIcon(QString::fromStdString(IoTivity::LED_RED_ICON)));
    else if (color == 10)
        pLEDButton->setIcon(QIcon(QString::fromStdString(IoTivity::LED_BLUE_ICON)));
    else if (color == 11)
        pLEDButton->setIcon(QIcon(QString::fromStdString(IoTivity::LED_GREEN_ICON)));

    pLEDButton->update();
}

void SensorCell::updateMinowmaxCell(const OC::OCRepresentation &rep)
{

}
