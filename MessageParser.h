#ifndef MESSAGE_PARSER_H
#define MESSAGE_PARSER_H

#include <stdio.h>

struct Message
{
    char command[20];
    char nodeid[17];
    int input[4];
    int output[4];
};

class MessageParser{
    public:
      static struct Message * parseNMEA(char *message);
      static bool isValid(char *message);
};

#endif