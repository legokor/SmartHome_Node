#include "config.h"
#include <ESP8266WebServer.h>
#include "MeshNetwork.h"
#include "UDPServer.h"

bool started = false;
UDPServer udpServer;

void setup()
{
  WiFi.persistent(false);

  pinMode(board::STATUS_LED, OUTPUT);

  Serial.begin(115200);
  Serial.println();
  
  MeshNetwork mesh;
  started = mesh.init();

  if(started){
    udpServer.start();
  }
}

void loop() {
  if(started){
    udpServer.handleClient();
  }
}
