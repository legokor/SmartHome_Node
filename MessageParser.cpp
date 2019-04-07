#include "MessageParser.h"
#include <string.h>
#include <stdio.h>

Message *MessageParser::parseNMEA(char *tmpMessage)
{
    char *message = new char[strlen(tmpMessage) + 1];
    message[strlen(message)] = '\0';
    strcpy(message,tmpMessage);

    memmove(message, message + 1, strlen(message));
    message[strlen(message) - 1] = 0;

    char *p = strtok(message, ",");
    Message * newMessage = new Message();
    int fieldIndex = 0;
    while (p)
    {
        if (fieldIndex == 0)
            strcpy(newMessage->command, p);
        else if (fieldIndex == 1)
            strcpy(newMessage->nodeid, p);
        else if (fieldIndex == 2)
            for (int k = 0; k < 4; k++)
                newMessage->input[k] = p[k];
        else if (fieldIndex == 3)
            for (int k = 0; k < 4; k++)
                newMessage->output[k] = p[k];
        else if (fieldIndex == 4){
            newMessage->extra = new char[strlen(p)+1];
            strcpy(newMessage->extra,p);
        }

        p = strtok(NULL, ",");
        fieldIndex++;
    }

    return newMessage;
}

char * MessageParser::createNMEA(Message * msg){
    int size = strlen(msg->nodeid) + strlen(msg->command);
    size += sizeof(msg->output) / sizeof(int) + sizeof(msg->input) / sizeof(int);
    size += strlen(msg->extra) + 3 + 4; //$*\0 and 4*,

    char *msgStr = new char[size];
    msgStr[0] = '$';
    msgStr[1] = '\0';

    strcat(msgStr, msg->command);
    strcat(msgStr, ",");

    strcat(msgStr, msg->nodeid);
    strcat(msgStr, ",");

    int offset = strlen(msgStr);
    for (int k = 0; k < 4; k++)
        msgStr[k + offset] = msg->input[k];
    msgStr[offset + 4] = '\0';
    strcat(msgStr, ",");

    offset = strlen(msgStr);
    for (int k = 0; k < 4; k++)
        msgStr[k + offset] = msg->output[k];
    msgStr[offset + 4] = '\0';
    strcat(msgStr, ",");

    strcat(msgStr, msg->extra);

    msgStr[size - 2] = '*';
    msgStr[size - 1] = '\0';

    return msgStr;
}

bool MessageParser::isValid(char * message){
    int checkSum = 0;
    for (int l = 0; l < strlen(message); l++)
    {
        checkSum ^= message[l];
    }
    printf("0x%02X\n", checkSum);
}