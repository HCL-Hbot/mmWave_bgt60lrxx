#ifndef BGT60LTR11XXX_DRIVER_HPP
#define BGT60LTR11XXX_DRIVER_HPP

#include <type_traits> //for std::underlying_type
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
    // Section 2.2.5 SPI mode sequence used: 
    static void initialize_device() {
        REGISTER_ADDRESS reg15 = REGISTER_ADDRESS::REG15;

        // 1 Init for SPI mode
        clearRegisterField(REGISTER_ADDRESS::REG1, REGISTER_FIELDS::REG1::QS_RD_EN); // Datasheet table 11.
        setRegisterField(reg15, REGISTER_FIELDS::REG15::START_PM, 1); // Rising edge triggers CW mode (if bit 12 is set as well)
        setRegisterField(reg15, REGISTER_FIELDS::REG15::START_CW, 1); //Changes behavior of bit 14 (“start_pm”) to start CW mode instead of pulsed mode (both can be set in same SPI-access)

        // 2 Enabling ADC 
        EnableADC();
    }

    // Functions to interact with the hardware
    static void setRegisterField(REGISTER_ADDRESS registerAddress, const RegisterField& field, uint16_t value) {
        uint16_t regValue = spi_read_register(static_cast<uint8_t>(registerAddress));
        regValue = (regValue & ~(field.mask << field.shift)) | ((value & field.mask) << field.shift);
        spi_write_register(static_cast<uint8_t>(registerAddress), regValue);
    }

    static void clearRegister(REGISTER_ADDRESS registerAddress) {
        spi_write_register(static_cast<uint8_t>(registerAddress), 0x0000);
    }

    static void clearRegisterField(REGISTER_ADDRESS registerAddress, const RegisterField& field) {
        uint16_t regValue = spi_read_register(static_cast<uint8_t>(registerAddress));
        regValue &= ~(field.mask << field.shift);
        spi_write_register(static_cast<uint8_t>(registerAddress), regValue);
    }

    static uint16_t getRegisterField(REGISTER_ADDRESS registerAddress, const RegisterField& field) {
        uint8_t regAddr = static_cast<uint8_t>(registerAddress);
        uint16_t regValue = spi_read_register(regAddr);
        return getField(regValue, field);
    }

protected:

    static void EnableADC() {
        REGISTER_ADDRESS reg34 = REGISTER_ADDRESS::REG34;
        setRegisterField(reg34, REGISTER_FIELDS::REG34::BANDGAP_EN, 1);
        setRegisterField(reg34, REGISTER_FIELDS::REG34::ADC_CLK_EN, 1);
        
        uint16_t bandgap_up_flag = 0;
        do {
            bandgap_up_flag = getRegisterField(reg34, REGISTER_FIELDS::REG34::BANDGAP_EN);
        } while (bandgap_up_flag != 1);

        setRegisterField(reg34, REGISTER_FIELDS::REG34::ADC_ED, 1);
    }

    static uint8_t DisableADC() {
        REGISTER_ADDRESS reg34 = REGISTER_ADDRESS::REG34;
        uint16_t adc_status = getRegisterField(reg34, REGISTER_FIELDS::REG34::ADC_ED);

        if (adc_status) {
            setRegisterField(reg34, REGISTER_FIELDS::REG34::ADC_ED, 0);
            setRegisterField(reg34, REGISTER_FIELDS::REG34::ADC_CLK_EN, 0);
            return 0; // ADC turned off.
        }
        return 1; // ADC is not on.
    }

    static void StartAD_ChannelConversion(ADC_REG_CHANNELS channel) {
        REGISTER_ADDRESS reg36 = REGISTER_ADDRESS::REG36;
        uint8_t adc_ready_flag = 0;
        uint8_t channel_integer = static_cast<uint8_t>(channel);

        do {
            adc_ready_flag = getRegisterField(reg36, REGISTER_FIELDS::REG36::ADC_READY);
        } while (adc_ready_flag != 1); 

        setRegisterField(REGISTER_ADDRESS::REG35, REGISTER_FIELDS::REG35::CHNR_ALL, channel_integer);
        setRegisterField(REGISTER_ADDRESS::REG35, REGISTER_FIELDS::REG35::CHNR_ALL, 0);
    }

    static inline uint16_t ReadAD_ChannelResult(ADC_REG_CHANNELS channel) {
        uint16_t result_ready_flag = getRegisterField(REGISTER_ADDRESS::GSR0, REGISTER_FIELDS::GSPR0::ADC_RESULT_READY);
        if (result_ready_flag) {
            const uint8_t result_reg = getAdcRegisterAddress(channel);
            uint16_t channel_result = getRegisterField(static_cast<REGISTER_ADDRESS>(result_reg), REGISTER_FIELDS::ADC_REGS::ADC_RESULT);
            return channel_result;
        }
        return 0; // Default for channel result not read.
    }

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

    static void spi_write_register(const uint8_t reg, const uint16_t value) {
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
        const uint8_t regAddr = (reg & 0x7F) | 0x80;
        const uint8_t data_to_send[3] = {regAddr, 0x00, 0x00};
        uint8_t received_data[2] = {0, 0};

        gpio_put(SPI_CS, 0);
        spi_write_blocking(SPI_PORT, data_to_send, 1);
        spi_read_blocking(SPI_PORT, 0, received_data, 2);
        gpio_put(SPI_CS, 1); 

        return (received_data[0] << 8) | received_data[1];
    }

    /* Helper functions for field manipulation*/
    static inline constexpr uint16_t setField(uint16_t reg, RegisterField field, uint16_t value) {
        return (reg & ~field.mask) | ((value << field.shift) & field.mask);
    }

    static inline constexpr uint16_t getField(uint16_t reg, RegisterField field) {
        return (reg & field.mask) >> field.shift;
    }
};
} // End of BGT60 namespace. 
#endif // BGT60LTR11XXX_DRIVER_HPP