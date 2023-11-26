#include <main.h>

TaskHandle_t Core1TaskHnd;
uint8_t led_gpio_pins[] = {23, 22, 16, 4, 2, 15};
struct Lamp led_states[NUM_LEDS];

#define POT_IN 32
#define NUM_SAMPLES 20
// is 1023 because we left shift 2 bits while sampling
#define POT_MAL_VAL 1023
#define SPEED_MAX_VAL 12

#define B_1 33
#define B_2 25
#define B_3 26
#define B_4 27
#define B_5 14
#define B_6 12

hw_timer_t *poti_read_timer = NULL;
volatile double speedMultiplier = 1;

void scaledDelayMs(uint32_t ms) {
    delay(ms/speedMultiplier);
}

void IRAM_ATTR poti_read(){
    uint32_t sum = 0;
    for(uint16_t i = 0; i < NUM_SAMPLES; i++) {
        sum += analogRead(POT_IN);
    }
    speedMultiplier = 1 + ((double)((sum/NUM_SAMPLES)>>2) / POT_MAL_VAL) * SPEED_MAX_VAL;
}

uint8_t mode = MODE_0;

uint8_t getMode() {
    return mode;
}

void IRAM_ATTR isr_b1() {
    mode = MODE_0;
}

void IRAM_ATTR isr_b2() {
    mode = MODE_1;
}

void IRAM_ATTR isr_b3() {
    mode = MODE_2;
}

void IRAM_ATTR isr_b4() {
    mode = MODE_3;
}

void IRAM_ATTR isr_b5() {
    mode = MODE_4;
}

void IRAM_ATTR isr_b6() {
    mode = MODE_5;
}

void setup()
{
    Serial.begin(115200);

    pinMode(B_1, INPUT_PULLUP);
    attachInterrupt(B_1, isr_b1, FALLING);
    pinMode(B_2, INPUT_PULLUP);
    attachInterrupt(B_2, isr_b2, FALLING);
    pinMode(B_3, INPUT_PULLUP);
    attachInterrupt(B_3, isr_b3, FALLING);
    pinMode(B_4, INPUT_PULLUP);
    attachInterrupt(B_4, isr_b4, FALLING);
    pinMode(B_5, INPUT_PULLUP);
    attachInterrupt(B_5, isr_b5, FALLING);
    pinMode(B_6, INPUT_PULLUP);
    attachInterrupt(B_6, isr_b6, FALLING);

    poti_read_timer = timerBegin(0, 80, true);
    timerAttachInterrupt(poti_read_timer, &poti_read, true);
    timerAlarmWrite(poti_read_timer, 200000, true);
    timerAlarmEnable(poti_read_timer);

    //  there are 16 PWM channels in total. i must not exeed 15!
    for (uint8_t i = 0; i < NUM_LEDS; i++)
    {
        ledcSetup(i, PWM_FREQUENCY, PWM_RESOLUTION);
        ledcAttachPin(led_gpio_pins[i], i);
    }

    xTaskCreatePinnedToCore(CoreTask1, "CPU_1", 1000, NULL, 1, &Core1TaskHnd, 1);
}

void loop()
{
    for (uint8_t i = 0; i < NUM_LEDS; i++)
    {
        boolean changed = false;
        struct Lamp* lamp = &led_states[i];
        if (lamp->counter > 0)
        {
            lamp->counter--;
            continue;
        }
        if (lamp->level > lamp->target)
        {
            lamp->level--;
            lamp->counter = lamp->counter_start / speedMultiplier;
            changed = true;
        }
        else if (lamp->level < lamp->target)
        {
            lamp->level++;
            lamp->counter = lamp->counter_start / speedMultiplier;
            changed = true;
        }

        if(changed || ledcRead(i) != lamp->level) 
        {
            ledcWrite(i, lamp->level);
        }
    }
}

void setLed(uint8_t led, uint8_t level, uint8_t target, uint16_t counter_start) {
  led_states[led].target = target;
  led_states[led].level = level;
  led_states[led].counter = 0;
  led_states[led].counter_start = counter_start;
}
