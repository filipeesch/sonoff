#include <Arduino.h>
#include <SPI.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include "FS.h"
//#include <ESP8266mDNS.h>

#include "ConfigurationManager.h"
#include "WebConfiguration.h"

ESP8266WebServer server(80);
ConfigurationManager configManager;

WebConfiguration webConfig(server, configManager);

const char *ssid = "Esch";
const char *pwd = "wireless";

void setup()
{
    Serial.begin(9600);

    if (!SPIFFS.begin())
        Serial.println("Error on mounting FS!");

    WiFi.begin(ssid, pwd);

    Serial.print("Connecting to Wi-Fi");
    while (WiFi.status() != WL_CONNECTED)
    {
        delay(1000);
        Serial.print(".");
    }

    Serial.print("Connected to ");
    Serial.println(ssid);
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());

    // Set WiFi to station mode and disconnect from an AP if it was previously connected
    // WiFi.mode(WIFI_STA);
    // WiFi.disconnect();
    // delay(100);

    // Serial.println("Setup done");

    webConfig.configure();

    server.begin();
}

void loop()
{
    server.handleClient();

    // Serial.println("scan start");

    // // WiFi.scanNetworks will return the number of networks found
    // int n = WiFi.scanNetworks();
    // Serial.println("scan done");
    // if (n == 0)
    //     Serial.println("no networks found");
    // else
    // {
    //     Serial.print(n);
    //     Serial.println(" networks found");
    //     for (int i = 0; i < n; ++i)
    //     {
    //         // Print SSID and RSSI for each network found
    //         Serial.print(i + 1);
    //         Serial.print(": ");
    //         Serial.print(WiFi.SSID(i));
    //         Serial.print(" (");
    //         Serial.print(WiFi.RSSI(i));
    //         Serial.print(")");
    //         Serial.println((WiFi.encryptionType(i) == ENC_TYPE_NONE) ? " " : "*");
    //         delay(10);
    //     }
    // }
    // Serial.println("");

    // // Wait a bit before scanning again
    // delay(5000);
}