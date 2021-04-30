#include <Arduino.h>


#define IN_PIN 12
#define OUT_PIN 14
#define DELAY 50

volatile byte state = LOW;
volatile uint32_t lastInMillis;
void blink();

void setup() {
  Serial.begin(115200);
  pinMode(OUT_PIN,OUTPUT);
  pinMode(IN_PIN, INPUT);
  attachInterrupt(IN_PIN, blink, RISING);
}

void loop() {
  digitalWrite(OUT_PIN, state);
}

void blink() {
  // Invertiere den Status: "Lass die LED blinken von HIGH auf LOW/ an auf aus"
  if (millis() > lastInMillis + DELAY || lastInMillis > millis()) {
    state = !state;
    Serial.println("change");
    lastInMillis = millis();
  }
}


/*
using namespace std;

#define NUM_SAMPLES 15
#define MEDIAN  NUM_SAMPLES/2

void setup() {
  Serial.begin(115200);
  delay(1000); 
}

void loop() {
  uint16_t touch1[NUM_SAMPLES];
  uint16_t touch2[NUM_SAMPLES];
  for (uint32_t i = 0; i<NUM_SAMPLES; i++) {
    touch1[i] = touchRead(12);
    touch2[i] = touchRead(14);
    delay(1);
  }

  sort(touch1, touch1 + NUM_SAMPLES);
  sort(touch2, touch2 + NUM_SAMPLES);

  Serial.printf("[G12: %d, G14: %d]\n", touch1[MEDIAN], touch2[MEDIAN]); 
  delay(20);
}
*/