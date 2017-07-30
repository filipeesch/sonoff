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
    saveMqttPage();
    rebootPage();
    factoryResetPage();
}

void WebConfiguration::createMasterPage(HtmlBuilder &html, RenderPageHandler renderPage)
{
    auto head = html.head([](HtmlBuilder *head) {
        head->raw("<meta name='viewport' content='width=device-width, initial-scale=1.3, maximum-scale=1.3, user-scalable=no'/>");
        head->raw("<style>ul{list-style-type:none;padding:0;margin:0;}</style>");
    });

    auto body = html.body([](HtmlBuilder *body) {
        body->a("/", "Home");

        body->div()->text("Free Memory: " + String(ESP.getFreeHeap(), 10));

        if (WiFi.status() == WL_CONNECTED)
            body->div()->text("Connected Wi-Fi: " + WiFi.SSID());

        body->br();
    });

    renderPage(head, body);
}

void WebConfiguration::homePage()
{
    server.on("/", [this]() {

        HtmlTag htmlTag;
        HtmlBuilder html(&htmlTag);

        createMasterPage(html, [](HtmlBuilder *head, HtmlBuilder *body) {
            body->ul([](HtmlBuilder *ul) {
                ul->li()->a("/wifi", "Configure Wi-Fi");
                ul->li()->a("/mqtt", "Configure MQTT");
                ul->li()->a("/reboot", [](HtmlBuilder *a) {
                    a->text("Reboot");
                    a->attr("onclick", "return confirm('Really want to Reboot?')");
                });
                ul->li()->a("/factory-reset", [](HtmlBuilder *a) {
                    a->text("Factory Reset");
                    a->attr("onclick", "return confirm('Really want to Factory Reset?')");
                });
            });
        });

        server.send(200, "text/html", html.build());
    });
}

void WebConfiguration::wifiPage()
{
    server.on("/wifi", [this]() {

        HtmlTag htmlTag;
        HtmlBuilder html(&htmlTag);

        createMasterPage(html, [](HtmlBuilder *head, HtmlBuilder *body) {

            head->raw("<script>function ssid(id){document.getElementsByName('ssid')[0].value = id}</script>");

            body->div([](HtmlBuilder *div) {
                div->label("Select Wi-Fi:");

                div->ul([](HtmlBuilder *ul) {

                    int n = WiFi.scanNetworks();

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

                        auto networkDescription = ssid + " - (" + String(signal, 10) + ")%";

                        ul->li()->a("#pwd", networkDescription)->attr("onclick", "ssid('" + ssid + "')");
                    }
                });

                div->br();

                div->form("/wifi-save", "POST", [](HtmlBuilder *form) {

                    form->div()->label("SSID:");
                    form->div()->input("text", "ssid");

                    form->div()->label("Password:");
                    form->div()->input("password", "password")->attr("id", "pwd");

                    form->br();
                    form->div()->input("submit", "submit", "Save");
                });
            });
        });

        server.send(200, "text/html", html.build());
    });
}

void WebConfiguration::saveWifiPage()
{
    server.on("/wifi-save", [this]() {

        WiFiConfiguration config;

        config.ssid = server.arg("ssid");
        config.password = server.arg("password");

        configManager.setWiFi(config);

        HtmlTag htmlTag;
        HtmlBuilder html(&htmlTag);

        createMasterPage(html, [](HtmlBuilder *head, HtmlBuilder *body) {
            body->div()->text("Wi-Fi settings saved.");
        });

        server.send(200, "text/html", html.build());
    });
}

void WebConfiguration::mqttPage()
{
    server.on("/mqtt", [this]() {

        HtmlTag htmlTag;
        HtmlBuilder html(&htmlTag);

        createMasterPage(html, [](HtmlBuilder *head, HtmlBuilder *body) {

            body->form("/mqtt-save", "POST", [](HtmlBuilder *form) {

                form->div()->label("Board Name:");
                form->div()->input("text", "name")->attr("required");

                form->div()->label("MQTT Host:");
                form->div()->input("text", "host")->attr("required");

                form->div()->label("MQTT Port:");
                form->div()->input("number", "port", "1883")->attr("required");

                form->div()->label("MQTT User:");
                form->div()->input("text", "user");

                form->div()->label("MQTT Password:");
                form->div()->input("password", "password");

                form->div()->label("Relay Topic:");
                form->div()->input("text", "relayTopic")->attr("required");

                form->div()->label("Pin 1 Topic:");
                form->div()->input("text", "pin1Topic");

                form->div()->label("Pin 3 Topic:");
                form->div()->input("text", "pin3Topic");

                form->div()->label("Pin 14 Topic:");
                form->div()->input("text", "pin14Topic");

                form->br();
                form->div()->input("submit", "submit", "Save");
            });
        });

        server.send(200, "text/html", html.build());
    });
}

void WebConfiguration::saveMqttPage()
{
    server.on("/wifi-save", [this]() {

        MqttConfiguration config;

        config.name = server.arg("name");
        config.host = server.arg("host");
        config.port = server.arg("port").toInt();
        config.user = server.arg("user");
        config.password = server.arg("password");
        config.relayTopic = server.arg("relayTopic");
        config.pin1Topic = server.arg("pin1Topic");
        config.pin3Topic = server.arg("pin3Topic");
        config.pin14Topic = server.arg("pin14Topic");

        configManager.setMqtt(config);

        HtmlTag htmlTag;
        HtmlBuilder html(&htmlTag);

        createMasterPage(html, [](HtmlBuilder *head, HtmlBuilder *body) {
            body->div()->text("MQTT settings saved.");
        });

        server.send(200, "text/html", html.build());
    });
}

void WebConfiguration::rebootPage()
{
    server.on("/reboot", [this]() {

        HtmlTag htmlTag;
        HtmlBuilder html(&htmlTag);

        createMasterPage(html, [](HtmlBuilder *head, HtmlBuilder *body) {
            body->div()->text("Rebooting...");
        });

        server.send(200, "text/html", html.build());

        ESP.restart();
    });
}

void WebConfiguration::factoryResetPage()
{
    server.on("/factory-reset", [this]() {

        HtmlTag htmlTag;
        HtmlBuilder html(&htmlTag);

        createMasterPage(html, [](HtmlBuilder *head, HtmlBuilder *body) {
            body->div()->text("Resetting...");
        });

        server.send(200, "text/html", html.build());

        configManager.clearConfig();
        ESP.restart();
    });
}