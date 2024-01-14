//
// Created by hiets on 30.12.2023.
//

#ifndef JVD_PROJECT_LED_H
#define JVD_PROJECT_LED_H

#include <stdio.h>

#define LED_PIN 21
#define BUTTON_PIN 12
#define DEBOUNCE_TIME 20
#define PWM_FREQUENCY 1000
#define MHZ_1 1000000UL

void toggle_leds(uint16_t gpio, uint16_t level);
#endif //JVD_PROJECT_LED_H
