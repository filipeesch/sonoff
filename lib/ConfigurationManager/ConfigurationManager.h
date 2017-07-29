#ifndef CONFIGURATION_MANAGER_H
#define CONFIGURATION_MANAGER_H

struct WiFiConfiguration
{
    String ssid;
    String password;
};

class ConfigurationManager
{
  private:
    const char *wifiConfigFile = "wifi.txt";

  public:
    void setWiFi(WiFiConfiguration &);
    bool getWiFi(WiFiConfiguration &);
};

#endif