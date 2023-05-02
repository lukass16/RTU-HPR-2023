#include <Arduino.h>
#include <SPI.h>

static constexpr uint8_t VSPI_SS{SS}; // default: GPIO 5
SPIClass master(VSPI);
static const uint32_t BUFFER_SIZE{8};
uint8_t spi_master_tx_buf[BUFFER_SIZE]{1, 2, 3, 4, 5, 6, 7, 8};
uint8_t spi_master_rx_buf[BUFFER_SIZE]{0};

void setup()
{
	Serial.begin(115200);

	pinMode(VSPI_SS, OUTPUT);
	digitalWrite(VSPI_SS, HIGH);
	master.begin(); // VSPI = CS: 5, CLK: 18, MOSI: 23, MISO: 19

	Serial.println("Start Master Test");
}

void loop()
{
	// start master transaction
	master.beginTransaction(SPISettings(4000000, MSBFIRST, SPI_MODE0));
	digitalWrite(VSPI_SS, LOW);
	master.transferBytes(spi_master_tx_buf, spi_master_rx_buf, BUFFER_SIZE);
	digitalWrite(VSPI_SS, HIGH);
	master.endTransaction();
	delay(500);
	for(int i = 0; i < BUFFER_SIZE; i++)
	{
		Serial.print(spi_master_rx_buf[i]);
		Serial.print(" ");
	}
	Serial.println();
	delay(500);
	
}