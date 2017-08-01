#include <Arduino.h>
#include <SPI.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <FS.h>
#include <string.h>
#include <Bounce2.h>
//#include <ESP8266mDNS.h>

#include <PubSubClient.h>

#include "ConfigurationManager.h"
#include "WebConfiguration.h"
#include "Timer.h"

#define RELAY_PIN 12
#define STATUS_LED_PIN 13

ESP8266WebServer server(80);
ConfigurationManager configManager;

WiFiConfiguration wifiConfig;
MqttConfiguration mqttConfig;

WiFiClient wifiClient;
PubSubClient mqttClient(wifiClient);

WebConfiguration webConfig(server, configManager);

Timer mqttConnectedTimer(5000);
Timer wifiConnectedTimer(1000);
Timer wifiReconnectedTimer(30 * 1000);
Timer apModeTimer(5 * 60 * 1000);

bool apModeOn = false, mqttEnabled = false;

Bounce pin1, pin3, pin14;
bool pin1Enabled = false, pin3Enabled = false, pin14Enabled = false;

void enableAPMode()
{
    if (apModeOn)
        return;

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
    Serial.println(wifiConfig.ssid);

    if (WiFi.waitForConnectResult() != WL_CONNECTED)
    {
        Serial.println("Wi-Fi Timeout");
        return false;
    }

    WiFi.softAPdisconnect(true);
    apModeOn = false;

    Serial.println("\nWi-Fi Connected!");
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

        digitalWrite(STATUS_LED_PIN, !digitalRead(RELAY_PIN));

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

            mqttClient.subscribe(mqttConfig.relayTopic.c_str());
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
        if (mqttConfig.pin1Topic != "")
        {
            pinMode(1, INPUT_PULLUP);
            pin1Enabled = true;
            pin1.interval(200);
            pin1.attach(1);
        }

        if (mqttConfig.pin3Topic != "")
        {
            pinMode(3, INPUT_PULLUP);
            pin3Enabled = true;
            pin3.interval(200);
            pin3.attach(3);
        }
    }

    if (mqttEnabled)
        configureMqtt();

    if (mqttConfig.pin14Topic != "")
    {
        pinMode(14, INPUT_PULLUP);
        pin14.interval(200);
        pin14.attach(14);
        pin14Enabled = true;
    }

    pinMode(RELAY_PIN, OUTPUT);
    pinMode(STATUS_LED_PIN, OUTPUT);

    digitalWrite(STATUS_LED_PIN, !digitalRead(RELAY_PIN));

    if (connectWifi())
        connectMqtt();

    webConfig.configure();

    server.begin();

    apModeTimer.reset();
}

void loop()
{
    if (WiFi.status() == WL_CONNECTED)
    {
        if (mqttConnectedTimer.ready())
            connectMqtt();

        if (pin1Enabled && pin1.update())
            mqttClient.publish(mqttConfig.pin1Topic.c_str(), (const uint8_t *)"-1", 2, false);

        if (pin3Enabled && pin3.update())
            mqttClient.publish(mqttConfig.pin3Topic.c_str(), (const uint8_t *)"-1", 2, false);

        if (pin14Enabled && pin14.update())
            mqttClient.publish(mqttConfig.pin14Topic.c_str(), (const uint8_t *)"-1", 2, false);

        mqttClient.loop();
        apModeTimer.reset();
    }
    else
    {
        if (wifiReconnectedTimer.ready())
        {
            if (connectWifi())
                connectMqtt();
        }

        if (apModeTimer.ready())
            enableAPMode();
    }

    server.handleClient();
}