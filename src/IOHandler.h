#ifndef IOHANDLER_H
#define IOHANDLER_H

#include <Arduino.h>

#define STATUS_LED 2

#define OUTPUT_1 2
#define OUTPUT_2 0
#define OUTPUT_3 4
#define OUTPUT_4 5

#define INPUT_1 A0
#define INPUT_2 6
#define INPUT_3 7
#define INPUT_4 11

class IOHandler
{
  private:
    int outputPins[4];
    int inputPins[4];

  public:
    IOHandler();
    void initPins();
    void setStatusLED(int state){ digitalWrite(STATUS_LED,state); }
    int getNumberOfInput(){ return sizeof(this->inputPins)/ sizeof(int); }
    char* getInputAll();
    void setOutput(int channel,int state);
};

#endif
