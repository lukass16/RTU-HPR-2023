#include <Arduino.h>
#include <ESP32SPISlave.h>

#define S_SCK 32
#define S_CS 33
#define S_MOSI 12
#define S_MISO 13

// https://github.com/hideakitai/ESP32DMASPI
// https://forum.arduino.cc/t/how-to-receive-data-via-spi/251768

//* Testing variables
bool command_active = 0;

ESP32SPISlave slave;

static constexpr uint32_t BUFFER_SIZE{32};
uint8_t spi_slave_tx_buf[BUFFER_SIZE];
uint8_t spi_slave_rx_buf[BUFFER_SIZE];

void setup()
{
	// put your setup code here, to run once:
	Serial.begin(115200);
	Serial.println("Start testing");

	// set SPI
	slave.setDataMode(SPI_MODE0);
	// use HSPI with our own pin assignment
	slave.begin(HSPI, S_SCK, S_MISO, S_MOSI, S_CS);

	// for safety - setting pins accordingly
	pinMode(S_SCK, INPUT);
	pinMode(S_MOSI, INPUT);

	pinMode(S_MISO, OUTPUT);

	//* Create custom value for tx buffer testing
	for (uint8_t i = 0; i < BUFFER_SIZE; i++)
	{
		spi_slave_tx_buf[i] = i;
	}
}

void loop()
{
	// block until the transaction comes from master - simultaneously send active data in tx buffer
	slave.wait(spi_slave_rx_buf, spi_slave_tx_buf, BUFFER_SIZE);

	// if transaction has completed from master, available() returns size of results of transaction and `spi_slave_rx_buf` is automatically updated
	while (slave.available())
	{
		// check if command
		if (spi_slave_rx_buf[0] == 0x07)
		{
			Serial.println("Received command: " + String(spi_slave_rx_buf[0]));
			command_active = 1;
			for (uint8_t i = 0; i < 8; i++) // set sendable data
			{
				spi_slave_tx_buf[i] = 237 + i;
			}
			Serial.print("Set new tx buffer: ");
			for (int i = 0; i < 8; i++)
			{
				Serial.print(String(spi_slave_tx_buf[i]) + " ");
			}
			Serial.println("\n");
		}
		else
		{
			Serial.print("Received dummy data: ");
			for (int i = 0; i < 8; i++)
			{
				Serial.print(String(spi_slave_rx_buf[i]) + " ");
			}
			Serial.println();

			command_active = 0;
			for (uint8_t i = 0; i < 8; i++) // reset tx buffer to zeros
			{
				spi_slave_tx_buf[i] = 0;
			}
			Serial.print("Reset tx buffer: ");
			for (int i = 0; i < 8; i++)
			{
				Serial.print(String(spi_slave_tx_buf[i]) + " ");
			}
			Serial.println("\n");
		}

		slave.pop(); // pop the oldest transaction result
	}
}
