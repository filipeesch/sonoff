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

void WebConfiguration::createMasterPage(HtmlBuilder &html, RenderPageHandler renderPage)
{
    auto head = html.head([](HtmlBuilder *head) {
        head->raw("<meta name='viewport' content='width=device-width, initial-scale=1.0, maximum-scale=1.0, user-scalable=no'/>");
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

            head->raw("<script>function ssid(evt,id){evt.preventDefault();document.getElementsByName('ssid')[0].value = id}</script>");

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

                        ul->li()->a("#", networkDescription)->attr("onclick", "ssid(event,'" + ssid + "')");
                    }
                });

                div->form("/wifi-save", "POST", [](HtmlBuilder *form) {
                    auto div = form->div();
                    div->text("SSID: ");
                    div->input("text", "ssid");

                    div = form->div();
                    div->text("Password: ");
                    div->input("text", "password");

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
            body->div()->text("Wifi settings saved.");
        });

        server.send(200, "text/html", html.build());
    });
}

void WebConfiguration::mqttPage()
{
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