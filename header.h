//
// Created by hiets on 18.12.2023.
//

#ifndef JVD_PROJECT_HEADER_H
#define JVD_PROJECT_HEADER_H

// libs
#include <stdbool.h>
#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include "pico/stdlib.h"
#include "hardware/pwm.h"
#include "hardware/clocks.h"

// c files
//#include "main.c"
//#include "EEPROM.c"
//#include "LED.c"
//#include "LoRaWAN.c"
//#include "rotator.c"
//#include "ring_buffer.c"
//#include "gpio_init_file.c"

// define pins
#define LED_PIN 21
#define BUTTON_PIN 12
#define STR_LEN 256
#define UART_NR 1
#define RX_PIN 5
#define TX_PIN 4

// define global values
#define BAUD_RATE 9600
#define DEBOUNCE_TIME 20
#define PWM_FREQUENCY 1000
#define MHZ_1 1000000UL
#define MAX_LEVEL 999
#define DUTY_STEPS 100

// declare functions
//gpio_init_file.c
void initialize_gpios(const char *type, int nr, ...);
void handler(uint gpio, uint32_t eventmask);
pwm_config pwm_get_config_struct();
void toggle_leds(uint16_t gpio, uint16_t level);


#endif //JVD_PROJECT_HEADER_H