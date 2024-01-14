#include "header.h"

int main(void) {
//    stdio_init_all();
//    // parametrit: gpio-tyyppi (tälä hetkel vaan "BTN" tai "LED", gpio-määrä, gpio:t (eli voi inittaa monta samanlaista kerralla)
//    initialize_gpios("BTN", 1, BUTTON_PIN);
//    initialize_gpios("LED", 1, LED_PIN);

    // rotor initialize + positioning!!
//    rotor_startup();

//    printf("Program starts\n");

//    while(true) {
//        switch(program_state) {
//            case (0):
//                if (!gpio_get(BUTTON_PIN)) {
//                    while (!gpio_get(BUTTON_PIN)) {
//                        sleep_ms(50);
//                    }
//                    variable_reset();
//                    position_calib();
//                    printf("case 0\n");
//                    program_state = 1;
//                }
//                break;
//            case (1):
//                printf("case 1\n");
//                calibration();
//                program_state = 2;
//                break;
//            case (2):
//                if (!gpio_get(BUTTON_PIN)) {
//                    while (!gpio_get(BUTTON_PIN)) {
//                        sleep_ms(50);
//                    }
//                    program_state = 3;
//                }
//                break;
//            case (3): {
//                printf("case 3\n");
//                if (turns_done < 7) {
//                    turns_done++;
//                    turn_divider();
//                    sleep_ms(TURN_DIVIDER_TIMER_MS);
//                }
//                else {
//                    printf("Full revolution done, all pills dispensed.\n");
//                    stop_ABCD();
//                    program_state = 0;
//                }
//            }
//            break;
//            case (4):
//                printf("case 4\n");
//                stop_ABCD();
//                break;
//            case (5):
//                break;
//        }
//    }
//    uint8_t mask = 10;
//    write_to_eeprom(mask);

//    printf("%d", i2c_get_read_available(i2c0));

    return 0;
}