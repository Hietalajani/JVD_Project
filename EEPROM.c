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

uint8_t write_to_eeprom(const uint8_t *address, const uint8_t *data, uint8_t len) {
    i2c_write_blocking(i2c0, EEPROM_ADDR, address, 2, true);
    uint8_t ret = i2c_write_blocking(i2c0, EEPROM_ADDR, data, len, false);
    sleep_ms(20);
    return ret;
}

void read_from_eeprom(const uint8_t *address, uint8_t *buffer, const uint8_t len) {
    i2c_write_blocking(i2c0, EEPROM_ADDR, address, 2, true);
    i2c_read_blocking(i2c0, EEPROM_ADDR, buffer, len, false);
}