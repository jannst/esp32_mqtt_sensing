#include <main.h>

void execute_n_times(uint16_t num_executions, void (*callback)(void), uint32_t delay_between)
{
    for (uint16_t i=0; i<num_executions; i++)
    {
        callback();
        if(i < num_executions-1)
        scaledDelayMs(delay_between);
    }
}

void glowRow() {
    for (uint8_t i = 0; i < NUM_LEDS; i++)
    {
        setLed(i, 0, 255, 400);
        scaledDelayMs(600);
    }   
    scaledDelayMs(1200);

    for (uint8_t i = 0; i < NUM_LEDS; i++)
    {
        setLed(i, 255, 0, 400);
        scaledDelayMs(600);
    }   
    scaledDelayMs(1200);
}


void runway() {
    for (uint8_t i = 0; i < NUM_LEDS/2; i++)
    {
        setLed(i, 0, 255, 500);
        setLed(NUM_LEDS-1-i, 0, 255, 500);
        scaledDelayMs(800);
        setLed(i, 255, 0, 900);
        setLed(NUM_LEDS-1-i, 255, 0, 900);
        scaledDelayMs(800);
    }
    scaledDelayMs(3200);
}

void blink() {
    for (uint8_t i = 0; i < NUM_LEDS; i++)
    {
        setLed(i, 0, 255, 160);
    }   
    scaledDelayMs(1000);

    for (uint8_t i = 0; i < NUM_LEDS; i++)
    {
        setLed(i, 255, 0, 160);
    }   
    scaledDelayMs(1000);  
}

void row_towards() {
    uint8_t led_delay = 90;
    uint16_t delay_proc = 600;
    

    setLed(0, 0, 255, led_delay);
    scaledDelayMs(delay_proc);

    setLed(0, 255, 0, led_delay);
    setLed(5, 0, 255, led_delay);
    scaledDelayMs(delay_proc);

    setLed(5, 255, 0, led_delay);
    setLed(1, 0, 255, led_delay);
    scaledDelayMs(delay_proc);

    setLed(1, 255, 0, led_delay);
    setLed(4, 0, 255, led_delay);
    scaledDelayMs(delay_proc);

    setLed(4, 255, 0, led_delay);
    setLed(2, 0, 255, led_delay);
    scaledDelayMs(delay_proc);

    setLed(2, 255, 0, led_delay);
    setLed(3, 0, 255, led_delay);
    scaledDelayMs(delay_proc);
    setLed(3, 255, 0, led_delay);
}

void mode_coreo() {
    execute_n_times(3, glowRow, 0);
    scaledDelayMs(1500);
    execute_n_times(5, row_towards, 0);
    scaledDelayMs(1500);
    execute_n_times(5, runway, 0);
    scaledDelayMs(1500);
    execute_n_times(4, blink, 100);
}

void CoreTask1(void *parameter)
{
    for (;;)
    {
        switch (getMode())
        {
        case MODE_0:
            glowRow();
            break;
        case MODE_1:
            row_towards();
            break;
        case MODE_2:
            blink();
            break;
        case MODE_3:
            runway();
            break;
        case MODE_4:
            break;
        case MODE_5:
            mode_coreo();
            break;
        default:
            break;
        }


    }
}