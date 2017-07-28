class WebConfiguration
{
  private:
    ESP8266WebServer &server;

    void masterPage(String &);
    void homePage();
    void wifiPage();
    void saveWifiPage();
    void mqttPage();
    void rebootPage();

  public:
    void configure();

    WebConfiguration(ESP8266WebServer &);
};