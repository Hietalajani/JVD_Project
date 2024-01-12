#include "header.h"
#include "rotator.c"
#include "rotator.h"

// TODO:
//  MAke button failproof.
//  Need to remove led on/off from button press.
//  Piezo sensor takes interrupt from pressin button, idk why. Fix.
//  Variables to save in E-EPROM: turns_done, program_state, current_steps_taken, rotor_state?(might be useless)
//  Detect reset.
//  After reset: set program_state to new case where we reset the current program where it was.
//  After program catch up: set the program_state to case where the program was running.
//  LoraWan, where are we? Set up server/mosqq(at school) via Lorawan to sent current status of program.(idk whats the meaning of this.)
//  set dispenser that it turns exact 30s or the w/e time we set, atm its going with sleep_ms. not gud.
//



int main(void) {
    // parametrit: gpio-tyyppi (tälä hetkel vaan "BTN" tai "LED", gpio-määrä, gpio:t (eli voi inittaa monta samanlaista kerralla)
    initialize_gpios("BTN", 1, BUTTON_PIN);
    initialize_gpios("LED", 1, LED_PIN);

    // rotor initialize + positioning!!!
    rotor_startup();
    stdio_init_all();

    printf("Program starts\n");
    toggle_leds(LED_PIN, PWM_FREQUENCY);

    while(true) {
        switch(program_state) {
            case (0):
                toggle_leds(LED_PIN, PWM_FREQUENCY);
                sleep_ms(150);
                if (!gpio_get(BUTTON_PIN)) {
                    while (!gpio_get(BUTTON_PIN)) {
                        sleep_ms(50);
                    }
                    toggle_leds(LED_PIN, 0);
                    variable_reset();
                    position_calib();
                    printf("case 0\n");
                    program_state = 1;
                }
                break;
            case (1):
                printf("case 1\n");
                calibration();
                program_state = 2;
                toggle_leds(LED_PIN, PWM_FREQUENCY);
                break;
            case (2):
                if (!gpio_get(BUTTON_PIN)) {
                    while (!gpio_get(BUTTON_PIN)) {
                        sleep_ms(50);
                    }
                    program_state = 3;
                    toggle_leds(LED_PIN, 0);
                }
                break;
            case (3): {
                printf("case 3\n");
                if (turns_done < 7) {
                    turns_done++;
                    turn_divider();
                    sleep_ms(TURN_DIVIDER_TIMER_MS);
                }
                else {
                    printf("Full revolution done, all pills dispensed.\n");
                    stop_ABCD();
                    program_state = 0;
                }
            }
            break;
            case (4):
                printf("case 4\n");
                stop_ABCD();
                break;
            case (5):
                // When reset happens we come here.
                break;
        }
    }
}