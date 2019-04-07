#include "ClientHandler.h"
#include <WiFiUdp.h>
#include "config.h"
#include "MessageParser.h"

#define LOCAL_UDP_PORT 1360

void ClientHandler::sendToChildren(Message * message)
{
    char *incomingPacket = MessageParser::createNMEA(message);
    struct station_info *station = wifi_softap_get_station_info();
    while (station)
    {
        this->udp->beginPacket(IPAddress(&station->ip), LOCAL_UDP_PORT);
        this->udp->write(incomingPacket,message->getSize());
        this->udp->endPacket();

        station = STAILQ_NEXT(station, next);
    }
    wifi_softap_free_station_info();
    delete incomingPacket;
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
 
    Message * newMessage = MessageParser::parseNMEA(incomingPacket,len);
 
    //Message Routing
    if (strcmp(newMessage->nodeid, WiFi.softAPmacAddress().c_str()) == 0)
        return newMessage;
    else if (WiFi.gatewayIP() == this->udp->remoteIP())
        this->sendToChildren(newMessage);
    else
        this->sendToParent(newMessage);

    delete newMessage;
    return nullptr;
}

void ClientHandler::sendToParent(Message * message)
{
    char* incomingPacket = MessageParser::createNMEA(message);
    this->udp->beginPacket(WiFi.gatewayIP(), 1360);
    this->udp->write(incomingPacket,message->getSize());
    this->udp->endPacket();
    delete incomingPacket;
}

char*  ClientHandler::listChildren(){
    int size = WiFi.softAPgetStationNum()*6;
    
    char * outputString = new char[size];

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

    if(!strcmp(message->command,"READ")){
        Serial.println("read");
        delete message->payload;
        message->payload = ioHandler->getInputAll();
        message->payloadSize = ioHandler->getNumberOfInput();
        this->sendToParent( message );
    }
    else if(!strcmp(message->command,"WRTE")){
       
        int channel = message->payload[0] - '0';
        int state = message->payload[1] - '0';
        Serial.printf("write channel: %d state: %d\n",channel,state);
        ioHandler->setOutput(channel,state);
    }
    else if(!strcmp(message->command,"LIST")){
        Serial.println("list");
        delete message->payload;
        message->payload = this->listChildren();
        message->payloadSize = WiFi.softAPgetStationNum()*6;
        this->sendToParent( message );
    }

    delete message;
}