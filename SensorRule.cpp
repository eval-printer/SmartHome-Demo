#include <QApplication>
#include <QLabel>
#include <QCheckBox>
#include <QFormLayout>
#include <QComboBox>

#include "IoTivity.h"
#include "SensorRule.h"

SensorRule::SensorRule(QWidget *parent) :
    QWidget(parent),
    m_resource(nullptr),
    m_kitchGas(NULL),
    m_crazyJumping(NULL)
{
}

void SensorRule::setObjectName(const QString &name)
{
    QWidget::setObjectName(name);

    QGridLayout *ruleLayout;
    QFont rule_font;
    QLabel *defineDensityMax, *defineHeartRate;

    m_kitchGas = new QCheckBox(tr("Enable Kitch Gas           "), this);
    m_crazyJumping = new QCheckBox(tr("Enable Crazy Jumping"), this);

    defineDensityMax = new QLabel(tr("Density: "), this);
    defineHeartRate = new QLabel(tr("Heart Rate: "), this);
    m_densitySelect = new QComboBox(this);
    m_heartRateSelect = new QComboBox(this);

    m_densitySelect->insertItems(0, QStringList()
                                 << QApplication::translate("Dialog", "6", 0)
                                 << QApplication::translate("Dialog", "7", 0)
                                 << QApplication::translate("Dialog", "8", 0)
                                 << QApplication::translate("Dialog", "9", 0)
                                 << QApplication::translate("Dialog", "10", 0)
                                 << QApplication::translate("Dialog", "11", 0)
                                 << QApplication::translate("Dialog", "12", 0));

    m_heartRateSelect->insertItems(0, QStringList()
                                 << QApplication::translate("Dialog", "90", 0)
                                 << QApplication::translate("Dialog", "100", 0)
                                 << QApplication::translate("Dialog", "110", 0)
                                 << QApplication::translate("Dialog", "120", 0));

    rule_font = m_crazyJumping->font();
    rule_font.setPointSize(IoTivity::RULE_FONT_SIZE);
    rule_font.setBold(true);
    rule_font.setItalic(true);
    rule_font.setKerning(true);
    m_kitchGas->setFont(rule_font);
    m_crazyJumping->setFont(rule_font);

    ruleLayout = new QGridLayout(this);
    ruleLayout->addWidget(m_kitchGas, 0, 0, 1, 1, Qt::AlignCenter);
    ruleLayout->addWidget(m_crazyJumping, 1, 0, 1, 1, Qt::AlignCenter);
    ruleLayout->addWidget(defineDensityMax, 0, 1, 1, 1, Qt::AlignCenter);
    ruleLayout->addWidget(m_densitySelect, 0, 2, 1, 1, Qt::AlignCenter);
    ruleLayout->addWidget(defineHeartRate, 1, 1, 1, 1, Qt::AlignCenter);
    ruleLayout->addWidget(m_heartRateSelect, 1, 2, 1, 1, Qt::AlignCenter);

    m_kitchGas->setMinimumWidth(
                parentWidget()->parentWidget()->geometry().width() * IoTivity::RULE_ITEM_MIN_WIDTH_SCALE);
    m_crazyJumping->setMinimumWidth(
                parentWidget()->parentWidget()->geometry().width() * IoTivity::RULE_ITEM_MIN_WIDTH_SCALE);

    setLayout(ruleLayout);
    ruleLayout->setAlignment(Qt::AlignCenter);

    setEnabled(false);

    QObject::connect(m_kitchGas, SIGNAL(clicked(bool)), this, SLOT(onKitchChecked(bool)));
    QObject::connect(m_crazyJumping, SIGNAL(clicked(bool)), this, SLOT(onCrazyJumpingChecked(bool)));
    QObject::connect(m_densitySelect, SIGNAL(currentIndexChanged(QString)), this, SLOT(onDensitySelectChanged(QString)));
    QObject::connect(m_heartRateSelect, SIGNAL(currentIndexChanged(QString)), this, SLOT(onHeartRateSelectChanged(QString)));
}

void SensorRule::updateRuleView(const OC::OCRepresentation &rep)
{
    bool value;

    setEnabled(true);

    if (rep.hasAttribute(IoTivity::IOTIVITY_KITCH_RULE))
    {
        rep.getValue(IoTivity::IOTIVITY_KITCH_RULE, value);
        updateKitchMonitorState(value);
    }

    if (rep.hasAttribute(IoTivity::IOTIVITY_CRAZY_JUMPING_RULE))
    {
        rep.getValue(IoTivity::IOTIVITY_CRAZY_JUMPING_RULE, value);
        updateCrazyJumpingState(value);
    }

    if (rep.hasAttribute("density"))
    {
        int densityValue;
        rep.getValue("density", densityValue);
        updateDefineDensity(densityValue);
    }

    if (rep.hasAttribute("heartRate"))
    {
        int heartRateValue;
        rep.getValue("heartRate", heartRateValue);
        updateDefineHeartRate(heartRateValue);
    }
}

void SensorRule::sensorAdded(std::shared_ptr< OC::OCResource > sensor)
{
    if (objectName().toStdString() != sensor->uri())
        return;

    m_resource = sensor;
}

void SensorRule::sensorRemoved(std::shared_ptr< OC::OCResource > sensor)
{

}

void SensorRule::sensorChanged(const OC::OCRepresentation &sensor)
{
    RuleChangedEvent *event;

    if (sensor.getUri() != objectName().toStdString())
        return;

    event = new RuleChangedEvent(static_cast< QEvent::Type >(QEvent::User + 1));
    event->setSensorRepresentation(sensor);
    QApplication::instance()->postEvent(this, event);
}

bool SensorRule::event(QEvent *event)
{
    RuleChangedEvent *sensorEvent;

    if (event->type() != QEvent::User + 1)
        return QWidget::event(event);

    sensorEvent = dynamic_cast< RuleChangedEvent* >(event);
    updateRuleView(sensorEvent->m_rep);

    return true;
}

SensorRule::RuleChangedEvent::RuleChangedEvent(QEvent::Type type)
    : QEvent(type)
{
}

void SensorRule::RuleChangedEvent::setSensorRepresentation(const OC::OCRepresentation &rep)
{
    m_rep = rep;
}

void SensorRule::onKitchChecked(bool checkedState)
{
    bool state;
    OC::OCRepresentation rep;

    if (m_resource == nullptr)
        return;

    state = checkedState == Qt::Unchecked ? false : true;

    rep.setValue(IoTivity::IOTIVITY_KITCH_RULE, state);

    m_resource->put(rep, OC::QueryParamsMap(), IoTivity::onPut);
}

void SensorRule::onCrazyJumpingChecked(bool checkedState)
{
    bool state;
    OC::OCRepresentation rep;

    if (m_resource == nullptr)
        return;

    state = checkedState == Qt::Unchecked ? false : true;

    rep.setValue(IoTivity::IOTIVITY_CRAZY_JUMPING_RULE, state);

    m_resource->put(rep, OC::QueryParamsMap(), IoTivity::onPut);
}

void SensorRule::updateKitchMonitorState(bool value)
{
    qDebug("updateKitchMonitorState: %d", value);
    if (value == 0)
        m_kitchGas->setCheckState(Qt::Unchecked);
    else
        m_kitchGas->setCheckState(Qt::Checked);
}

void SensorRule::updateCrazyJumpingState(bool value)
{
    qDebug("updateCrazyJumpingState: %d", value);
    if (value == 0)
        m_crazyJumping->setCheckState(Qt::Unchecked);
    else
        m_crazyJumping->setCheckState(Qt::Checked);
}

void SensorRule::updateDefineDensity(int value)
{
    qDebug("updateDefineDensity: %d", value);
    m_densitySelect->setCurrentText(QString::fromStdString(std::to_string(value)));

    IoTivity::densityDefineValue = value;
}

void SensorRule::updateDefineHeartRate(int value)
{
    qDebug("updateDefineHeartRate: %d", value);
    m_heartRateSelect->setCurrentText(QString::fromStdString(std::to_string(value)));
}

void SensorRule::onDensitySelectChanged(QString value)
{
    OC::OCRepresentation rep;

    rep.setValue("density", value.toInt());

    m_resource->put(rep, OC::QueryParamsMap(), IoTivity::onPut);
}

void SensorRule::onHeartRateSelectChanged(QString value)
{
    OC::OCRepresentation rep;

    rep.setValue("heartRate", value.toInt());

    m_resource->put(rep, OC::QueryParamsMap(), IoTivity::onPut);
}
