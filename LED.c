#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/pwm.h"

volatile bool leds_on = false;

void toggle_leds(uint16_t gpio, uint16_t level) {
    if(!leds_on) {
        pwm_set_gpio_level(gpio, level);
        leds_on = true;
    }
    else {
        pwm_set_gpio_level(gpio, 0);
        leds_on = false;
    }

}