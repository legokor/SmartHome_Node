#include "WebServer.h"
#include "config.h"
#include <WiFiUdp.h>
#include "MessageParser.h"

#define LOCAL_UDP_PORT 1360

void getSoftAPClients()
{
    /*struct station_info *station = wifi_softap_get_station_info();
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

        station = STAILQ_NEXT(station, next);
    }
    wifi_softap_free_station_info();*/
}

WebServer::WebServer()
{
    this->udp = new WiFiUDP();
}

WebServer::~WebServer()
{
    delete this->udp;
}

void WebServer::start(){
    this->udp->begin(LOCAL_UDP_PORT);
}

void WebServer::handleClient(){
    int packetSize = this->udp->parsePacket();
    if (packetSize)
    {
        // receive incoming EthernetUdp packets
        Serial.printf("Received %d bytes from %s, port %d\n", packetSize, this->udp->remoteIP().toString().c_str(), this->udp->remotePort());
        int len = this->udp->read(incomingPacket, 255);
        if (len > 0)
        {
            incomingPacket[len] = 0;
        }

        Serial.printf("Udp packet contents: %s\n", incomingPacket);

        //Serial.printf("Converted: %s\n", MessageParser::parse(incomingPacket));

        // send back a reply, to the IP address and port we got the packet from
        // this->udp->beginPacket(this->udp->remoteIP(), this->udp->remotePort());
        this->udp->beginPacket(WiFi.gatewayIP(), 1360);
        this->udp->write("got it");
        this->udp->endPacket();

        this->udp->beginPacket(this->udp->remoteIP(), this->udp->remotePort());
        this->udp->write("got it");
        this->udp->endPacket();
    }
}
