#include "Arduino.h"
#include <ESP32SPISlave.h>

ESP32SPISlave slave;

static constexpr uint32_t BUFFER_SIZE {32};
uint8_t spi_slave_tx_buf[BUFFER_SIZE];
uint8_t spi_slave_rx_buf[BUFFER_SIZE];

void set_buffer() {
    for (uint32_t i = 0; i < BUFFER_SIZE; i++) {
        spi_slave_tx_buf[i] = (0xFF - i) & 0xFF;
    }
    memset(spi_slave_rx_buf, 0, BUFFER_SIZE);
}

void setup() {
    Serial.begin(115200);
	Serial.println("Starting ESP32 Slave test");
    delay(2000);

    slave.setDataMode(SPI_MODE0);
    slave.begin();

    set_buffer();
}

void loop() {
    
    // if there is no transaction in queue, add transaction
    if (slave.remained() == 0) {
        slave.queue(spi_slave_rx_buf, spi_slave_tx_buf, BUFFER_SIZE);
    }


    while (slave.available()) {
        Serial.println("Read: " + String(slave.available()));
        // // show received data
        // for (size_t i = 0; i < slave.size(); ++i) {
        //     printf("%d ", spi_slave_rx_buf[i]);
        // }
        // printf("\n");
       

        // set_buffer();

        // // for(int i = 0; i < BUFFER_SIZE; i++)
        // // {
        // //     Serial.print(spi_slave_tx_buf[i]);
        
        // // Serial.println();

        slave.pop();
    }

}
