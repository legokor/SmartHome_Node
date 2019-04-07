#ifndef MESSAGE_PARSER_H
#define MESSAGE_PARSER_H

#include <stdio.h>
#include <string.h>

class Message
{
    public:
        char command[10];
        char nodeid[17];
        char * payload;
        int checkSum;
        int payloadSize;
        int getSize() { return strlen(command) + strlen(nodeid) + payloadSize + 2 + 2 + 2; } //$* plus ,, plus checksum
        Message(){ payloadSize = 0; payload = nullptr;}
        ~Message(){ delete payload;}
};

class MessageParser{
    public:
      static Message * parseNMEA(char *message, int size);
      static char * createNMEA(Message *);
      static int calculateCheckSum(char *message);
};

#endif