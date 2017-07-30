#include <FS.h>
#include <spi_flash.h>

#include "ConfigurationManager.h"

void ConfigurationManager::clearConfig()
{
    Serial.println("Removind files");
    Serial.println(SPIFFS.remove(mqttConfigFile));
    Serial.println(SPIFFS.remove(wifiConfigFile));
    //spi_flash_erase_sector(0x7E); //clear stored wifi settings
}

void ConfigurationManager::setWiFi(WiFiConfiguration &config)
{
    auto file = SPIFFS.open(wifiConfigFile, "w+");

    file.print(config.ssid);
    file.print("\n");
    file.print(config.password);
    file.print("\n");

    file.close();
}

bool ConfigurationManager::getWiFi(WiFiConfiguration &config)
{
    auto f = SPIFFS.open(wifiConfigFile, "r");

    if (!f)
        return false;

    config.ssid = f.readStringUntil('\n');
    config.password = f.readStringUntil('\n');

    f.close();

    return true;
}

void ConfigurationManager::setMqtt(MqttConfiguration &config)
{
    auto file = SPIFFS.open(mqttConfigFile, "w+");

    file.print(config.name);
    file.print("\n");
    file.print(config.host);
    file.print("\n");
    file.print(config.port);
    file.print("\n");
    file.print(config.user);
    file.print("\n");
    file.print(config.password);
    file.print("\n");
    file.print(config.relayTopic);
    file.print("\n");
    file.print(config.pin1Topic);
    file.print("\n");
    file.print(config.pin3Topic);
    file.print("\n");
    file.print(config.pin14Topic);
    file.print("\n");

    file.close();
}

bool ConfigurationManager::getMqtt(MqttConfiguration &config)
{
    auto f = SPIFFS.open(mqttConfigFile, "r");

    if (!f)
        return false;

    config.name = f.readStringUntil('\n');
    config.host = f.readStringUntil('\n');
    config.port = f.readStringUntil('\n').toInt();
    config.user = f.readStringUntil('\n');
    config.password = f.readStringUntil('\n');
    config.relayTopic = f.readStringUntil('\n');
    config.pin1Topic = f.readStringUntil('\n');
    config.pin3Topic = f.readStringUntil('\n');
    config.pin14Topic = f.readStringUntil('\n');

    f.close();

    return true;
}
