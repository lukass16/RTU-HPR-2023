#include <Arduino.h>
#include <SPI.h>

#define CS 10

// https://circuitdigest.com/microcontroller-projects/arduino-spi-communication-tutorial
// https://docs.arduino.cc/learn/communication/spi
// https://randomnerdtutorials.com/esp32-spi-communication-arduino/#:~:text=Report%20Ad-,Introducing%20ESP32%20SPI%20Communication%20Protocol,SPI%20or%20with%20another%20microcontroller.
// https://forum.arduino.cc/t/how-to-receive-data-via-spi/251768
// http://www.gammon.com.au/forum/?id=10892&reply=1#reply1

void setup()
{
	Serial.begin(9600);
	Serial.println("Starting SPI test");

	// Set the CS pin as output
	pinMode(CS, OUTPUT);

	SPI.begin();
}

void loop()
{
	Serial.print("Sending: ");
	SPI.beginTransaction(SPISettings(14000000, MSBFIRST, SPI_MODE0));
	// Set the CS pin low to select the slave
	digitalWrite(CS, LOW);
	// Transfer byte to the slave
	uint8_t b = 0x00;
	for (int i = 0; i < 10; i++)
	{
		SPI.transfer(b);
		//Serial.print(String(b) + " ");
		b++;
	}
	Serial.println();

	// Set the CS pin high to deselect the slave
	digitalWrite(CS, HIGH);
	delay(50);
}