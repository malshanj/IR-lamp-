#include <IRremote.h>

// Define the IR receiver pin and create the IRrecv object
#define RECV_PIN 13
IRrecv irrecv(RECV_PIN);

// Define the LED pins (using analog pins for white LEDs)
#define WHITE_LED1 5
#define WHITE_LED2 6
#define WHITE_LED3 9
#define WHITE_LED4 10
#define RGB_LED_R 11
#define RGB_LED_G 12
#define RGB_LED_B 8

// Define the IR remote button codes
#define VOL_UP 0xFF9867
#define VOL_DOWN 0xFF6897
#define DECREASE_BRIGHTNESS 0xFFE01F
#define INCREASE_BRIGHTNESS 0xFFA857

// Define the state variable
int state = 0;

// Define the brightness levels for the white LEDs
int whiteLedBrightness[4] = {255, 255, 255, 255};  // Initial brightness level

void setup() {
  // Initialize the IR receiver
  irrecv.enableIRIn();

  // Initialize the LEDs as outputs
  pinMode(WHITE_LED1, OUTPUT);
  pinMode(WHITE_LED2, OUTPUT);
  pinMode(WHITE_LED3, OUTPUT);
  pinMode(WHITE_LED4, OUTPUT);
  pinMode(RGB_LED_R, OUTPUT);
  pinMode(RGB_LED_G, OUTPUT);
  pinMode(RGB_LED_B, OUTPUT);

  // Turn off all LEDs initially
  analogWrite(WHITE_LED1, 0);
  analogWrite(WHITE_LED2, 0);
  analogWrite(WHITE_LED3, 0);
  analogWrite(WHITE_LED4, 0);
  digitalWrite(RGB_LED_R, LOW);
  digitalWrite(RGB_LED_G, LOW);
  digitalWrite(RGB_LED_B, LOW);
}

void loop() {
  decode_results results;
  if (irrecv.decode(&results)) {
    if (results.value == VOL_UP) {
      state++;
    } else if (results.value == VOL_DOWN) {
      state--;
      if (state < 0) state = 0;  // Ensure state doesn't go below 0
    } else if (results.value == DECREASE_BRIGHTNESS) {
      // Decrease the brightness of the white LEDs
      for (int i = 0; i < 4; i++) {
        if (whiteLedBrightness[i] > 0) {
          whiteLedBrightness[i] -= 50;  // Decrease brightness by 50 units
          if (whiteLedBrightness[i] < 0) whiteLedBrightness[i] = 0;  // Ensure brightness doesn't go below 0
        }
      }
    } else if (results.value == INCREASE_BRIGHTNESS) {
      // Increase the brightness of the white LEDs
      for (int i = 0; i < 4; i++) {
        if (whiteLedBrightness[i] < 255) {
          whiteLedBrightness[i] += 50;  // Increase brightness by 50 units
          if (whiteLedBrightness[i] > 255) whiteLedBrightness[i] = 255;  // Ensure brightness doesn't go above 255
        }
      }
    }

    // Turn off all LEDs
    analogWrite(WHITE_LED1, 0);
    analogWrite(WHITE_LED2, 0);
    analogWrite(WHITE_LED3, 0);
    analogWrite(WHITE_LED4, 0);
    digitalWrite(RGB_LED_R, LOW);
    digitalWrite(RGB_LED_G, LOW);
    digitalWrite(RGB_LED_B, LOW);

    // Turn on the appropriate LEDs based on the state
    if (state >= 1) analogWrite(WHITE_LED1, whiteLedBrightness[0]);
    if (state >= 2) analogWrite(WHITE_LED2, whiteLedBrightness[1]);
    if (state >= 3) analogWrite(WHITE_LED3, whiteLedBrightness[2]);
    if (state >= 4) analogWrite(WHITE_LED4, whiteLedBrightness[3]);
    if (state >= 5 && state % 3 == 2) digitalWrite(RGB_LED_B, HIGH);
    if (state >= 6 && state % 3 == 0) digitalWrite(RGB_LED_G, HIGH);
    if (state >= 7 && state % 3 == 1) digitalWrite(RGB_LED_R, HIGH);

    irrecv.resume();  // Receive the next value
  }
}
