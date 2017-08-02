#ifndef CONFIGURATION_MANAGER_H
#define CONFIGURATION_MANAGER_H

struct SettingsConfiguration
{
    String updateServerUrl;
};

struct MqttConfiguration
{
    String name;
    String host;
    int port = 0;
    String user;
    String password;
    String relayTopic;
    String relayStatusTopic;
    String boardStatusTopic;
    String pin1Topic;
    String pin3Topic;
    String pin14Topic;
};

struct WiFiConfiguration
{
    String ssid;
    String password;
};

class ConfigurationManager
{
  private:
    const char *settingsConfigFile = "settings";
    const char *wifiConfigFile = "wifi.txt";
    const char *mqttConfigFile = "mqtt.txt";

  public:
    void clearConfig();

    void setSettings(SettingsConfiguration &);
    bool getSettings(SettingsConfiguration &);

    void setWiFi(WiFiConfiguration &);
    bool getWiFi(WiFiConfiguration &);

    void setMqtt(MqttConfiguration &);
    bool getMqtt(MqttConfiguration &);
};

#endif