#include <Arduino.h>
#include <SPI.h>

#define CS 10

int flag = 1;
uint8_t send = 0x07;

// https://circuitdigest.com/microcontroller-projects/arduino-spi-communication-tutorial
// https://docs.arduino.cc/learn/communication/spi
// https://randomnerdtutorials.com/esp32-spi-communication-arduino/#:~:text=Report%20Ad-,Introducing%20ESP32%20SPI%20Communication%20Protocol,SPI%20or%20with%20another%20microcontroller.
// https://forum.arduino.cc/t/how-to-receive-data-via-spi/251768
// http://www.gammon.com.au/forum/?id=10892&reply=1#reply1

//? Notes:
/*
1) Test one byte transactions
2) check pins - test other pins
3) read about special pin definitions
4) test from 2 separate PCs
- when removing (grounding) MOSI pin (12) teh data comes through
*/

void setup()
{
	Serial.begin(9600);
	Serial.println("Starting SPI test");

	delay(2000);

	// Set the CS pin as output
	pinMode(CS, OUTPUT);

	SPI.begin();
}

void loop()
{
	if(flag)
	{
		send = 0x07;
		flag = 0;
	}
	else
	{
		send = 0x27;
		flag = 1;
	}

	//* Send command
	SPI.beginTransaction(SPISettings(1e6, MSBFIRST, SPI_MODE0));
	// Set the CS pin low to select the slave
	digitalWrite(CS, LOW);
	// Transfer byte to the slave
	for (int i = 0; i < 4; i++) // retrieving 8 bytes
	{
		byte received = SPI.transfer(send); // transfer dummy data - get back answer
		Serial.print(String(received) + " ");
		delayMicroseconds(10);
		send++;
	}
	// Set the CS pin high to deselect the slave
	digitalWrite(CS, HIGH);
	SPI.endTransaction();

	delayMicroseconds(1000); // a delay between transactions - helped to enable both sides to update and clear buffers

	//* Receive answer
	SPI.beginTransaction(SPISettings(1e6, MSBFIRST, SPI_MODE0));
	// Set the CS pin low to select the slave
	digitalWrite(CS, LOW);
	// Retrieve answers
	Serial.print("\tReceived response: ");
	for (int i = 0; i < 8; i++) // retrieving 8 bytes
	{
		byte received = SPI.transfer(0x00); // transfer dummy data - get back answer
		Serial.print(String(received) + " ");
		delayMicroseconds(10);
	}
	Serial.println();
	// Set the CS pin high to deselect the slave
	digitalWrite(CS, HIGH);
	SPI.endTransaction();

	delay(1000);
}

