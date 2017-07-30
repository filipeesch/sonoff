#include <Arduino.h>
#include <SPI.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include "FS.h"
//#include <ESP8266mDNS.h>

#include "ConfigurationManager.h"
#include "WebConfiguration.h"
#include "Timer.h"

ESP8266WebServer server(80);
ConfigurationManager configManager;

WiFiConfiguration wifiConfig;
MqttConfiguration mqttConfig;

WebConfiguration webConfig(server, configManager);

Timer wifiConnectedTimer(1000);
Timer apModeTimer(5 * 60 * 1000);

bool apModeOn = false;

void enableAPMode()
{
    Serial.println("Starting AP mode");

    IPAddress ip(192, 168, 1, 1);
    IPAddress mask(255, 255, 255, 0);

    WiFi.softAPConfig(ip, ip, mask);

    auto ssid = "ESP_" + String(ESP.getChipId(), 10);
    apModeOn = WiFi.softAP(ssid.c_str());
}

bool connectWifi()
{
    if (!configManager.getWiFi(wifiConfig))
    {
        enableAPMode();
        return false;
    }

    WiFi.begin(wifiConfig.ssid.c_str(), wifiConfig.password.c_str());

    Serial.print("\n\nConnecting to Wi-Fi: ");
    Serial.print(wifiConfig.ssid);

    int timeout = 0;

    while (WiFi.status() != WL_CONNECTED)
    {
        delay(1000);
        Serial.print(".");
        if (timeout++ > 10)
        {
            Serial.println("Wi-Fi Timeout");
            return false;
        }
    }

    Serial.print("\nConnected to ");
    Serial.println(wifiConfig.ssid);
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());

    return true;
}

void setup()
{
    Serial.begin(9600);

    if (!SPIFFS.begin())
        Serial.println("Error on mounting FS!");

    connectWifi();

    if (configManager.getMqtt(mqttConfig))
    {
    }

    webConfig.configure();

    server.begin();

    apModeTimer.reset();
}

void loop()
{
    if (wifiConnectedTimer.ready())
    {
        if (WiFi.status() == WL_CONNECTED)
        {
            apModeTimer.reset();

            if (apModeOn)
                apModeOn = !WiFi.softAPdisconnect(true);
        }
    }

    if (apModeTimer.ready())
    {
        if (WiFi.status() != WL_CONNECTED)
        {
            if (!apModeOn)
                enableAPMode();
        }
    }

    server.handleClient();
}