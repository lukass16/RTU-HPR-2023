#include <Arduino.h>
#include <spislave_wrapper.h>

#define GO_IDLE 0x01
#define START_STABILISING 0x02
#define START_POINTING 0x03

uint8_t command = 0;

void setup()
{
	// put your setup code here, to run once:
	Serial.begin(115200);
	spislave::setup();
}

void loop()
{
	spislave::poll(); // check if new transaction
	command = spislave::processTransaction(); // check what type: command or data retrieving (0) - if data retrieving then give data, otherwise retrieve command

	// if(command != 0)
	// {
	// 	Serial.println("Received command: "+ String(command));
	// }
}
