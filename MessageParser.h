#ifndef MESSAGE_PARSER_H
#define MESSAGE_PARSER_H

#include <stdio.h>

class Message
{
    public:
        char command[10];
        char nodeid[17];
        int input[4];
        int output[4];
        char * extra;
        ~Message(){ delete extra;}
};

class MessageParser{
    public:
      static Message * parseNMEA(char *message);
      static char * createNMEA(Message *);
      static bool isValid(char *message);
};

#endif