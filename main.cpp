#include <stdio.h>
#include <stdint.h>

#include "pico/stdlib.h"
#include "bgt60/bgt60ltr11XXX_driver.hpp"

BGT60::BGT60_DRIVER driver;

void init() {
    stdio_init_all();
    driver.initialize_device();
}

int main() {
    init();
    printf("START OF DEMO...\n");
    for (;;) {
        printf("Looping...\n");
        driver.setRegisterField(BGT60::REGISTER_ADDRESS::REG34, BGT60::REGISTER_FIELDS::REG34::ADC_ED, 1);
        sleep_ms(100);
    }
    return 0;
}
