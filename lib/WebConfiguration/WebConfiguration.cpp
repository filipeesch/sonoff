#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include "WebConfiguration.h"

WebConfiguration::WebConfiguration(ESP8266WebServer &server) : server(server)
{
}

void WebConfiguration::configure()
{
    server.on("/", [this]() {

        String body;

        if (WiFi.status() == WL_CONNECTED)
        {
            body.concat("<div>Wifi: ");
            body.concat(WiFi.SSID());
            body.concat("</div>");
        }

        body.concat("<br/>");
        body.concat("<div>");
        body.concat("<ul>");
        body.concat("<li><a href='/wifi'>Configure Wifi</a></li>");
        body.concat("<li><a href='/mqtt'>Configure MQTT</a></li>");
        body.concat("</ul>");
        body.concat("</div>");

        server.send(200, "text/html", body);
    });
}