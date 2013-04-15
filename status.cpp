#include <QTextStream>

#include "stdio.h"
#include "status.h"

Status::Status(QWidget *parent) :
    QWidget(parent)
{
    wifiLabel = new QLabel;
    wifiLabel->setText("Wifi:");
    storageLabel = new QLabel;
    storageLabel->setText("Storage:");
    batteryLabel = new QLabel;
    batteryLabel->setText("Battery:");
    deviceLabel = new QLabel;
    deviceLabel->setText("Devices:");
    wifiText = new QLabel;
    storageText = new QLabel;
    batteryText = new QLabel;
    deviceText = new QLabel;

    update();

    gridLayout = new QGridLayout;
    gridLayout->setHorizontalSpacing(25);
    gridLayout->setContentsMargins(0, 0, 0, 0);
    gridLayout->setAlignment(Qt::AlignLeft);

    gridLayout->addWidget(wifiLabel, 0, 0);
    gridLayout->addWidget(storageLabel, 1, 0);
    gridLayout->addWidget(batteryLabel, 2, 0);
    gridLayout->addWidget(deviceLabel, 3, 0);
    gridLayout->addWidget(wifiText, 0, 1);
    gridLayout->addWidget(storageText, 1, 1);
    gridLayout->addWidget(batteryText, 2, 1);
    gridLayout->addWidget(deviceText, 3, 1);
    setLayout(gridLayout);
}

Status::~Status()
{
}

void Status::update()
{
    updateCmd("iwconfig wlan0 | grep 'Link Quality' | cut -d '=' -f 2 | cut -d ' ' -f 1", wifiText);
    updateCmd("df -h | egrep '/$' | awk '{print $5}'", storageText);
    updateCmd("acpi | awk '{print $4}'", batteryText);
    updateCmd("lsusb | cut -d ' ' -f 7-", deviceText);
}

void Status::updateCmd(const char* cmd, QLabel *result)
{
    FILE *handler = popen(cmd, "r");
    QTextStream stream(handler, QIODevice::ReadOnly);
    QString text = stream.readAll();
    result->setText(text);    
    pclose(handler);
}

