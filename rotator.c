#include "rotator.h"
// rototopototo functions

// Globals.

// Time variables
volatile uint start,end,timer;

// state of case we are going at in main.
volatile int program_state = 0;
// how many slots used from expenser/how many times turned.
volatile int turns_done = 0;

// state of which coils are currently in use.
volatile int rotor_state = 0;
// falling edge noted.
volatile int opto_state = 0;

// For calibrating count of revolutions and steps of full revolution.
volatile int revolutions = 0;
volatile int steps = 0;
// Is calibration going on and shet
volatile bool calibration_on = false;
volatile int steps_colib = 0;
volatile bool rising_edge = false;

// Keeps track on how many steps taken for mid program calibration or shutdown or shet.
volatile int current_steps_taken = 0;

// Pills dropped?
volatile bool pill_drop = false;
volatile int piezo_error_handle = 0;

void init_rotor() {

    // FOR TESTING, DELETE LATER
    gpio_init(BUTTON_PIN);
    gpio_set_dir(BUTTON_PIN, GPIO_IN);
    gpio_pull_up(BUTTON_PIN);

    // Initialize Rotor pins
    gpio_init(IN1);
    gpio_set_dir(IN1, GPIO_OUT);

    gpio_init(IN2);
    gpio_set_dir(IN2, GPIO_OUT);

    gpio_init(IN3);
    gpio_set_dir(IN3, GPIO_OUT);

    gpio_init(IN4);
    gpio_set_dir(IN4, GPIO_OUT);

    // Initialize opto pin as s an input with pull-up.
    gpio_pull_up(OPTO_PIN);
    gpio_set_input_enabled(OPTO_PIN, true);

    // Initialize piezo pin as s an input with pull-up.
    gpio_pull_up(PIEZO_PIN);
    gpio_set_input_enabled(PIEZO_PIN, true);
    stdio_init_all();
    //needs to be configured or set in main func. Doesnt find callback like this.
    //gpio_set_irq_enabled_with_callback(OPTO_PIN,GPIO_IRQ_EDGE_FALL, true, &colib_steps_callback);


}

// functions for coil rotating.
void step_A(){
    gpio_put(IN1, 1);
    sleep_ms(5);
    gpio_put(IN1, 0);
}

void step_B(){
    gpio_put(IN2, 1);
    sleep_ms(5);
    gpio_put(IN2, 0);
}

void step_AB(){
    gpio_put(IN1, 1);
    gpio_put(IN2, 1);
    sleep_ms(5);
    gpio_put(IN1, 0);
    gpio_put(IN2, 0);
}

void step_C(){
    gpio_put(IN3, 1);
    sleep_ms(5);
    gpio_put(IN3, 0);
}

void step_BC(){
    gpio_put(IN2, 1);
    gpio_put(IN3, 1);
    sleep_ms(5);
    gpio_put(IN2, 0);
    gpio_put(IN3, 0);
}

void step_D(){
    gpio_put(IN4, 1);
    sleep_ms(5);
    gpio_put(IN4, 0);
}

void step_CD(){
    gpio_put(IN3, 1);
    gpio_put(IN4, 1);
    sleep_ms(5);
    gpio_put(IN3, 0);
    gpio_put(IN4, 0);
}

void step_DA(){
    gpio_put(IN4, 1);
    gpio_put(IN1, 1);
    sleep_ms(5);
    gpio_put(IN4, 0);
    gpio_put(IN1, 0);
}

void stop_ABCD(){
    //leaves the current coil on/stops the rotor.
    switch (rotor_state) {
        case(0):
            gpio_put(IN1, 1);
            break;
        case(1):
            gpio_put(IN1, 1);
            gpio_put(IN2, 1);
            break;
        case(2):
            gpio_put(IN2, 1);
            break;
        case(3):
            gpio_put(IN2, 1);
            gpio_put(IN3, 1);
            break;
        case(4):
            gpio_put(IN3, 1);
            break;
        case(5):
            gpio_put(IN3, 1);
            gpio_put(IN4, 1);
            break;
        case(6):
            gpio_put(IN4, 1);
            break;
        case(7):
            gpio_put(IN4, 1);
            gpio_put(IN1, 1);
            break;

    }
}

void position_calib(){
    //Calibrates/catches to used coil
    //Run all coils trought
    step_A();
    step_AB();
    step_B();
    step_BC();
    step_C();
    step_CD();
    step_D();
    step_DA();
    //stop
    stop_ABCD();
    sleep_ms(50);
    //run coils back so steps wont differ
    step_D();
    step_CD();
    step_C();
    step_BC();
    step_B();
    step_AB();
    step_A();
    //stop
    stop_ABCD();
    sleep_ms(50);
}

void turn_clock(){
    //turns rotor 1 step clockwise
    switch (rotor_state) {
        case(0):
            step_A();
            rotor_state = 1;
            break;
        case(1):
            step_AB();
            rotor_state = 2;
            break;
        case(2):
            step_B();
            rotor_state = 3;
            break;
        case(3):
            step_BC();
            rotor_state = 4;
            break;
        case(4):
            step_C();
            rotor_state = 5;
            break;
        case(5):
            step_CD();
            rotor_state = 6;
            break;
        case(6):
            step_D();
            rotor_state = 7;
            break;
        case(7):
            step_DA();
            rotor_state = 0;
            break;

    }
}

void turn_counterclock(){
    switch (rotor_state) {
        //turns rotor 1 step counterclockwise
        case(0):
            step_A();
            rotor_state = 7;
            break;
        case(1):
            step_AB();
            rotor_state = 0;
            break;
        case(2):
            step_B();
            rotor_state = 1;
            break;
        case(3):
            step_BC();
            rotor_state = 2;
            break;
        case(4):
            step_C();
            rotor_state = 3;
            break;
        case(5):
            step_CD();
            rotor_state = 4;
            break;
        case(6):
            step_D();
            rotor_state = 5;
            break;
        case(7):
            step_DA();
            rotor_state = 6;
            break;

    }
}

//Falling edge callback for calibration
void colib_steps_callback() {
    if (calibration_on) {
        end = clock();
        timer = (end - start)/CLOCKS_PER_SEC;
        printf("%d\n", timer);
        if (opto_state == 1 && timer>5) {
            revolutions++;
            printf("%d rising edge\n", revolutions);
        }
        if (opto_state == 0) {
            printf("Starting calibration/ counting steps.\n");
            opto_state = 1;
        }
        start = clock();
    }
}

//Rising edge callback for calibration.
void calibration_callback(){
    if(calibration_on==true){
        printf("Calibration rising edge callback\n");
        rising_edge = true;
        gpio_set_irq_enabled_with_callback(OPTO_PIN,GPIO_IRQ_EDGE_FALL, true, &colib_steps_callback);
    }
}

void piezo_callback(){
    printf("Piezo callback\n");
    piezo_error_handle++;
    if (program_state==3 && piezo_error_handle>3) {
        pill_drop = true;
    }
}

void rotor_startup(){
    init_rotor();
    position_calib();
    stop_ABCD();
}

void variable_reset(){
    turns_done = 0;
    opto_state = 0;
    revolutions = 0;
    steps = 0;
    steps_colib = 0;
    rising_edge = false;
    current_steps_taken = 0;
    gpio_set_irq_enabled_with_callback(OPTO_PIN,GPIO_IRQ_EDGE_FALL, true, &colib_steps_callback);
}

void calibration(){
    // Calibration
    //Calibrating steps for full rotate. (3xfull rotation/3)
    calibration_on = true;

    // runs until finds falling edge
    do {
        turn_clock();
    }while(opto_state!=1);
    //calculating steps
    do {
        turn_clock();
        steps++;
    }while(revolutions!=1);
    steps = steps/revolutions;

    printf("Steps: %d\n", steps);
    stop_ABCD();
    sleep_ms(50);

    //Correcting the positioning of the holes.
    gpio_set_irq_enabled_with_callback(OPTO_PIN,GPIO_IRQ_EDGE_RISE, true, &calibration_callback);
    do {
        turn_clock();
        steps_colib++;
    }while(rising_edge!=true);

    stop_ABCD();
    sleep_ms(50);

    steps_colib = steps_colib*2/3;
    for(int i=0;i<=steps_colib;i++){
        turn_counterclock();
    }

    calibration_on = false;
    stop_ABCD();
    sleep_ms(50);
}

void turn_divider(){
    //turn 1/8 full steps
    gpio_set_irq_enabled_with_callback(PIEZO_PIN,GPIO_IRQ_EDGE_FALL, true, &piezo_callback);
    for(int i=0;i<=steps/8;i++){
        turn_counterclock();
        current_steps_taken++;
    }
    stop_ABCD();
    if(!pill_drop){
        for(int i=0;i<5;i++){
            toggle_leds(LED_PIN, PWM_FREQUENCY);
            sleep_ms(50);
            toggle_leds(LED_PIN, PWM_FREQUENCY);
        }
        printf("No pill drop.\n");
        printf("Currents steps: %d\n", current_steps_taken);
        printf("Turns done: %d\n", turns_done);
        //add here blink led 5 times.
    }
    else {
        printf("Pill dispensed.\n");
        printf("Currents steps: %d\n", current_steps_taken);
        printf("Turns done: %d\n", turns_done);
        pill_drop = false;
    }
    piezo_error_handle = 0;
}



