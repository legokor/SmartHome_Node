#include "config.h"
#include <ESP8266WebServer.h>
#include "MeshNetwork.h"
#include "ClientHandler.h"
#include "IOHandler.h"

bool started = false;
ClientHandler clientHandler;
IOHandler * ioHandler;

void setup()
{
  ioHandler = new IOHandler();
  WiFi.persistent(false);

  Serial.begin(115200);
  
  MeshNetwork mesh;
  started = mesh.init();

  if(started){
    ioHandler->setStatusLED(1);
    clientHandler.start();
  }
}

void loop() {
  if(started){
    clientHandler.handle(ioHandler);
  }
}
