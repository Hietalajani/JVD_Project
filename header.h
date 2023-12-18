//
// Created by hiets on 18.12.2023.
//

#ifndef JVD_PROJECT_HEADER_H
#define JVD_PROJECT_HEADER_H

// c files
#include "gpio_init_file.c"
#include "EEPROM.c"
#include "LED.c"
#include "LoRaWAN.c"
#include "rotator.c"
#include "ring_buffer.c"


// define pins
#define LED_PIN 21
#define BUTTON_PIN 12
#define STR_LEN 256
#define UART_NR 1
#define RX_PIN 5
#define TX_PIN 4
#define BAUD_RATE 9600
#define KUCKUU 20
#endif //JVD_PROJECT_HEADER_H