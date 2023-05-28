#include "states/preperation_state.cpp"

Context *mode = new Context(new PreperationState);

void setup() {
    Serial.begin(115200);
    delay(1000);

    mode->Start();
}
 
void loop() {}