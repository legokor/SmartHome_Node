#ifndef CLIENT_HANDLER_H
#define CLIENT_HANDLER_H

#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include "MessageParser.h"
#include "IOHandler.h"

class ClientHandler{
    private:
      char incomingPacket[256];
      void sendToChildren(char *packet);
      void sendToParent(char * packet);
      char * listChildren();
      WiFiUDP * udp;

    public:
      ClientHandler();
      ~ClientHandler();
      void start();
      struct Message *handleMessage();
      void handle(IOHandler *);
};

#endif