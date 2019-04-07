#include "IOHandler.h"

IOHandler::IOHandler(){
    this->initPins();
}

void IOHandler::initPins(){
    pinMode(STATUS_LED, OUTPUT);
}