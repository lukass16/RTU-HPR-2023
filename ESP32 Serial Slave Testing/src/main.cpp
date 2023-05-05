#include <Arduino.h>
#include "serial_comms.h"

float testFloat = 0;
int len = 0;

void setup()
{
  Serial.begin(115200);
  Serial.println("Starting serial comms test");

  //* Setting up Serial comms
  serialcomms::setup();
}

void loop()
{
  len = serialcomms::readPacket(true);
}