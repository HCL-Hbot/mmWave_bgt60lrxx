#ifndef BGT60LTR11XXX_REGS_HPP
#define BGT60LTR11XXX_REGS_HPP

#include <stdint>

namespace BGT60 {

typedef struct {
    uint16_t clkFrequencyKHz;
    uint8_t bitsPerTransfer;
} SPIConfig_t;

struct RegisterField {
    uint16_t mask;
    uint16_t shift;
};

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

struct REGISTER_FIELDS {
    struct Reg0 {
        static constexpr RegisterField RXBUF_EN         = {0x0001, 0};
        static constexpr RegisterField LNA_EN           = {0x0002, 0};
        static constexpr RegisterField MIXI_EN          = {0x0004, 0};
        static constexpr RegisterField MIXQ_EN          = {0x0008, 0};
        static constexpr RegisterField TXBUF_EN         = {0x0010, 0};
        static constexpr RegisterField MPA_EN           = {0x0020, 0};

        static constexpr RegisterField PLL_EN           = {0x0001, 8};
        static constexpr RegisterField PLL_ACTIVE       = {0x0002, 8};
        static constexpr RegisterField PLL_CLK_GATE_EN  = {0x0004, 8};
        static constexpr RegisterField PLL_OPEN_LOOP    = {0x0008, 8};
        static constexpr RegisterField VCO_EN           = {0x0010, 8};
        static constexpr RegisterField VCOBUF_EN        = {0x0020, 8};
    };

    struct REG1 {
        static constexpr RegisterField DIV_BIAS_END     = {0x0001, 13};
        static constexpr RegisterField LNA_EN           = {0x0001, 9};
        static constexpr RegisterField QS_RD_EN         = {0x0001, 8};
        static constexpr RegisterField BB_DIG_DET_EN    = {0x0001, 7};
        // Reserved.
        static constexpr RegisterField BB_BOOST_DIS     = {0x0001, 5}; 
        static constexpr RegisterField BB_CLK_C_HOP_EN  = {0x0001, 4};
        // Reserved.
        static constexpr RegisterField BB_STRUP_HP      = {0x0001, 2}; 
        static constexpr RegisterField BB_AMP_EN        = {0x0001, 1}; 
        static constexpr RegisterField BB_SAMPLE_EN     = {0x0001, 0};  
    };

    struct REG2 {
        static constexpr RegisterField HPRT             = {0x0001, 15};
        static constexpr RegisterField APRT             = {0x0001, 14};
        static constexpr RegisterField DIR_MODE         = {0x0001, 13};
        // Reserved.
        static constexpr RegisterField THRS             = {0x1FFF, 0};  
    };

    struct REG3 {
        static constexpr RegisterField Reserved         = {0xFFFF, 0};  
    };

    struct REG4 {
        static constexpr RegisterField PLL_DFT_DMUX     = {0x00C0, 8};
        // Reserved.
        static constexpr RegisterField PLL_BIAS_DIS     = {0x0001, 12};
        static constexpr RegisterField PLL_LF_ISO       = {0x0001, 11};
        static constexpr RegisterField PLL_LF_R4_SEL    = {0x0001, 10};
        static constexpr RegisterField PLL_CL_LOOP_PMODE= {0x0001, 9};
        static constexpr RegisterField PLL_LF_R2_SEL    = {0x0001, 8};
        static constexpr RegisterField XOSC_MODE        = {0x0001, 7};
        static constexpr RegisterField PLL_FBDIV_CNT    = {0x0001, 6};  
        static constexpr RegisterField PLL_CP_ICP_SEL   = {0x0038, 0};
        static constexpr RegisterField PLL_CP_MODE      = {0x0001, 14};
        static constexpr RegisterField PLL_PFD_RDT_SEL  = {0x0003, 0};
    };

    struct Reg5 {
        // Assuming pll_fcw occupies bits 0 to 7
        static constexpr RegisterField PLL_FCW = {0x0FFF, 0};
    };

    struct Reg6 {
        static constexpr RegisterField PLL_LD_TW_SEL    = {0xE000, 0};
        static constexpr RegisterField PLL_LD_LEN       = {0x0010, 8};
        static constexpr RegisterField PLL_LD_EN        = {0x0008, 8};
    };

    struct Reg7 {
        static constexpr RegisterField VCO2PLL_DLY      = {0x0001, 6};
        static constexpr RegisterField MPA2SH_DLY       = {0x0003, 4};
        static constexpr RegisterField PD_EN            = {0x0001, 3};
        static constexpr RegisterField MPA_CTRL         = {0x0007, 0};
    };

    struct Reg8 {
        static constexpr RegisterField DIV_SEL = {0x0003, 2};
        static constexpr RegisterField DIV_OUT_EN = {0x0002, 1};
        static constexpr RegisterField DIV_TEST_MODE_EN = {0x0001, 0};
    };

    struct Reg9 {
        static constexpr RegisterField BB_HP_RES        = {0x0001, 8};
        static constexpr RegisterField BB_CLK_CHOP_SEL  = {0x0001, 5};
        static constexpr RegisterField BB_LPF_BW        = {0x0001, 4};
        static constexpr RegisterField BB_CTRL_GAIN     = {0x000F, 0};
    };

    struct Reg10 {
        static constexpr RegisterField HOLD = {0xFFFF, 0};
    };


}
} // End of BGT60 namespace. 
#endif // BGT60LTR11XXX_REGS_HPP