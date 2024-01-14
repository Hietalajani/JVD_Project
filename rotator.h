//
// Created by hiets on 18.12.2023.
//

#ifndef JVD_PROJECT_ROTATOR_H
#define JVD_PROJECT_ROTATOR_H
#include <stdio.h>
#include "LED.h"
#include "time.h"
#include "pico/stdlib.h"
// define other stuff
#define TURN_DIVIDER_TIMER_MS 10000 // 30000 for 30s turns
#define CALIBRATION_REVOS 1 // 3 for more accurate
// define pins
#define IN1 2
#define IN2 3
#define IN3 6
#define IN4 13
#define OPTO_PIN 28
#define PIEZO_PIN 27

//testing
#define BUTTON_PIN 12

// declaring functions

void init_rotor();
void step_A();
void step_B();
void step_AB();
void step_C();
void step_BC();
void step_D();
void step_CD();
void step_DA();
void stop_ABCD();
void position_calib();
void turn_clock();
void turn_counterclock();
void colib_steps_callback();
void calibration_callback();
void piezo_callback();
void rotor_startup();
void variable_reset();
void calibration();
void turn_divider();


#endif //JVD_PROJECT_ROTATOR_H