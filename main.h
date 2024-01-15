//
// Created by hiets on 12.1.2024.
//

#ifndef JVD_PROJECT_MAIN_H
#define JVD_PROJECT_MAIN_H

// libs
#include <stdbool.h>
#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include "pico/stdlib.h"
#include "hardware/pwm.h"
#include "hardware/clocks.h"
#include "EEPROM.h"
#include "gpio_init.h"
#include "rotator.h"
#include "LoRaWAN.h"
#include "time.h"

// define pins
#define LED_PIN 21
#define BUTTON_PIN 12
#define UART_NR 1

// define global values

#define DEBOUNCE_TIME 20
#define PWM_FREQUENCY 1000
#define MHZ_1 1000000UL

// declare functions
//gpio_init_file.c

#endif //JVD_PROJECT_MAIN_H
