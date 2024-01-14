//
// Created by hiets on 12.1.2024.
//

#ifndef JVD_PROJECT_GPIO_INIT_H
#define JVD_PROJECT_GPIO_INIT_H

#include "LED.h"
#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include "pico/stdlib.h"
#include "hardware/pwm.h"
#include "hardware/clocks.h"

#define LED_PIN 21
#define BUTTON_PIN 12
#define DEBOUNCE_TIME 20
#define PWM_FREQUENCY 1000
#define MHZ_1 1000000UL
#define GPIO_PULL_UP 1
#define GPIO_PULL_DOWN 0
#define GPIO_LED 1
#define GPIO_NOT_LED 0

void initialize_gpios(bool up, bool out, bool input, const char *type, int nr, ...);

#endif //JVD_PROJECT_GPIO_INIT_H
