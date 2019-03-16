#include "config.h"
#include "MeshNetwork.h"



void setup()
{
  WiFi.persistent(false);

  pinMode(board::STATUS_LED, OUTPUT);

  Serial.begin(115200);
  Serial.println();
  
  MeshNetwork mesh;
  mesh.init();
}

void loop() {

}
