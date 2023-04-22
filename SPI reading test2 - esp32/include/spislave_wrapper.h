#pragma once

#include <Arduino.h>
#include <ESP32SPISlave.h>

#define S_SCK 32
#define S_CS 33
#define S_MOSI 12
#define S_MISO 13

// https://github.com/hideakitai/ESP32SPISlave
// https://forum.arduino.cc/t/how-to-receive-data-via-spi/251768

ESP32SPISlave slave;

static constexpr uint32_t BUFFER_SIZE{32};
uint8_t spi_slave_tx_buf[BUFFER_SIZE];
uint8_t spi_slave_rx_buf[BUFFER_SIZE];

namespace spislave
{

    //* Testing variables
    bool command_active = 0;
    uint8_t command = 0;

    void setup()
    {
        Serial.println("Setting up SPI slave.");

        // set SPI
        slave.setDataMode(SPI_MODE0);
        // use HSPI with our own pin assignment
        slave.begin(HSPI, S_SCK, S_MISO, S_MOSI, S_CS);

        // for safety - setting pins accordingly
        pinMode(S_SCK, INPUT);
        pinMode(S_MOSI, INPUT);

        pinMode(S_MISO, OUTPUT);

        //* Testing: Create custom value for tx buffer testing
        for (uint8_t i = 0; i < BUFFER_SIZE; i++)
        {
            spi_slave_tx_buf[i] = i;
        }
    }

    void printTxBuffer()
    {
        Serial.print("Tx buffer: ");
        for (int i = 0; i < BUFFER_SIZE; i++)
        {
            Serial.print(String(spi_slave_tx_buf[i]) + " ");
        }
        Serial.println();
    }

    void printRxBuffer()
    {
        Serial.print("Rx buffer: ");
        for (int i = 0; i < BUFFER_SIZE; i++)
        {
            Serial.print(String(spi_slave_rx_buf[i]) + " ");
        }
        Serial.println();
    }

    void poll()
    {
        // if there is no transaction in queue, add transaction
        if (slave.remained() == 0)
            slave.queue(spi_slave_rx_buf, spi_slave_tx_buf, BUFFER_SIZE);
    }

    uint8_t processTransaction()
    {
        command = 0; // reset current command
        // if transaction has completed from master, available() returns size of results of transaction and `spi_slave_rx_buf` is automatically updated
        while (slave.available())
        {
            command = spi_slave_rx_buf[0]; // retrieve sent command

            // check if not dummy command
            if (command != 0x00)
            {
                command_active = 1;

                // if command is for data requesting, set the requested data:
                // (the command essentially says: get ready - I will be requesting such and such data from you)
                for (uint8_t i = 0; i < 8; i++)
                {
                    spi_slave_tx_buf[i] = 237 + i;
                }
                spislave::printTxBuffer();
                Serial.println("\n");
            }
            // reset the transmittable data (the sendable data was already set in the previous transaction)
            else
            {
                command_active = 0; // set that no command is currently active
            
                for (uint8_t i = 0; i < 8; i++) // reset tx buffer to zeros
                {
                    spi_slave_tx_buf[i] = 0;
                }
            }

            slave.pop();                // pop the oldest transaction result
        }
        return command; // return received command
    }
    

}