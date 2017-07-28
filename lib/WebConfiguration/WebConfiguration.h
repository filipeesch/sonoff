class WebConfiguration
{
  private:
    ESP8266WebServer &server;

  public:
    void configure();

    WebConfiguration(ESP8266WebServer &);
};