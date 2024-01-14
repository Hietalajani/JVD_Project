#ifndef JVD_PROJECT_EEPROM_H
#define JVD_PROJECT_EEPROM_H

#include <stdio.h>
#include <time.h>
#include "pico/stdlib.h"
#include "pico/binary_info.h"
#include "hardware/i2c.h"

typedef struct {
    uint8_t state;
    uint8_t not_state;
} pgstate;

#define BAUD_RATE 115200
#define MAX_BYTES 16

// EEPROM address register
#define EEPROM_ADDR 0x50
#define PROGRAM_STATE_ADDRESS 0, 0
#define NOT_PROGRAM_STATE_ADDRESS 0, 1
#define ROTOR_RUNNING_ADDRESS 0, 2
#define ROTOR_TURNS_ADDRESS 0, 3
#define PILLS_LEFT_ADDRESS 0, 4
#define MAX_ADDRESS 0x7F, 0xFF

// EEPROM lil uwu hexes
#define ROTOR_RUNNING 0x57 // that's a W
#define ROTOR_NOT_RUNNING 0x4C // common L
#define ROTOR_STATE

bool init_eeprom();
uint8_t write_to_eeprom(uint8_t address1, uint8_t address2, const uint8_t *data, uint8_t len);
void read_from_eeprom(uint8_t address1, uint8_t address2, uint8_t *buffer, uint8_t len);
bool validate_mem(pgstate *ps);
void set_pg_state(pgstate *ps, uint8_t value);

#endif //JVD_PROJECT_EEPROM_H