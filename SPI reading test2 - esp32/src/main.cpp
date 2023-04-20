#include <Arduino.h>
#include <ESP32SPISlave.h>

#define S_SCK 32
#define S_CS 33
#define S_MOSI 12
#define S_MISO 13

// https://github.com/hideakitai/ESP32DMASPI
// https://forum.arduino.cc/t/how-to-receive-data-via-spi/251768

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
}

void loop()
{
	// block until the transaction comes from master
	slave.wait(spi_slave_rx_buf, spi_slave_tx_buf, BUFFER_SIZE);

	// if transaction has completed from master, available() returns size of results of transaction and `spi_slave_rx_buf` is automatically updated
	while (slave.available())
	{
		for(int i = 0; i < 15; i++)
		{
			Serial.print(String(spi_slave_rx_buf[i]) + " ");
		}
		Serial.println("Received: " + String(slave.available()));

		slave.pop(); // pop the oldest transaction result
	}
}
