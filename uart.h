//
// Created by T460s on 12.1.2024.
//

#ifndef JVD_PROJECT_UART_H
#define JVD_PROJECT_UART_H

#include <string.h>
#include "pico/stdlib.h"
#include "hardware/uart.h"
#include "ring_buffer.h"

int uart_read(int uart_nr, uint8_t *buffer, int size);
int uart_write(int uart_nr, const uint8_t *buffer, int size);
int uart_send(int uart_nr, const char *str);
void uart_setup (int uart_num, int tx_pin, int rx_pin, int speed);

#endif //JVD_PROJECT_UART_H
