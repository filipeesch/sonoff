#ifndef WEB_CONFIGURATION_H
#define WEB_CONFIGURATION_H

#include "HtmlBuilder.h"
#include "ConfigurationManager.h"

class WebConfiguration
{
  private:
    ESP8266WebServer &server;
    ConfigurationManager &configManager;

    void masterPage(String &);
    void homePage();
    void wifiPage();
    void saveWifiPage();
    void mqttPage();
    void rebootPage();

  public:
    void configure();

    WebConfiguration(ESP8266WebServer &, ConfigurationManager &);
};

#endif