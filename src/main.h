#include <Arduino.h>

#ifndef main_h
#define main_h

void CoreTask1(void *parameter);

void scaledDelayMs(uint32_t ms);

uint8_t getMode();

# define MODE_0 0
# define MODE_1 1
# define MODE_2 2
# define MODE_3 3
# define MODE_4 4
# define MODE_5 5

#define NUM_LEDS 6
#define PWM_FREQUENCY 5000
#define PWM_RESOLUTION 8

struct Lamp{
    uint8_t level;
    uint8_t target;
    uint16_t counter_start;
    uint16_t counter;
};


void setLed(uint8_t led, uint8_t level, uint8_t target, uint16_t counter_start);

#endif