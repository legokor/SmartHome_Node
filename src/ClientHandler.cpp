#include "ClientHandler.h"
#include <WiFiUdp.h>
#include "config.h"
#include "MessageParser.h"

#define LOCAL_UDP_PORT 1360



ClientHandler::ClientHandler()
{
    this->udp = new WiFiUDP();
    this->incomingPacket = nullptr;
}

ClientHandler::~ClientHandler()
{
    delete this->udp;
}

void ClientHandler::start()
{
    this->udp->begin(LOCAL_UDP_PORT);
}

void ClientHandler::sendToParent(Message * message)
{
    char* incomingPacket = MessageParser::createNMEA(message);
    this->udp->beginPacket(WiFi.gatewayIP(), 1360);
    this->udp->write(incomingPacket,message->getSize());
    this->udp->endPacket();
    delete incomingPacket;
}

void ClientHandler::sendToChildren(Message *message)
{
    char *incomingPacket = MessageParser::createNMEA(message);
    struct station_info *station = wifi_softap_get_station_info();
    while (station)
    {
        this->udp->beginPacket(IPAddress(&station->ip), LOCAL_UDP_PORT);
        this->udp->write(incomingPacket, message->getSize());
        this->udp->endPacket();

        station = STAILQ_NEXT(station, next);
    }
    wifi_softap_free_station_info();
    delete incomingPacket;
}

char*  ClientHandler::listChildren(){
    int size = WiFi.softAPgetStationNum()*6; //6*MAC
    
    char * outputString = new char[size];

    int offset = 0;
    struct station_info *station = wifi_softap_get_station_info();
    while (station)
    {
        memcpy(outputString+offset,station->bssid,6);
        offset += 6;
        station = STAILQ_NEXT(station, next);
    }
    wifi_softap_free_station_info();

    return outputString;
}

void ClientHandler::handle(IOHandler *ioHandler)
{
    Message *message = this->recieveMessage();
    if (!message) //Message is forwarded
        return;
    Serial.println(message->command);
    if (!strcmp(message->command, "READ"))
    {
        Serial.println("read");
        message->setPayload(ioHandler->getInputAll(), ioHandler->getNumberOfInput());
        this->sendToParent(message);
    }
    else if (!strcmp(message->command, "WRTE"))
    {
        int channel = message->payload[0];// - '0';
        int state = message->payload[1];// - '0';
        ioHandler->setOutput(channel, state);
        this->sendToParent(message);
    }
    else if (!strcmp(message->command, "LIST"))
    {
        Serial.println("list");
        message->setPayload(this->listChildren(), WiFi.softAPgetStationNum() * 6);
        this->sendToParent(message);
    }
    else if (!strcmp(message->command, "PING"))
    {
        Serial.println("ping");
        this->sendToChildren(message);
        memcpy(message->nodeid, WiFi.softAPmacAddress().c_str(), 17); 
        message->setPayload(ioHandler->getInputAll(), ioHandler->getNumberOfInput());  
        this->sendToParent(message);
    }

    delete message;
}

Message *ClientHandler::recieveMessage()
{
    int packetSize = this->udp->parsePacket();
    if (!packetSize)
        return nullptr;
    
    this->incomingPacket = new char[packetSize+1];

    int len = this->udp->read(this->incomingPacket, packetSize);
    if (len > 0) incomingPacket[packetSize] = 0;

    Serial.printf("INC: %s\n", this->incomingPacket);

    Message *newMessage = MessageParser::parseNMEA(this->incomingPacket, packetSize);
    delete this->incomingPacket;
    //Message Routing
    if (strcmp(newMessage->nodeid, WiFi.softAPmacAddress().c_str()) == 0 || 
        strcmp(newMessage->nodeid,"FF:FF:FF:FF:FF:FF") == 0) //If its ours
        return newMessage;
    else if (WiFi.gatewayIP() == this->udp->remoteIP()) //If come from parent
        this->sendToChildren(newMessage);
    else    //Anything else forwarded to parent
        this->sendToParent(newMessage);

    delete newMessage;
    return nullptr;
}
