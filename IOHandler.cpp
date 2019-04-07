#include "IOHandler.h"
#include <stdlib.h>

IOHandler::IOHandler(){
    this->outputPins[0] = 2; //D4 led
    this->outputPins[1] = 0; //D3
    this->outputPins[2] = 4; //D2
    this->outputPins[3] = 5; //D1

    this->inputPins[0] = 17; //A0
    this->inputPins[1] = 12; //D6
    this->inputPins[2] = 13; //D7
    this->inputPins[3] = 14; //D5
}

void IOHandler::initPins(){
    Serial.println("Pins initialized");
    pinMode(STATUS_LED, OUTPUT);

    for (int k = 0; k < sizeof(this->inputPins) / sizeof(int); k++)
    {
        pinMode(this->inputPins[k], INPUT);
    }
    for (int k = 0; k < sizeof(this->outputPins) / sizeof(int); k++)
    {
        pinMode(this->outputPins[k], OUTPUT);
        setOutput(k,0);
    }
}

char * IOHandler::getInputAll(){
    char * values = new char[sizeof(this->inputPins)/ sizeof(int)];
    
    for(int k = 0; k < sizeof(this->inputPins) / sizeof(int); k++){
        if(k == 0){
            values[k] = analogRead(this->inputPins[k]) / 1024 * 255;
        }
        else values[k] = digitalRead(this->inputPins[k]);
    }

    return values;
}

void IOHandler::setOutput(int channel,int state){
    digitalWrite(this->outputPins[channel], state);
}