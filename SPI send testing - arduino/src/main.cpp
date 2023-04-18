#include <Arduino.h>
#include <SPI.h>

#define CS 10

// https://circuitdigest.com/microcontroller-projects/arduino-spi-communication-tutorial
// https://docs.arduino.cc/learn/communication/spi
// https://randomnerdtutorials.com/esp32-spi-communication-arduino/#:~:text=Report%20Ad-,Introducing%20ESP32%20SPI%20Communication%20Protocol,SPI%20or%20with%20another%20microcontroller.


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
	Serial.println("Beginning SPI transaction");
	SPI.beginTransaction(SPISettings(14000000, MSBFIRST, SPI_MODE0));
	// Set the CS pin low to select the slave
	digitalWrite(CS, LOW);
	// Transfer byte to the slave
	byte b = 0x01;
	SPI.transfer(b);
	// Set the CS pin high to deselect the slave
	digitalWrite(CS, HIGH);
	delay(1000);
}