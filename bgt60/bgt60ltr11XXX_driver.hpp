#ifndef BGT60LTR11XXX_DRIVER_HPP
#define BGT60LTR11XXX_DRIVER_HPP

#include "hardware/spi.h"
#include "bgt60ltr11XXX_regs.hpp"

// Define SPI port
#define SPI_PORT    spi0
#define SPI_SCK     2  // SPI Clock
#define SPI_MOSI    3  // Master Out Slave In (MOSI)
#define SPI_MISO    4  // Master In Slave Out (MISO)
#define SPI_CS      5  // Chip Select (CS)

namespace BGT60 {

class BGT60_DRIVER {
public:
    BGT60_DRIVER() {
        initialize_spi_interface();
    }
    
    ~BGT60_DRIVER() {}

    // Initialize the BGT60LTR11AIP device
    void initialize_device() {
        initialize_spi_interface();
    }

    // Functions to interact with the hardware
    static void setRegisterField(REGISTER_ADDRESS registerAddress, const RegisterField& field, uint16_t value) {
        uint16_t regValue = 0x0000; // spi_read_register(static_cast<uint8_t>(registerAddress));
        regValue = (regValue & ~(field.mask << field.shift)) | ((value & field.mask) << field.shift);
        spi_write_register(static_cast<uint8_t>(registerAddress), regValue);
    }

    // static uint32_t getRegisterField(REGISTER_ADDRESS registerAddress, const RegisterField& field) {
    //     uint32_t regAddr = static_cast<uint32_t>(registerAddress);
    //     uint32_t regValue = readRegister(regAddr);
    //     return getField(regValue, field);
    // }

private:
    /* CLASS VARIABLES: */
    static inline SPIConfig_t spi_config = {1, 8};

    /* CLASS FUNCTIONS: */
    // Initialize SPI inteface: Reference User Manual: 3.1.1
    static void initialize_spi_interface() {
        const uint8_t dataFieldSize = spi_config.bitsPerTransfer;
        const uint32_t busFrequency = spi_config.clkFrequencyMHz*1'000'000;

        spi_init(SPI_PORT, busFrequency);
        spi_set_format(SPI_PORT, 
            dataFieldSize, 
            SPI_CPOL_0,         // Clock low in idle.
            SPI_CPHA_0,         // CLock phase rising. 
            SPI_MSB_FIRST);

        gpio_set_function(SPI_MISO, GPIO_FUNC_SPI);
        gpio_set_function(SPI_SCK, GPIO_FUNC_SPI);
        gpio_set_function(SPI_MOSI, GPIO_FUNC_SPI);

        // Initialize CS pin
        gpio_init(SPI_CS);
        gpio_set_dir(SPI_CS, GPIO_OUT);
        gpio_put(SPI_CS, 1);
    }

    static inline void spi_write_register(const uint8_t reg, const uint16_t value) {
        uint8_t data_to_send[3];
        
        // 7-bit address + RW bit set to 1 for write operation
        data_to_send[0] = (reg & 0x7F) | 0x80;
        // 16-bit payload sent MSB first
        data_to_send[1] = (value >> 8) & 0xFF; // MSB of the value to write
        data_to_send[2] = value & 0xFF;        // LSB of the value to write

        gpio_put(SPI_CS, 0);
        spi_write_blocking(SPI_PORT, data_to_send, 3);
        gpio_put(SPI_CS, 1);
    }

    const static uint16_t spi_read_register(uint8_t reg) {
        const uint8_t regAddr = (reg << 0x01) & 0xFE;
        const uint8_t data_to_send[3] = {regAddr, 0x00, 0x00};
        uint8_t received_data[2] = {0, 0};

        gpio_put(SPI_CS, 0);
        spi_write_blocking(SPI_PORT, data_to_send, 1);
        spi_read_blocking(SPI_PORT, 0, received_data, 2);
        gpio_put(SPI_CS, 1); 

        return (received_data[0] << 8) | received_data[1];
    }

    // Helper functions for field manipulation
    static constexpr uint16_t setField(uint16_t reg, RegisterField field, uint16_t value) {
        return (reg & ~field.mask) | ((value << field.shift) & field.mask);
    }

    static constexpr uint16_t getField(uint16_t reg, RegisterField field) {
        return (reg & field.mask) >> field.shift;
    }
};
} // End of BGT60 namespace. 
#endif // BGT60LTR11XXX_DRIVER_HPP