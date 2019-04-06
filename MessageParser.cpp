#include "MessageParser.h"
#include <string.h>
#include <stdio.h>

struct Message  *MessageParser::parseNMEA(char * message){
    memmove(message, message + 1, strlen(message));
    message[strlen(message) - 1] = 0;

    char *p = strtok(message, ",");
    struct Message * newMessage = new Message;
    int fieldIndex = 0;
    while (p)
    {
        if (fieldIndex == 0)
            strcpy(newMessage->command, p);
        if (fieldIndex == 1)
            strcpy(newMessage->nodeid, p);
        if (fieldIndex == 2)
            for (int k = 0; k < 4; k++)
                newMessage->input[k] = p[k];
        if (fieldIndex == 3)
            for (int k = 0; k < 4; k++)
                newMessage->output[k] = p[k];

        p = strtok(NULL, ",");
        fieldIndex++;
    }

    return newMessage;
}

bool MessageParser::isValid(char * message){
    int checkSum = 0;
    for (int l = 0; l < strlen(message); l++)
    {
        checkSum ^= message[l];
    }
    printf("0x%02X\n", checkSum);
}