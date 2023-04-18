#include <Arduino.h>
#include <ESP32DMASPISlave.h>

#define S_SCK 32
#define S_CS 33
#define S_MOSI 12
#define S_MISO 13

// https://github.com/hideakitai/ESP32SPISlave

ESP32DMASPI::Slave slave;

static const uint32_t BUFFER_SIZE = 8192;
uint8_t *spi_slave_tx_buf;
uint8_t *spi_slave_rx_buf;

void setup()
{
	// to use DMA buffer, use these methods to allocate buffer
	spi_slave_tx_buf = slave.allocDMABuffer(BUFFER_SIZE);
	spi_slave_rx_buf = slave.allocDMABuffer(BUFFER_SIZE);

	// slave device configuration
	slave.setDataMode(SPI_MODE0);
	slave.setMaxTransferSize(BUFFER_SIZE);

	// begin() after setting
	slave.begin(HSPI, S_SCK, S_MISO, S_MOSI, S_CS); 
}

void loop()
{
	// if there is no transaction in queue, add transaction
	if (slave.remained() == 0)
	{
		slave.queue(spi_slave_rx_buf, spi_slave_tx_buf, BUFFER_SIZE);
	}

	// if transaction has completed from master, available() returns size of results of transaction and buffer is automatically updated

	while (slave.available())
	{
		for(int i = 0; i < BUFFER_SIZE; i++)
		{
			Serial.print(spi_slave_rx_buf[i]);
		}
		Serial.println();

		slave.pop();
	}
}

/*
ESP32SPISlave slave;

static constexpr uint32_t BUFFER_SIZE{32};
uint8_t spi_slave_tx_buf[BUFFER_SIZE];
uint8_t spi_slave_rx_buf[BUFFER_SIZE];

void setup()
{
	// put your setup code here, to run once:
	Serial.begin(115200);

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
		// print out received data
		for(int i = 0; i < BUFFER_SIZE; i++)
		{
			Serial.print(spi_slave_rx_buf[i]);
		}
		Serial.println();

		slave.pop(); // pop the oldest transaction result
	}
}
*/