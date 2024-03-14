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
    ADDRESS_BYTE_SIZE   = 1,
    PAYLOAD_BYTE_SIZE   = 2,
};

void init() {
    stdio_init_all();

    // Initialize SPI inteface: Reference User Manual: 3.1.1
    spi_init(SPI_PORT, static_cast<uint16_t>(SPI_SETTINGS::CLK_FREQUENCY_KHZ)*1000);
    spi_set_format(SPI_PORT, 
        static_cast<uint16_t>(SPI_SETTINGS::PAYLOAD_BYTE_SIZE), 
        SPI_CPOL_0, // Clock low in idle.
        SPI_CPHA_0, // CLock phase rising. 
        SPI_MSB_FIRST); 

    gpio_set_function(SPI_SCK, GPIO_FUNC_SPI);
    gpio_set_function(SPI_MOSI, GPIO_FUNC_SPI);
    gpio_set_function(SPI_MISO, GPIO_FUNC_SPI);
    
    // Initialize CS pin
    gpio_init(SPI_CS);
    gpio_set_dir(SPI_CS, GPIO_OUT);
    gpio_put(SPI_CS, 1); // Pull up
}

int main() {
    init();

    for (;;) {
    }
    return 0;
}
