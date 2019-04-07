#include "config.h"
#include <ESP8266WebServer.h>
#include "MeshNetwork.h"
#include "ClientHandler.h"
#include "IOHandler.h"

bool started = false;
ClientHandler clientHandler;
IOHandler ioHandler;

void setup()
{
  Serial.begin(115200);
  WiFi.persistent(false);
  ioHandler.initPins();

  MeshNetwork mesh;
  started = mesh.init();

  if(started){
    ioHandler.setStatusLED(1);
    clientHandler.start();
  }
}

void loop() {
  if(started){
    clientHandler.handle(&ioHandler);
  }
}
