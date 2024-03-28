#include <stdio.h>
#include <stdint.h>

#include "pico/stdlib.h"
#include <bgt60ltr11XXX_driver.hpp>

BGT60::BGT60_DRIVER driver;

void init() {
    stdio_init_all();
}

int main() {
    init();
    printf("START OF DEMO...\n");

    driver.configureRadar();

    driver.getRegisterField(BGT60::REGISTER_ADDRESS::REG56, BGT60::REGISTER_FIELDS::REG56::CHIP_VERSION);
    
    for (;;) {
        printf("Looping...\n");
        // driver.setRegisterField(BGT60::REGISTER_ADDRESS::REG10,BGT60::REGISTER_FIELDS::REG10::HOLD,1);
        // driver.getVariable_I();
        // driver.getVariable_Q();
        sleep_ms(100);
    }
    return 0;
}   


