#ifndef MESSAGE_PARSER_H
#define MESSAGE_PARSER_H

#include <stdio.h>

class Message
{
    public:
        char command[10];
        char nodeid[17];
        char * payload;
        int checkSum;
        ~Message(){ delete payload;}
};

class MessageParser{
    public:
      static Message * parseNMEA(char *message);
      static char * createNMEA(Message *);
      static int calculateCheckSum(char *message);
};

#endif