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
  len = serialcomms::readPacket();
  if(len != -1)
  {
    serialcomms::displayPacket(serialcomms::received, len, true);
    serialcomms::decodeFloat(&testFloat, serialcomms::received, 0);
    Serial.println("Float: " + String(testFloat, 3));
  }
  
}