#include "EEPROM.h"

bool init_eeprom() {
    bool succ;
    succ = i2c_init(i2c0, BAUD_RATE);
    gpio_set_function(17, GPIO_FUNC_I2C);
    gpio_set_function(16, GPIO_FUNC_I2C);
    gpio_pull_up(17);
    gpio_pull_up(16);
    return succ;
}

uint8_t write_to_eeprom(const uint8_t address1, const uint8_t address2, const uint8_t *data, uint8_t len) {
    uint8_t address[MAX_BYTES] = {address1, address2};
    for (int i = 0; i < len; i++) address[i+2] = data[i];
    uint8_t ret = i2c_write_blocking(i2c0, EEPROM_ADDR, address, len+2, false);
    sleep_ms(5);
    return ret;
}

void read_from_eeprom(const uint8_t address1, const uint8_t address2, uint8_t *buffer, const uint8_t len) {
    uint8_t address[2] = {address1, address2};
    i2c_write_blocking(i2c0, EEPROM_ADDR, address, 2, true);
    i2c_read_blocking(i2c0, EEPROM_ADDR, buffer, len, false);
}

void set_pg_state(pgstate *ps, uint8_t value)
{
    ps->state = value;
    ps->not_state = ~value;
}

bool validate_mem(pgstate *ps) {
    return ps->state == (uint8_t) ~ps->not_state;
}