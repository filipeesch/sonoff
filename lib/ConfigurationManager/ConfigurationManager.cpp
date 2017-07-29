#include <FS.h>

#include "ConfigurationManager.h"

void ConfigurationManager::setWiFi(WiFiConfiguration &config)
{
    auto file = SPIFFS.open(wifiConfigFile, "w+");

    file.println(config.ssid);
    file.println(config.password);

    file.close();
}

bool ConfigurationManager::getWiFi(WiFiConfiguration &config)
{
    auto f = SPIFFS.open(wifiConfigFile, "r");

    config.ssid = f.readStringUntil('\n');
    config.password = f.readStringUntil('\n');

    f.close();
}
