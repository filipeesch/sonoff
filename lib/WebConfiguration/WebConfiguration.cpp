#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include "WebConfiguration.h"

WebConfiguration::WebConfiguration(ESP8266WebServer &server) : server(server)
{
}

void WebConfiguration::configure()
{
    homePage();
    wifiPage();
    mqttPage();
    rebootPage();
}

void WebConfiguration::masterPage(String &body)
{

    body.concat("<head>");

    body.concat("<meta name='viewport' content='width=device-width, initial-scale=1.0, maximum-scale=1.0, user-scalable=no' />");

    body.concat("<style>");
    body.concat("ul{list-style-type: none;}");
    body.concat("</style>");

    body.concat("</head>");

    if (WiFi.status() == WL_CONNECTED)
    {
        body.concat("<div><h2>Conneted Wifi: ");
        body.concat(WiFi.SSID());
        body.concat("</h2></div>");
    }
}

void WebConfiguration::homePage()
{
    server.on("/", [this]() {

        String body;

        masterPage(body);

        body.concat("<br/>");
        body.concat("<div>");
        body.concat("<ul>");
        body.concat("<li><a href='/wifi'>Configure Wifi</a></li>");
        body.concat("<li><a href='/mqtt'>Configure MQTT</a></li>");
        body.concat("<li><a href='/reboot'>Reboot</a></li>");
        body.concat("</ul>");
        body.concat("</div>");

        server.send(200, "text/html", body);
    });
}

void WebConfiguration::wifiPage()
{
    server.on("/wifi", [this]() {

        String body;
        wchar_t buffer[100];

        masterPage(body);

        int n = WiFi.scanNetworks();

        body.concat("<script>function ssid(evt,id){evt.preventDefault();document.getElementsByName('ssid')[0].value = id}</script>");

        body.concat("<br/><div><label>Select Wifi:</label><ul>");

        for (int i = 0; i < n; ++i)
        {
            int signal = WiFi.RSSI(i);

            if (signal <= -100)
                signal = 0;
            else if (signal >= -50)
                signal = 100;
            else
                signal = 2 * (signal + 100);

            String ssid = WiFi.SSID(i);

            body.concat("<li><a href='#' onclick='ssid(event,\"");
            body.concat(ssid);
            body.concat("\")'>");
            body.concat(ssid);
            body.concat(" - ");
            body.concat(signal);
            body.concat("%</a></li>");
        }

        body.concat("</ul></div>");

        body.concat("<form action='/wifi/save' method='POST'>");
        body.concat("<div>SSID: <input type='text' name='ssid'/></div>");
        body.concat("<div>Password: <input type='text' name='password'/></div>");
        body.concat("<input type='submit' value='Save'/>");
        body.concat("</form>");

        server.send(200, "text/html", body);
    });
}

void WebConfiguration::mqttPage()
{
}

void WebConfiguration::rebootPage()
{
}