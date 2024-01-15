#include "main.h"

extern volatile int turns_done;
extern volatile uint8_t program_state;
extern volatile int current_steps_taken;
extern volatile uint8_t rotor_running;
volatile bool led_on = false;
volatile uint8_t old_program_state = 0;

int main(void) {
    uint8_t pills_left = 7;
    uint timer_start,timer_end,timer_dif = 0;
    pgstate programstate;

    stdio_init_all();
    init_eeprom();
    initialize_gpios(GPIO_PULL_UP, GPIO_IN, 0, "BTN", 1, BUTTON_PIN);
    initialize_gpios(GPIO_PULL_DOWN, GPIO_OUT, 0, "LED", 1, LED_PIN);

    uint8_t buffer[6];
    read_from_eeprom(PROGRAM_STATE_ADDRESS, buffer, 5);
    if (!lorawan_connection() || !connect_to_server()) { exit(-1); }

    programstate.state = buffer[0];
    programstate.not_state = buffer[1];

    for (int i = 0; i < 5; i++) printf("EEPROM %d: %d\n", i, buffer[i]);

    if (validate_mem(&programstate)) {
        rotor_running = buffer[2];
        program_state = buffer[0];
        turns_done = buffer[3];
        pills_left = buffer[4];
    }
    else {
        printf("Previous state not found. Commence operation Tabula Rasa.");
    }

    // rotor initialize + positioning!!
    rotor_startup();

//    printf("Program starts\n");

    while(true) {
        switch(program_state) {
            case (0):
                pwm_set_gpio_level(LED_PIN, led_on ? 0 : 500);
                led_on = !led_on;
                sleep_ms(500);
                break;
            case (1):
                pwm_set_gpio_level(LED_PIN, 0);
                variable_reset();
                position_calib();
                program_state = 2;
                old_program_state = 1;
                break;
            case (2):
                calibration();
                program_state = 3;
                old_program_state = 2;
                break;
            case (3):
                pwm_set_gpio_level(LED_PIN, 500);
                old_program_state = 3;
                if (!gpio_get(BUTTON_PIN)) while (!gpio_get(BUTTON_PIN)) sleep_ms(20);
                program_state = 4;
                break;
            case (4): {
                pwm_set_gpio_level(LED_PIN, 0);
                timer_end = clock();
                timer_dif = (timer_end-timer_start)/CLOCKS_PER_SEC;
                if(timer_dif>=TURN_DIVIDER_TIMER || timer_start==0) {
                    if (turns_done < 7) {
                        timer_start = clock();
                        turns_done++;
                        turn_divider();
                    }
                }
                else {
                    printf("Full revolution done, all pills dispensed.\n");
                    stop_ABCD();
                    program_state = 0;
                }
                old_program_state = 4;
                break;
            }
            case (5):
        }

        // at the end of every loop write information to EEPROM and LoRa
        if (program_state != old_program_state){
            set_pg_state(&programstate, program_state);

            uint8_t data[5] = {
                    programstate.state,
                    programstate.not_state,
                    rotor_running,
                    turns_done,
                    pills_left
            };

//            uint8_t data[5] = {
//                    0, 0, 0, 0, 0
//            };

            char msg[256];
            sprintf(msg, "Program state: %d"
                         "Rotor running: %d"
                         "Turns done: %d"
                         "Pills left: %d", programstate.state, rotor_running, current_steps_taken, pills_left);

//            printf("%s", msg);

            write_to_eeprom(PROGRAM_STATE_ADDRESS, data, 5);
            speak_to_server(msg);
        }

    }
    return 0;
}