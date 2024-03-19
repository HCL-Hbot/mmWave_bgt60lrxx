#ifndef BGT60LTR11XXX_REGS_HPP
#define BGT60LTR11XXX_REGS_HPP

#include <stdint>

namespace BGT60 {

typedef struct {
    uint16_t clkFrequencyKHz;
    uint8_t bitsPerTransfer;
} SPIConfig_t;

enum class REGISTER_ADDRESS : uint8_t {
    REG1    = 0x01,     /* Direct Access Register */
    REG2    = 0x02,     /* Threshold Configuration */
    REG4    = 0x04,     /* PLL Configuration 1 */
    REG5    = 0x05,     /* PLL Configuration 2 */
    REG6    = 0x06,     /* PLL Configuration 3 */
    REG7    = 0x07,     /* Duty Cycling Timing PD MPA */
    REG8    = 0x08,     /* Divider Configuration */
    REG9    = 0x09,     /* Baseband Configuration */
    REG10   = 0x0A,     /* Hold Time Configuration */
    REG12   = 0x0C,     /* BITE (Built-In Test Equipment) */
    REG13   = 0x0D,     /* Algorithm Configuration 1 */
    REG14   = 0x0E,     /* Algorithm Configuration 2 */
    REG15   = 0x0F,     /* Digital Control */
    REG34   = 0x22,     /* ADC Start */
    REG35   = 0x23,     /* ADC Convert */
    REG36   = 0x24,     /* ADC Status */
    REG56   = 0x38,     /* Status and Chip Version */
    GSR0    = 0x39      /* SPI Status Register - Datasheet does not explicitly state this. */
};

} // End of BGT60 namespace. 
#endif // BGT60LTR11XXX_REGS_HPP