#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include "FS.h"

#include "WebConfiguration.h"
#include "ConfigurationManager.h"

WebConfiguration::WebConfiguration(
    ESP8266WebServer &server,
    ConfigurationManager &configManager) : server(server), configManager(configManager)
{
}

void WebConfiguration::configure()
{
    homePage();
    wifiPage();
    saveWifiPage();
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

    body.concat("<a href='/'>Home</a>");

    HtmlDivElement div;
    div.name("testeName");
    div.id("testeId");

    div.append(new HtmlInputElement("text", "testeName", String(ESP.getFreeHeap(), 10)));
    div.append(new HtmlInputElement("text", "testeName2", "testeValue"));

    div.build(body);

    if (WiFi.status() == WL_CONNECTED)
    {
        body.concat("<div>Conneted Wifi: ");
        body.concat(WiFi.SSID());
        body.concat("</div>");
    }
}

void WebConfiguration::homePage()
{
    server.on("/", [this]() {

        // auto html = new HtmlTag();
        // auto head = new HtmlHeadElement();
        // auto body = new HtmlBodyElement();

        // html->append(head)->append(body);

        // //masterPage(head, body);

        // head->append(new HtmlRaw("<meta name='viewport' content='width=device-width, initial-scale=1.0, maximum-scale=1.0, user-scalable=no' />"));
        // head->append(new HtmlRaw("<style>ul{list-style-type: none;}</style>"));

        // body->append((new HtmlLinkElement("/"))->append(new HtmlText("Home")));

        // if (WiFi.status() == WL_CONNECTED)
        //     body->append((new HtmlDivElement())->append(new HtmlText("Connected Wi-Fi: " + WiFi.SSID())));

        // /////////////////////////////

        // body->append(new HtmlBrElement());

        // auto ul = new HtmlUlElement();

        // auto confirmReboot = new HtmlAttribute("onclick", "return confirm('Realy want to Reboot?')");

        // ul
        //     ->append((new HtmlLiElement())->append(new HtmlLinkElement("/wifi", "Configure Wi-Fi")))
        //     ->append((new HtmlLiElement())->append(new HtmlLinkElement("/mqtt", "Configure MQTT")))
        //     ->append((new HtmlLiElement())->append((new HtmlLinkElement("/reboot", "Reboot"))->appendAttr(confirmReboot)));

        // body->append((new HtmlDivElement())->append(ul));

        // server.send(200, "text/html", html->buildAll());

        // delete html;

        HtmlTag htmlTag;
        HtmlBuilder html(&htmlTag);

        html.head([](HtmlBuilder *head) {
            head->raw("<meta name='viewport' content='width=device-width, initial-scale=1.0, maximum-scale=1.0, user-scalable=no'/>");
            head->raw("<style>ul{list-style-type:none;padding:0;}</style>");
        });

        html.body([](HtmlBuilder *body) {
            body->a("/", "Home");

            body->div()->text("Free Memory: " + String(ESP.getFreeHeap(), 10));

            if (WiFi.status() == WL_CONNECTED)
                body->div()->text("Connected Wi-Fi: " + WiFi.SSID());

            body->ul([](HtmlBuilder *ul) {
                ul->li()->a("/wifi", "Configure Wi-Fi");
                ul->li()->a("/mqtt", "Configure MQTT");
                ul->li()->a("/reboot", [](HtmlBuilder *a) {
                    a->text("Reboot");
                    a->attr("onclick", "return confirm('Really want to Reboot?')");
                });
            });
        });

        server.send(200, "text/html", html.build());
    });
}

void WebConfiguration::wifiPage()
{
    server.on("/wifi", [this]() {

        String body;
        body.reserve(3500);

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

            auto ssid = WiFi.SSID(i);

            body.concat("<li><a href='#' onclick='ssid(event,\"");
            body.concat(ssid);
            body.concat("\")'>");
            body.concat(ssid);
            body.concat(" - ");
            body.concat(signal);
            body.concat("%</a></li>");
        }

        body.concat("</ul></div>");

        body.concat("<form action='/wifi-save' method='POST'>");
        body.concat("<div>SSID: <input type='text' name='ssid'/></div>");
        body.concat("<div>Password: <input type='text' name='password'/></div>");
        body.concat("<input type='submit' value='Save'/>");
        body.concat("</form>");

        server.send(200, "text/html", body);
    });
}

void WebConfiguration::saveWifiPage()
{
    server.on("/wifi-save", [this]() {

        WiFiConfiguration config;

        config.ssid = server.arg("ssid");
        config.password = server.arg("password");

        configManager.setWiFi(config);

        String body;
        body.reserve(1000);

        masterPage(body);

        body.concat("<div>Wifi settings saved.</div>");
        body.concat("<div>SSID: ");
        body.concat(config.ssid);
        body.concat("</div>");

        server.send(200, "text/html", body);
    });
}

void WebConfiguration::mqttPage()
{
}

void WebConfiguration::rebootPage()
{
    server.on("/reboot", [this]() {

        String body;
        body.reserve(1000);

        masterPage(body);

        body.concat("<div>Rebooting...</div>");

        server.send(200, "text/html", body);

        ESP.restart();
    });
}