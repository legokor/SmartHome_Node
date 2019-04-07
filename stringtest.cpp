#include <stdio.h>
#include <string.h>

struct Message{
    char command[20];
    char nodeid[17];
    int input[4];
    int output[4];
    char *extra;
    ~Message() { delete extra; }
};

struct Message*  nmeaparser(char *message){
    memmove(message, message + 1, strlen(message));
    message[strlen(message) - 1] = 0;

    char *p = strtok(message, ",");
    struct Message * newMessage = new Message;
    int fieldIndex = 0;
    while(p)
    {
        if(fieldIndex == 0) strcpy(newMessage->command,p);
        else if(fieldIndex == 1) strcpy(newMessage->nodeid,p);
        else if(fieldIndex == 2)  for(int k = 0; k<4;k++) newMessage->input[k] = p[k]; 
        else if(fieldIndex == 3)  for(int k = 0; k<4;k++) newMessage->output[k] = p[k];
        else
        {
            newMessage->extra = new char[strlen(p)];
            strcpy(newMessage->extra, p);
        }

        p = strtok(NULL, ",");
        fieldIndex++;
    }

    return newMessage;
}

const char * createNMEA(struct Message *msg)
{
    int size = strlen(msg->nodeid) + strlen(msg->command);
    size += sizeof(msg->output)/sizeof(int) + sizeof(msg->input)/sizeof(int);
    size += strlen(msg->extra) + 3 + 4; //$*\0 and 4*,

    char * msgStr = new char[size];
    msgStr[0] = '$';
    msgStr[1] = '\0';


    strcat(msgStr,msg->command);
    strcat(msgStr,",");

    strcat(msgStr,msg->nodeid);
    strcat(msgStr,",");
    
    int offset = strlen(msgStr);
    for(int k = 0; k < 4; k++)  msgStr[k+offset] += (char) msg->input[k];
    msgStr[offset + 4] = '\0';
    strcat(msgStr,",");

    offset = strlen(msgStr);
    for(int k = 0; k < 4; k++)  msgStr[k+offset] += (char) msg->output[k];
    msgStr[offset + 4] = '\0';
    strcat(msgStr,",");

    strcat(msgStr,msg->extra);

    msgStr[size - 2] = '*';
    msgStr[size - 1] = '\0';
    
    return msgStr;
}

int main(){
    char messageString[] = "$COMMAND,NODEID,0000,0000,vlaamirandomszoveg*";

    // printf("%d", sizeof(messageString));
   
    struct Message * message = nmeaparser(messageString);
    
    printf("%s\n", createNMEA(message));

    /*printf("%s\n", message->command);
    printf("%s\n", message->nodeid);
    printf("Extra: %s", message->extra);

    printf("Input: ");
    for(int k = 0; k< 4;k++)
        printf("%d,",message->input[k]);

    printf("\nOutput: ");
    for(int k = 0; k< 4;k++)
        printf("%d,",message->output[k]);*/

    return 0;
}

