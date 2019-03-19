#include "WebServer.h"
#include "config.h"

void getSoftAPClients()
{
    HTTPClient server;
    struct station_info *station = wifi_softap_get_station_info();
    while (station)
    {
        Serial.print("BSSID: ");
        for (int k = 0; k < 6; k++)
        {
            Serial.print(station->bssid[k]);
            Serial.print(":");
        }
        Serial.println();

        Serial.print("IP: ");
        String clientIP = IPAddress(&station->ip).toString();
        Serial.println(clientIP);
        
        // server.begin("http://" + clientIP+":80");
        // server.begin("http://192.168.43.1:12345");
        WiFiClient wifiClient;
        wifiClient.connect(IPAddress(192,168,44,100),80);

        // server.begin(&wifiClient,"http://" + clientIP);

        int httpCode = server.GET();

        Serial.print("HTTP CODE: ");
        Serial.println(httpCode);

        Serial.print("URL: ");
        Serial.println("http://" + clientIP + ":80");

        if (httpCode > 0)
        {
            String payload = server.getString(); //Get the request response payload
            Serial.println("Payload: ");
            Serial.println(payload); //Print the response payload
        }
        server.end();

        station = STAILQ_NEXT(station, next);
    }
    wifi_softap_free_station_info();
}


WebServer::WebServer(){
    this->server = new ESP8266WebServer(config::WEB_SERVER_PORT);
}

WebServer::~WebServer(){
    this->server->close();
    delete this->server;
}

void WebServer::initRoutes()
{   
    ESP8266WebServer * server = this->server;
    WebServer * webserver = this;
    this->server->on("/", [webserver,server]() {
        server->send(200, "text/plain", "Web server");
        // if (server->client().localIP().toString() == "192.168.44.1")
        //     webserver->softAPClient = server->client();

        // Serial.println(webserver->softAPClient.available());
       
      /*  Serial.println(server->client().localIP().toString());
        Serial.println(server->client().localPort());

        Serial.println(server->client().remoteIP().toString());
        Serial.println(server->client().remotePort());*/

        //getSoftAPClients();
    });
}

void WebServer::start(){
    this->initRoutes();
    this->server->begin();
}

void WebServer::handleClient(){
    this->server->handleClient();
}