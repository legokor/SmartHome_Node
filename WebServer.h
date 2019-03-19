#ifndef WEBSERVER
#define WEBSERVER

#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <ESP8266HTTPClient.h>

class WebServer{
    private:
      ESP8266WebServer* server;
      void initRoutes();
      void handle_main();
    public:
      WiFiClient softAPClient;
        WebServer();
        ~WebServer();
        void start();
        void handleClient();
};

#endif