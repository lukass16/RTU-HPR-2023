#include <Arduino.h>
#include "motor_wrapper.h"

void setup()
{
  Serial.begin(115200);
  motor::setup();
  motor::setupEncoder();
}

void loop()
{
  motor::printChannelValues();
  motor::printEncoderCounter();
}
