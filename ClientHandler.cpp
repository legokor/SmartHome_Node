#include "ClientHandler.h"
#include <WiFiUdp.h>
#include "config.h"
#include "MessageParser.h"

#define LOCAL_UDP_PORT 1360

void ClientHandler::sendToChildren(char *incomingPacket)
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

ClientHandler::ClientHandler()
{
    this->udp = new WiFiUDP();
}

ClientHandler::~ClientHandler()
{
    delete this->udp;
}

void ClientHandler::start()
{
    this->udp->begin(LOCAL_UDP_PORT);
}

Message *ClientHandler::handleMessage()
{
    int packetSize = this->udp->parsePacket();
    if (!packetSize) return nullptr;
    
    int len = this->udp->read(incomingPacket, 255);
    if (len > 0)  incomingPacket[len] = 0;

    Serial.printf("INC: %s\n",incomingPacket);
 
    Message * newMessage = MessageParser::parseNMEA(incomingPacket);
 
    //Message Routing
    if (strcmp(newMessage->nodeid, WiFi.softAPmacAddress().c_str()) == 0)
        return newMessage;
    else if (WiFi.gatewayIP() == this->udp->remoteIP())
        this->sendToChildren(incomingPacket);
    else
        this->sendToParent(incomingPacket);


    delete newMessage;
    return nullptr;
}

void ClientHandler::sendToParent(char *incomingPacket)
{
    this->udp->beginPacket(WiFi.gatewayIP(), 1360);
    this->udp->write(incomingPacket);
    this->udp->endPacket();
}

char*  ClientHandler::listChildren(){
    int size = WiFi.softAPgetStationNum()*6+1;
    
    char * outputString = new char[size];
    outputString[size-1] = '\0';

    int index = 0;
    struct station_info *station = wifi_softap_get_station_info();
    while (station)
    {
        for(int k = 0; k<6;k++){
            outputString[index] = station->bssid[k];
            index++;
        }
        station = STAILQ_NEXT(station, next);
    }
    wifi_softap_free_station_info();

    return outputString;
}

void ClientHandler::handle(IOHandler *ioHandler)
{
    Message * message = this->handleMessage();
    if(!message) return;

    if(!strcmp(message->command,"STAT")){

    }
    else if(!strcmp(message->command,"LIST")){

        message->extra = this->listChildren();
        char* packet = MessageParser::createNMEA(message);
        this->sendToParent(packet);
        Serial.println("list");
        Serial.println(packet);
        delete packet;
    }

    delete message;
}