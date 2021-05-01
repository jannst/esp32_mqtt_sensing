#include <WiFi.h>
#include <PubSubClient.h>
#include <Wire.h>

//shock sensor and led
#define IN_PIN 12
#define OUT_PIN 14
#define DELAY 30
volatile byte lastState = LOW;
volatile byte state = LOW;
volatile uint32_t lastInMillis;
unsigned int numTaps = 0;

//ultrasonic
#define PIN_TRIGGER 27
#define PIN_ECHO 26
#define NUM_SAMPLES 10
#define DIFF_THRESHOLD 100

//networking
#define WIFI_SSID "<wifi_ssid>"
#define WIFI_PASSWORD "<wifi_password_here>"
#define MQTT_SERVER "<hostname_of_mosquitto_server>"
#define MQTT_PORT 1883
char msg[64];

WiFiClient espClient;
PubSubClient client(espClient);

void setup_wifi();
void reconnect();
void blink();
unsigned long sampleUltrasonic();

void setup() {
  Serial.begin(115200);
  pinMode(OUT_PIN,OUTPUT);
  digitalWrite(OUT_PIN, LOW);
  pinMode(IN_PIN, INPUT);
  pinMode(PIN_TRIGGER, OUTPUT);
  pinMode(PIN_ECHO, INPUT);
  attachInterrupt(IN_PIN, blink, RISING);
  setup_wifi();
  client.setServer(MQTT_SERVER, MQTT_PORT);
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  unsigned long ultrasonicDiff = sampleUltrasonic();
  if(ultrasonicDiff > DIFF_THRESHOLD) {
    sprintf(msg, "%lu", ultrasonicDiff);
    client.publish("esp32/ultrasonic", msg);
    delay(20);
  }

  if(state != lastState) {
    lastState = state;
    digitalWrite(OUT_PIN, state);
    sprintf(msg, "tap %u received", ++numTaps);
    client.publish("esp32/tap", msg);
  }
}

void blink() {
  if (millis() > lastInMillis + DELAY || lastInMillis > millis()) {
    state = !state;
    lastInMillis = millis();
  }
}

void setup_wifi() {
  Serial.printf("Connecting to %s", WIFI_SSID);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.printf("WiFi connected\nIP address: %s\n", WiFi.localIP().toString().c_str());
}

void reconnect() {
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    if (client.connect("ESP32_Client")) {
      Serial.println("MQTT connected");
    } else {
      Serial.printf("failed, rc=%d\n", client.state());
      Serial.println("Trying again in 5 seconds");
      delay(5000);
    }
  }
}

unsigned long sampleUltrasonic()
{
  unsigned long samples[NUM_SAMPLES];
  for (int i = 0; i < NUM_SAMPLES; i++)
  {
    digitalWrite(PIN_TRIGGER, LOW);
    delayMicroseconds(2);

    digitalWrite(PIN_TRIGGER, HIGH);
    delayMicroseconds(10);

    samples[i] = pulseIn(PIN_ECHO, HIGH);
    delay(10);
  }

  std::sort(samples, samples + NUM_SAMPLES);
  return samples[NUM_SAMPLES - 1] - samples[0];
}