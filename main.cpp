#include <stdio.h>
#include <stdint.h>

#include "pico/stdlib.h"
#include "bgt60/bgt60ltr11XXX_driver.hpp"

void init() {
    stdio_init_all();
    BGT60::BGT60_DRIVER driver;
}

int main() {
    init();
    printf("START OF DEMO...\n");

    for (;;) {
        printf("Looping...\n");

        sleep_ms(100);
    }
    return 0;
}
