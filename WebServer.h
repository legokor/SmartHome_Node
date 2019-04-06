#ifndef WEBSERVER
#define WEBSERVER

#include <ESP8266WiFi.h>
#include <WiFiUdp.h>

class WebServer{
    private:
      void initRoutes();
      void handle_main();
      char incomingPacket[256];
      WiFiUDP * udp;

    public:
        WebServer();
        ~WebServer();
        void start();
        void handleClient();
};

#endif