//
// Created by T460s on 12.1.2024.
//

#ifndef JVD_PROJECT_LORAWAN_H
#define JVD_PROJECT_LORAWAN_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdint.h>
#include <stdbool.h>

#define STRLEN 80


// We are using pins 0 and 1, but see the GPIO function select table in the
// datasheet for information on which other pins cadn be used.
#if 0
#define UART_NR 0
#define UART_TX_PIN 0
#define UART_RX_PIN 1
#else
#define UART_NR 1
#define UART_TX_PIN 4
#define UART_RX_PIN 5
#endif

#define BAUD_RATE 9600
#define RESPONSE_AT "+AT: OK"

void lorawan_connection (void);
void connect_to_server (void);
void speak_to_server (const char *str);
void sending_process (int uart_num, const char *str);
void responding_process (void);
void check_connection (void);
void message_adaption (char** result, const char* str);

#endif //JVD_PROJECT_LORAWAN_H
