#include "main.h"

extern volatile uint8_t turns_done;
extern volatile uint16_t steps;
extern volatile uint8_t program_state;
extern volatile uint16_t current_steps_taken;
extern volatile uint8_t rotor_running;
extern volatile bool calibration_on;
volatile bool led_on = false;
volatile uint8_t old_program_state = 0;

int main(void) {
    uint8_t pills_left = 7;
    uint timer_start,timer_end,timer_dif;
    timer_start = 0;

    pgstate programstate;

    stdio_init_all();
    init_eeprom();
    initialize_gpios(GPIO_PULL_UP, GPIO_IN, 0, "BTN", 1, BUTTON_PIN);
    initialize_gpios(GPIO_PULL_DOWN, GPIO_OUT, 0, "LED", 1, LED_PIN);

//    uint8_t buffer[9] = {0, 0, 0, 0, 0, 0, 0, 0, 0};
    uint8_t buffer[9];
    read_from_eeprom(PROGRAM_STATE_ADDRESS, buffer, 9);
    if (!lorawan_connection() || !connect_to_server()) { exit(-1); }

    programstate.state = buffer[0];
    programstate.not_state = buffer[1];

    for (int i = 0; i < 9; i++) printf("EEPROM %d: %d\n", i, buffer[i]);

    if (validate_mem(&programstate)) {
        rotor_running = buffer[2];
        program_state = buffer[0];
        turns_done = buffer[3];
        pills_left = buffer[4];
        steps = buffer[5] << 8 | buffer[6] >> 8;
        current_steps_taken = buffer[7] << 8 | buffer[8] >> 8;
        printf("CURRENT_STEPS_TAKEN: %d\nSTEPS: %d", current_steps_taken, steps);

        if (rotor_running) {
            if (program_state == 2) program_state = 5;
            else program_state = 6;
        }
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
//                printf("Case 1");
                pwm_set_gpio_level(LED_PIN, 0);
                variable_reset();
                position_calib();
                program_state = 2;
                old_program_state = 1;
                break;
            case (2):
//                printf("Case 2");
                calibration();
                program_state = 3;
                old_program_state = 2;
                break;
            case (3):
                pwm_set_gpio_level(LED_PIN, 500);
                old_program_state = 3;
                if (gpio_get(BUTTON_PIN)) while (gpio_get(BUTTON_PIN)) sleep_ms(20);
                program_state = 4;
                break;
            case (4): {
//                printf("%d", timer_start);
                rotor_running = 1;
                uint8_t moro = 1;
                write_to_eeprom(ROTOR_RUNNING_ADDRESS, &moro, 1);
                pwm_set_gpio_level(LED_PIN, 0);
                timer_end = clock();
                timer_dif = (timer_end-timer_start)/CLOCKS_PER_SEC;
                if(timer_dif>=TURN_DIVIDER_TIMER || timer_start==0) {
                    turns_done++;
                    if (turns_done < 8) {
                        timer_start = clock();

                        turn_divider();
                    }
                    else {
                        printf("Full revolution done, all pills dispensed.\n");
                        stop_ABCD();
                        program_state = 0;
                        rotor_running = 0;
                        moro = 0;
                        write_to_eeprom(ROTOR_RUNNING_ADDRESS, &moro, 1);
                    }
                }
                old_program_state = 4;

                break;
            }
            case (5):
                //kesken calibroinnin
                printf("case 5");
                variable_reset();
                position_calib();
                program_state = 2;
                break;
            case (6):
                //kesken 1/8 turns
                printf("case 6");
                position_calib();
                reset_calib();
                timer_start = 0;
                program_state = 4;
                break;

        }

        // at the end of every loop write information to EEPROM and LoRa
        if (program_state != old_program_state){
            set_pg_state(&programstate, program_state);

            uint8_t data[7] = {
                    programstate.state,
                    programstate.not_state,
                    rotor_running,
                    turns_done,
                    pills_left,
                    steps,
                    current_steps_taken
            };

//            printf("Rotor running: %d\n", rotor_running);

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