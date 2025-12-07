#include <IRremote.h>

#define IR_RECEIVE_PIN 10
#define STOP_LED 9
#define BUZZER_PIN 3
#define TURN_LED 4
#define HEADLIGHT_PIN 5
#define REVERSE_LED 6
#define WIPER_LED 7
#define WINDOW_GREEN 8
#define WINDOW_RED 11
#define TRIG_PIN 12
#define ECHO_PIN 13
#define PARK_GREEN 2
#define PARK_YELLOW A0
#define PARK_RED A1

#define TURN_INTERVAL 500
#define WIPER_INTERVAL 200

bool turnOn = false;
bool turnLedState = false;
unsigned long previousMillisTurn = 0;

bool wipersOn = false;
bool wiperLedState = false;
unsigned long previousMillisWiper = 0;

bool headlightsOn = false;

void setup() {
  Serial.begin(9600);

  pinMode(STOP_LED, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(TURN_LED, OUTPUT);
  pinMode(HEADLIGHT_PIN, OUTPUT);
  pinMode(REVERSE_LED, OUTPUT);
  pinMode(WIPER_LED, OUTPUT);
  pinMode(WINDOW_GREEN, OUTPUT);
  pinMode(WINDOW_RED, OUTPUT);

  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);

  pinMode(PARK_GREEN, OUTPUT);
  pinMode(PARK_YELLOW, OUTPUT);
  pinMode(PARK_RED, OUTPUT);

  digitalWrite(WINDOW_GREEN, HIGH);
  digitalWrite(WINDOW_RED, LOW);

  IrReceiver.begin(IR_RECEIVE_PIN, ENABLE_LED_FEEDBACK);
}

void loop() {

  if (IrReceiver.decode()) {
    uint8_t command = IrReceiver.decodedIRData.command;

    if (command == 0x10) digitalWrite(STOP_LED, HIGH);
    else digitalWrite(STOP_LED, LOW);

    if (command == 0x11) digitalWrite(BUZZER_PIN, HIGH);
    else digitalWrite(BUZZER_PIN, LOW);

    if (command == 0x12) turnOn = !turnOn;

    if (command == 0x14) headlightsOn = !headlightsOn;

    if (command == 0x15) digitalWrite(REVERSE_LED, HIGH);
    else digitalWrite(REVERSE_LED, LOW);

    if (command == 0x16) wipersOn = !wipersOn;

    if (command == 0x18) {
      digitalWrite(WINDOW_GREEN, HIGH);
      digitalWrite(WINDOW_RED, LOW);
    }

    if (command == 0x19) {
      digitalWrite(WINDOW_GREEN, LOW);
      digitalWrite(WINDOW_RED, HIGH);
    }

    IrReceiver.resume();
  }

  if (turnOn) {
    unsigned long currentMillis = millis();
    if (currentMillis - previousMillisTurn >= TURN_INTERVAL) {
      previousMillisTurn = currentMillis;
      turnLedState = !turnLedState;
      digitalWrite(TURN_LED, turnLedState);
    }
  } else digitalWrite(TURN_LED, LOW);

  if (wipersOn) {
    unsigned long currentMillis = millis();
    if (currentMillis - previousMillisWiper >= WIPER_INTERVAL) {
      previousMillisWiper = currentMillis;
      wiperLedState = !wiperLedState;
      digitalWrite(WIPER_LED, wiperLedState);
    }
  } else digitalWrite(WIPER_LED, LOW);

  digitalWrite(HEADLIGHT_PIN, headlightsOn);

  updateParkingSensor();
}

void updateParkingSensor() {
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  long duration = pulseIn(ECHO_PIN, HIGH, 30000);
  if (duration == 0) {
    digitalWrite(PARK_GREEN, LOW);
    digitalWrite(PARK_YELLOW, LOW);
    digitalWrite(PARK_RED, LOW);
    return;
  }

  float distance = duration * 0.034 / 2.0;

  if (distance > 30) {
    digitalWrite(PARK_GREEN, HIGH);
    digitalWrite(PARK_YELLOW, LOW);
    digitalWrite(PARK_RED, LOW);
  } 
  else if (distance > 15) {
    digitalWrite(PARK_GREEN, LOW);
    digitalWrite(PARK_YELLOW, HIGH);
    digitalWrite(PARK_RED, LOW);
  } 
  else {
    digitalWrite(PARK_GREEN, LOW);
    digitalWrite(PARK_YELLOW, LOW);
    digitalWrite(PARK_RED, HIGH);
  }
}
