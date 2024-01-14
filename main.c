#include "main.h"

extern volatile int turns_done;

int main(void) {
    uint8_t program_state = 0;
    uint8_t rotor_running = 0;
    uint8_t pills_left = 7;
    pgstate programstate;

    stdio_init_all();
    initialize_gpios(GPIO_PULL_UP, GPIO_IN, 0, "BTN", 1, BUTTON_PIN);
    initialize_gpios(GPIO_PULL_DOWN, GPIO_OUT, 0, "LED", 1, LED_PIN);

    uint8_t buffer[4];
    read_from_eeprom(ROTOR_RUNNING_ADDRESS, buffer, 4);
    if (!lorawan_connection() || !connect_to_server()) exit(-1);

    programstate.state = buffer[0];
    programstate.not_state = buffer[1];

    if (validate_mem(&programstate)) {
        rotor_running = buffer[0];
        program_state = buffer[1];
        turns_done = buffer[2];
        pills_left = buffer[3];
    }
    else {
        printf("Previous state not found. Commence operation Tabula Rasa.");
    }

    // rotor initialize + positioning!!
    rotor_startup();

    printf("Program starts\n");

    while(true) {
        switch(program_state) {
            case (0):
                if (!gpio_get(BUTTON_PIN)) {
                    while (!gpio_get(BUTTON_PIN)) {
                        sleep_ms(20);
                    }
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
                break;
            case (2):
                if (!gpio_get(BUTTON_PIN)) {
                    while (!gpio_get(BUTTON_PIN)) {
                        sleep_ms(50);
                    }
                    program_state = 3;
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
                break;
        }

        // at the end of every loop write information to EEPROM and LoRa
        set_pg_state(&programstate, program_state);

        uint8_t data[5] = {
                programstate.state,
                programstate.not_state,
                rotor_running,
                turns_done,
                pills_left
        };

        char msg[256];
        sprintf(msg, "Program state: %d"
                     "Rotor running: %d"
                     "Turns done: %d"
                     "Pills left: %d", programstate.state, rotor_running, turns_done, pills_left);

        write_to_eeprom(PROGRAM_STATE_ADDRESS, data, 5);
        speak_to_server(msg);

    }
    return 0;
}