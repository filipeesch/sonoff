#include <FS.h>
#include <spi_flash.h>

#include "ConfigurationManager.h"

void ConfigurationManager::clearConfig()
{
    SPIFFS.remove(settingsConfigFile);
    SPIFFS.remove(mqttConfigFile);
    SPIFFS.remove(wifiConfigFile);
}

void ConfigurationManager::setSettings(SettingsConfiguration &config)
{
    auto file = SPIFFS.open(settingsConfigFile, "w+");

    file.print(config.updateServerUrl);
    file.print("\n");

    file.close();
}

bool ConfigurationManager::getSettings(SettingsConfiguration &config)
{
    auto f = SPIFFS.open(settingsConfigFile, "r");

    if (!f)
        return false;

    config.updateServerUrl = f.readStringUntil('\n');

    f.close();

    return true;
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
    file.print(config.relayStatusTopic);
    file.print("\n");
    file.print(config.boardStatusTopic);
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
    config.relayStatusTopic = f.readStringUntil('\n');
    config.boardStatusTopic = f.readStringUntil('\n');
    config.pin1Topic = f.readStringUntil('\n');
    config.pin3Topic = f.readStringUntil('\n');
    config.pin14Topic = f.readStringUntil('\n');

    f.close();

    return true;
}
