#include "MessageParser.h"

#include <stdio.h>

void Message::setPayload(char * content, int size){
    delete this->payload;
    this->payload = content;
    this->payloadSize = size;
}

Message *MessageParser::parseNMEA(char *tmpMessage,int size)
{
    char *message = new char[size];
    memcpy(message,tmpMessage, size);
    memmove(message, message + 1, size);

    unsigned char checkSum = 0;
    checkSum |= (message[size-2] - '0') << 4;
    checkSum |= (message[size - 1] - '0');

    message[size - 3] = 0;

    char *p = strtok(message, ",");
    struct Message * newMessage = new Message;

    newMessage->checkSum = checkSum;

    int fieldIndex = 0;
    while(p)
    {
        if(fieldIndex == 0) strcpy(newMessage->command,p);
        else if(fieldIndex == 1) strcpy(newMessage->nodeid,p);
        p = strtok(NULL, ",");
        fieldIndex++;
    }

    int offset = strlen(newMessage->command) + strlen(newMessage->nodeid) + 1 + 1 + 1; // $,,
    newMessage->payloadSize = (size-1) - offset - 3; 
    newMessage->payload = new char[newMessage->payloadSize];
    memcpy(newMessage->payload, tmpMessage + offset, newMessage->payloadSize);
    delete message;

    return newMessage;
}

char * MessageParser::createNMEA(Message * msg){

    int size = msg->getSize();
  
    char *msgStr = new char[size];
    msgStr[0] = '$';
    msgStr[1] = '\0';

    strcat(msgStr, msg->command);
    strcat(msgStr, ",");
    strcat(msgStr, msg->nodeid);
    strcat(msgStr, ",");
    
    int offset = strlen(msgStr);
    for (int k = 0; k < msg->payloadSize; k++)
    {
        msgStr[offset + k] = msg->payload[k];
    }

    msgStr[size - 3] = '*';

    int chkSum = MessageParser::calculateCheckSum(msgStr);
    char chkStr[3];
    sprintf(chkStr,"%02x",chkSum);
    
    msgStr[size - 2] = chkStr[0];
    msgStr[size - 1] = chkStr[1];

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