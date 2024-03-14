#include "pico/stdlib.h"
#include "hardware/spi.h"
#include <stdint.h>

// Define SPI port
#define SPI_PORT    spi0
#define SPI_SCK     2  // SPI Clock
#define SPI_MOSI    3  // Master Out Slave In (MOSI)
#define SPI_MISO    4  // Master In Slave Out (MISO)
#define SPI_CS      5  // Chip Select (CS)

enum class SPI_SETTINGS : uint16_t {
    CLK_FREQUENCY_KHZ   = 500,
    BITS_PER_TRANSFER   = 8,
};

// Initialize SPI inteface: Reference User Manual: 3.1.1
void spi_init() {
    const static uint8_t dataFieldSize = static_cast<uint8_t>(SPI_SETTINGS::BITS_PER_TRANSFER);
    const static uint16_t busFrequency = static_cast<uint16_t>(SPI_SETTINGS::CLK_FREQUENCY_KHZ);

    spi_init(SPI_PORT, busFrequency);
    spi_set_format(SPI_PORT, 
        dataFieldSize, 
        SPI_CPOL_0, // Clock low in idle.
        SPI_CPHA_0, // CLock phase rising. 
        SPI_MSB_FIRST); 

    gpio_set_function(SPI_MISO, GPIO_FUNC_SPI);
    gpio_set_function(SPI_SCK, GPIO_FUNC_SPI);
    gpio_set_function(SPI_MOSI, GPIO_FUNC_SPI);

    // Initialize CS pin
    gpio_init(SPI_CS);
    gpio_set_dir(SPI_CS, GPIO_OUT);
    gpio_put(SPI_CS, 1);
}

void spi_write_register(uint8_t reg, uint16_t value) {
    uint8_t data_to_send[3];
    data_to_send[0] = reg << 1 | 1;
    data_to_send[1] = (value >> 8) & 0xFF;
    data_to_send[2] = value & 0xFF;

    gpio_put(SPI_CS, 0);
    spi_write_blocking(SPI_PORT, data_to_send, 3);
    gpio_put(SPI_CS, 1);
}

uint16_t spi_read_register(uint8_t reg) {
    const uint8_t regAddr = (reg << 0x01) & 0xFE;
    const uint8_t data_to_send[3] = {regAddr, 0x00, 0x00};
    uint8_t received_data[2] = {0, 0};

    gpio_put(SPI_CS, 0);
    spi_write_blocking(SPI_PORT, data_to_send, 1);
    spi_read_blocking(SPI_PORT, 0, received_data, 2);
    gpio_put(SPI_CS, 1); 

    return (received_data[0] << 8) | received_data[1];
}

void init() {
    stdio_init_all();
}

int main() {
    init();
    
    spi_write_register(5, 350);
    spi_read_register(6);

    for (;;) {
    }
    return 0;
}
