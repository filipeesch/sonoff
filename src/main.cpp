#include <Arduino.h>
#include <SPI.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <FS.h>
#include <string.h>
//#include <ESP8266mDNS.h>

#include <PubSubClient.h>

#include "ConfigurationManager.h"
#include "WebConfiguration.h"
#include "Timer.h"

#define RELAY_PIN 12

ESP8266WebServer server(80);
ConfigurationManager configManager;

WiFiConfiguration wifiConfig;
MqttConfiguration mqttConfig;

WiFiClient wifiClient;
PubSubClient mqttClient(wifiClient);

WebConfiguration webConfig(server, configManager);

Timer mqttConnectedTimer(5000);
Timer wifiConnectedTimer(1000);
Timer apModeTimer(5 * 60 * 1000);

bool apModeOn = false, mqttEnabled = false;

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

void mqtt_callback(const char *topic, byte *payload, unsigned int length)
{
    Serial.print("Received MQTT: ");

    auto data = new char[length + 1];
    strncpy(data, (const char *)payload, length);
    data[length] = '\0';

    Serial.println(data);

    if (mqttConfig.relayTopic == topic)
    {
        if (strcmp(data, "1") == 0)
            digitalWrite(RELAY_PIN, 1);
        else if (strcmp(data, "0") == 0)
            digitalWrite(RELAY_PIN, 0);
        else
            digitalWrite(RELAY_PIN, !digitalRead(RELAY_PIN));

        if (mqttConfig.relayStatusTopic != "")
        {
            auto status = String(digitalRead(RELAY_PIN));
            mqttClient.publish(mqttConfig.relayStatusTopic.c_str(), (uint8_t *)status.c_str(), status.length(), true);
        }
    }
}

void configureMqtt()
{
    mqttClient.setServer(mqttConfig.host.c_str(), mqttConfig.port);
    mqttClient.setCallback(mqtt_callback);
}

void connectMqtt()
{
    if (mqttEnabled && !mqttClient.connected())
    {
        bool connected;

        if (mqttConfig.user != "" && mqttConfig.password != "")
            connected = mqttClient.connect(mqttConfig.name.c_str(), mqttConfig.user.c_str(), mqttConfig.password.c_str());
        else
            connected = mqttClient.connect(mqttConfig.name.c_str());

        if (connected)
        {
            Serial.println("MQTT Connected!");

            if (mqttConfig.boardStatusTopic != "")
                mqttClient.publish(mqttConfig.boardStatusTopic.c_str(), (uint8_t *)mqttConfig.name.c_str(), mqttConfig.name.length(), true);

            Serial.println(mqttClient.subscribe(mqttConfig.relayTopic.c_str()));
        }
    }
}

void setup()
{
    SPIFFS.begin();

    mqttEnabled = configManager.getMqtt(mqttConfig);

    if (mqttConfig.pin1Topic == "" && mqttConfig.pin3Topic == "")
    {
        Serial.begin(9600);
        Serial.println("Serial enabled!");
    }
    else
    {
        pinMode(1, INPUT);
        pinMode(3, INPUT);
        pinMode(14, INPUT);
    }

    if (mqttEnabled)
        configureMqtt();

    pinMode(RELAY_PIN, OUTPUT);

    connectWifi();

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

    if (mqttConnectedTimer.ready())
        connectMqtt();

    mqttClient.loop();
    server.handleClient();
}