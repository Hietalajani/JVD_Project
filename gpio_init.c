#include "gpio_init.h"

// globals
volatile uint32_t time = 0;
volatile bool state = false;
extern uint8_t program_state;
extern uint8_t old_program_state;

pwm_config pwm_get_config_struct() {
    const uint32_t f_pwm = PWM_FREQUENCY;

    uint32_t sys_clock = clock_get_hz(clk_sys);
    uint32_t divider = sys_clock / MHZ_1;
    uint32_t top = MHZ_1 / f_pwm - 1;

    pwm_config config = {
            sys_clock,
            divider,
            top
    };

    return config;
}

void handler(uint gpio, uint32_t eventmask) {
    uint32_t newtime = time_us_32() / 1000;
//    printf("TIME: %d\nMASK: %s\n", newtime-time, eventmask);
    if(eventmask == GPIO_IRQ_EDGE_FALL) {
        if ((newtime - time) > DEBOUNCE_TIME && !state) {
            state = true;
            if (program_state == 0) {
                program_state = 1;
                old_program_state = 0;
            }
            if (program_state == 3) {
                program_state = 4;
                old_program_state = 3;
            }
        }
    }
    else {
        state = false;
    }
    time = newtime;
}

void initialize_gpios(bool up, bool out, bool input, const char *type, int nr, ...) {
    va_list args;
    va_start(args, nr);

    for(int i = 0; i < nr; i++) {
        uint arg = va_arg(args, uint);
        gpio_init(arg);
        // pull up or down?
        up ? gpio_pull_up(arg) : gpio_pull_down(arg);
        // out or in?
        out ? gpio_set_dir(arg, GPIO_OUT) : gpio_set_dir(arg, GPIO_IN);
        // input?
        if (input) gpio_set_input_enabled(arg, true);
        // buttons get an interrupt
        if (strcmp(type, "BTN") == 0) {
            gpio_set_irq_enabled_with_callback(arg, GPIO_IRQ_EDGE_FALL | GPIO_IRQ_EDGE_RISE, true, &handler);
        }
        // leds get a pwm function for less future-sight
        else if (strcmp(type, "LED") == 0){
            gpio_set_function(arg, GPIO_FUNC_PWM);
            uint slice_num = pwm_gpio_to_slice_num(arg);
            pwm_config config = pwm_get_config_struct();
            pwm_init(slice_num, &config, true);
        }
    }
    va_end(args);
}