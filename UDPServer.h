#ifndef UDPServer
#define UDPServer

#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include "MessageParser.h"

class UDPServer{
    private:
      void initRoutes();
      void handle_main();
      char incomingPacket[256];
      void sendToChildren(char *packet);
      void sendToParent(char * packet);
      WiFiUDP * udp;

    public:
      UDPServer();
      ~UDPServer();
      void start();
      struct Message *handleMessage();
      void handleClient();
};

#endif