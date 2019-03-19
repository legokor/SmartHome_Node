#include "config.h"
#include <ESP8266WebServer.h>
#include "MeshNetwork.h"
#include "WebServer.h"

bool started = false;
WebServer webserver;

void setup()
{
  WiFi.persistent(false);

  pinMode(board::STATUS_LED, OUTPUT);

  Serial.begin(115200);
  Serial.println();
  
  MeshNetwork mesh;
  started = mesh.init();

  if(started){
    webserver.start();
  }
}

void loop() {
  if(started){
    webserver.handleClient();
  }
}
