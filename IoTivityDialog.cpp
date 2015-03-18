#include <QApplication>
#include <QStyleFactory>
#include <QLabel>
#include <QGroupBox>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QFormLayout>
#include <QCheckBox>

#include "SensorCell.h"
#include "SensorRule.h"
#include "IoTivity.h"
#include "IoTivityClient.h"
#include "IoTivityDialog.h"

IoTivityDialog::IoTivityDialog(QWidget *parent)
    : QDialog(parent)
{
    this->resize(IoTivity::DEMO_WIDTH, IoTivity::DEMO_HEIGHT);

    m_title = new QLabel(tr("IoTivity on Tizen Demo"), this);
    m_title->setAlignment(Qt::AlignCenter);

    QFont title_font;
    title_font = m_title->font();
    title_font.setPointSize(IoTivity::TITLE_FONT_SIZE);
    title_font.setBold(true);
    title_font.setKerning(true);
    m_title->setFont(title_font);

    m_sensorsGroupBox = createSensorsGroupBox();
    m_ruleGroupBox = createRuleGroupBox();

    m_mainLayout = new QVBoxLayout(this);
    m_mainLayout->addWidget(m_title);
    m_mainLayout->addWidget(m_sensorsGroupBox);
    m_mainLayout->addWidget(m_ruleGroupBox);
    setLayout(m_mainLayout);

    m_mainLayout->setStretch(1, 3);
    m_mainLayout->setStretch(2, 1);

    /* IoTivityClient is a singleton, lifecycle is in whole program.
     * So don't free it.
     * m_pIoTivityClient = std::make_shared< IoTivity::IoTivityClient >();
     */
    m_pIoTivityClient = &IoTivity::IoTivityClient::Instance();

    /* TODO: Merge Added/Removed/Changed interface to one Interface */
    m_pIoTivityClient->addSensorAddedListener(*dynamic_cast< SensorAddedListener* >(m_fanCell));
    m_pIoTivityClient->addSensorRemovedListener(*dynamic_cast< SensorRemovedListener* >(m_fanCell));
    m_pIoTivityClient->addSensorChangedListener(*dynamic_cast< SensorChangedListener* >(m_fanCell));

    m_pIoTivityClient->addSensorAddedListener(*dynamic_cast< SensorAddedListener* >(m_heartRateCell));
    m_pIoTivityClient->addSensorRemovedListener(*dynamic_cast< SensorRemovedListener* >(m_heartRateCell));
    m_pIoTivityClient->addSensorChangedListener(*dynamic_cast< SensorChangedListener* >(m_heartRateCell));

    m_pIoTivityClient->addSensorAddedListener(*dynamic_cast< SensorAddedListener* >(m_gasCell));
    m_pIoTivityClient->addSensorRemovedListener(*dynamic_cast< SensorRemovedListener* >(m_gasCell));
    m_pIoTivityClient->addSensorChangedListener(*dynamic_cast< SensorChangedListener* >(m_gasCell));

    m_pIoTivityClient->addSensorAddedListener(*dynamic_cast< SensorAddedListener* >(m_ledCell));
    m_pIoTivityClient->addSensorRemovedListener(*dynamic_cast< SensorRemovedListener* >(m_ledCell));
    m_pIoTivityClient->addSensorChangedListener(*dynamic_cast< SensorChangedListener* >(m_ledCell));

    m_pIoTivityClient->addSensorAddedListener(*dynamic_cast< SensorAddedListener* >(m_ruleView));
    m_pIoTivityClient->addSensorRemovedListener(*dynamic_cast< SensorRemovedListener* >(m_ruleView));
    m_pIoTivityClient->addSensorChangedListener(*dynamic_cast< SensorChangedListener* >(m_ruleView));

    /* Set style to 'Fusion', option style: 'Windows', 'GTK+' */
    changeStyle("Fusion");

    /* Use multi-case IP address to find gw.sensor */
    m_pIoTivityClient->findResource("224.0.1.187", "gw.sensor");
    m_pIoTivityClient->findResource("224.0.1.187", "gw.rule");
}

IoTivityDialog::~IoTivityDialog()
{

}

void IoTivityDialog::changeStyle(const QString &styleName)
{
    QApplication::setStyle(QStyleFactory::create(styleName));
}

QGroupBox *IoTivityDialog::createSensorsGroupBox()
{
    QGroupBox *groupBox;
    QGridLayout *groupBoxLayout;

    groupBox = new QGroupBox(this);

    m_fanCell = new SensorCell(groupBox);
    m_fanCell->setObjectName(QString::fromStdString(IoTivity::IOTIVITY_FAN));

    m_heartRateCell = new SensorCell(groupBox);
    m_heartRateCell->setObjectName(QString::fromStdString(IoTivity::IOTIVITY_HEARTRATE));

    m_gasCell = new SensorCell(groupBox);
    m_gasCell->setObjectName(QString::fromStdString(IoTivity::IOTIVITY_GAS));

    m_ledCell = new SensorCell(groupBox);
    m_ledCell->setObjectName(QString::fromStdString(IoTivity::IOTIVITY_LED));

    groupBoxLayout = new QGridLayout(this);
    groupBoxLayout->addWidget(m_heartRateCell, 0, 0, 1, 1, Qt::AlignCenter);
    groupBoxLayout->addWidget(m_gasCell, 0, 1, 1, 1, Qt::AlignCenter);
    groupBoxLayout->addWidget(m_ledCell, 1, 0, 1, 1, Qt::AlignCenter);
    groupBoxLayout->addWidget(m_fanCell, 1, 1, 1, 1, Qt::AlignCenter);

    groupBox->setLayout(groupBoxLayout);

    return groupBox;
}

QGroupBox *IoTivityDialog::createRuleGroupBox()
{
    QGroupBox *groupBox;
    QHBoxLayout *layout;

    groupBox = new QGroupBox(this);

    layout = new QHBoxLayout(groupBox);

    m_ruleView = new SensorRule(groupBox);
    m_ruleView->setObjectName(tr("/gw/rul"));

    layout->addWidget(m_ruleView);
    layout->setAlignment(Qt::AlignCenter);

    groupBox->setLayout(layout);

    return groupBox;
}
