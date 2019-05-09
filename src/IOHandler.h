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

#define NUMBER_OF_INPUT 4
#define NUMBER_OF_OUTPUT 4

class IOHandler
{
  private:
    int outputPins[NUMBER_OF_INPUT];
    int inputPins[NUMBER_OF_OUTPUT];

  public:
    IOHandler();
    void initPins();
    void setStatusLED(int state){ digitalWrite(STATUS_LED,state); }
    int getNumberOfInput() { return NUMBER_OF_INPUT; }
    char* getInputAll();
    void setOutput(int channel,int state);
};

#endif
