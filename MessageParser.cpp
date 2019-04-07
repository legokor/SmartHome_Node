#include "MessageParser.h"
#include <string.h>
#include <stdio.h>

Message *MessageParser::parseNMEA(char *tmpMessage)
{
    char *message = new char[strlen(tmpMessage) + 1];
    message[strlen(message)] = '\0';
    strcpy(message,tmpMessage);

    memmove(message, message + 1, strlen(message));
    unsigned char checkSum = 0;
    checkSum |= (message[strlen(message)-2] - '0') << 4;
    checkSum |= (message[strlen(message)-1] - '0');
   
    message[strlen(message) - 3] = 0;

    char *p = strtok(message, ",");
    struct Message * newMessage = new Message;

    newMessage->checkSum = checkSum;

    int fieldIndex = 0;
    while(p)
    {
        if(fieldIndex == 0) strcpy(newMessage->command,p);
        else if(fieldIndex == 1) strcpy(newMessage->nodeid,p);
        else if(fieldIndex == 2)
        {
            newMessage->payload = new char[strlen(p)];
            strcpy(newMessage->payload, p);
        }

        p = strtok(NULL, ",");
        fieldIndex++;
    }

    return newMessage;
}

char * MessageParser::createNMEA(Message * msg){

    int size = strlen(msg->nodeid) + strlen(msg->command) + strlen(msg->payload);
    size += 3 + 2 + 2; //$*\0 plus ,, plus checksum

    char *msgStr = new char[size];
    msgStr[0] = '$';
    msgStr[1] = '\0';

    strcat(msgStr, msg->command);
    strcat(msgStr, ",");
    strcat(msgStr, msg->nodeid);
    strcat(msgStr, ",");
    strcat(msgStr, msg->payload);

    msgStr[size - 4] = '*';
    int chkSum = MessageParser::calculateCheckSum(msgStr);
    char chkStr[3];
    sprintf(chkStr,"%02x",chkSum);
    msgStr[size - 3] = chkStr[0];
    msgStr[size - 2] = chkStr[1];

    msgStr[size - 1] = '\0';

    return msgStr;
}

int MessageParser::calculateCheckSum(char * message){
    unsigned char checkSum = 0;
    for (int l = 0; l < strlen(message); l++)
    {   
        if(message[l] == '*') break;
        if(message[l] != '$') checkSum ^= message[l];
            
    }
    return checkSum;
    // printf("0x%02X\n", checkSum);
}