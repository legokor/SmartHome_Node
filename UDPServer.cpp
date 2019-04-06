#include "UDPServer.h"
#include "config.h"
#include <WiFiUdp.h>
#include "MessageParser.h"

#define LOCAL_UDP_PORT 1360

void UDPServer::sendToChildren(char *incomingPacket)
{
    struct station_info *station = wifi_softap_get_station_info();
    while (station)
    {
        this->udp->beginPacket(IPAddress(&station->ip), LOCAL_UDP_PORT);
        this->udp->write(incomingPacket);
        this->udp->endPacket();

        station = STAILQ_NEXT(station, next);
    }
    wifi_softap_free_station_info();
}

UDPServer::UDPServer()
{
    this->udp = new WiFiUDP();
}

UDPServer::~UDPServer()
{
    delete this->udp;
}

void UDPServer::start()
{
    this->udp->begin(LOCAL_UDP_PORT);
}

struct Message *UDPServer::handleMessage()
{
    int packetSize = this->udp->parsePacket();
    if (!packetSize) return nullptr;

    // Serial.printf("Received %d bytes from %s, port %d\n", packetSize, this->udp->remoteIP().toString().c_str(), this->udp->remotePort());
    int len = this->udp->read(incomingPacket, 255);
    if (len > 0)  incomingPacket[len] = 0;

    struct Message * newMessage = MessageParser::parseNMEA(incomingPacket);
    
    /*Serial.printf("%s\n", newMessage.command);
    Serial.printf("%s\n", newMessage.nodeid);

    Serial.printf("Input: ");
    for (int k = 0; k < 4; k++)
        Serial.printf("%d,", newMessage.input[k]);

    Serial.printf("\nOutput: ");
    for (int k = 0; k < 4; k++)
        Serial.printf("%d,", newMessage.output[k]);*/

    //Message Routing
    if (strcmp(newMessage->nodeid, WiFi.softAPmacAddress().c_str()) == 0)
        return newMessage;
    else if (WiFi.gatewayIP() == this->udp->remoteIP())
        this->sendToChildren(incomingPacket);
    else
        this->sendToParent(incomingPacket);

    delete newMessage;
}

void UDPServer::sendToParent(char *incomingPacket)
{
    this->udp->beginPacket(WiFi.gatewayIP(), 1360);
    this->udp->write(incomingPacket);
    this->udp->endPacket();
}

void UDPServer::handleClient(){
    struct Message * message = this->handleMessage();
    if(message == nullptr) return;
    


    delete message;
}