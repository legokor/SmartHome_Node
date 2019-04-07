#ifndef IOHANDLER_H
#define IOHANDLER_H

#include <Arduino.h>

#define STATUS_LED 2

class IOHandler
{
  private:
    void initPins();

  public:
    IOHandler();
    void setStatusLED(int state){ pinMode(STATUS_LED,state); }
};

#endif
