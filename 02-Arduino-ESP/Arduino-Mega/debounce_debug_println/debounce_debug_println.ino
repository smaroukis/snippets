// Debounce Example and Debug-Flag based toggle Serial.println output on/off

// Usage:
// Set the DEBUG_ENABLED flag to 1 and use `debug_println()` instead of Serial.println 
// Set to 0 to disable output
#define DEBUG_ENABLED 0

#include <Arduino.h>

#if DEBUG_ENABLED
  #define debug_println(str) Serial.println(str)
  #define debug_print(str) Serial.print(str)
#else
  #define debug_println(str) 
  #define debug_print(str) 
#endif

// Module connection pins (Digital Pins)
#define CLK 61
#define DIO 60
#define BUTTON_PIN 2
#define DEBOUNCE_DELAY 50
#define BUTTON_PRESSED 1
#define DEBOUNCE_FALSE 0

// The amount of time (in milliseconds) between tests
#define TEST_DELAY 2000

// Function Decl
int update_button();

void setup() {
  // Setup button as input pullup
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  Serial.begin(9600);

  // Read EEPROM stored time value on startup
}

void loop() {
  // check for button press
  int buttonPress = update_button();
  if (buttonPress) {
    // Do something
    Serial.println("Button Press");
  }
}

// Non blocking debounce function that returns 1 on a Unpressed to Pressed transition 
// Requires: Button pin as input pullup
int update_button() {
  static int lastButtonState = 1; // unpressed = HIGH
  static unsigned long lastDebounceTime = 0; // overflow is defined for unsigned
  const int debounceDelay = DEBOUNCE_DELAY;
  int buttonState = digitalRead(BUTTON_PIN);
  static int flagDebouncing;

  // Reset/Start debounce timer if state change
  if (!flagDebouncing && buttonState != lastButtonState) {
    debug_println("Started debounce timer");
    // start debounce timer
    lastDebounceTime = millis();
    flagDebouncing = 1;
  }

  // Debounce timer finished, accept current button state
  if ( flagDebouncing && (millis() - lastDebounceTime > debounceDelay)) {
    debug_println("Timer finished");
    debug_print("Button state (accepted): "); debug_println(buttonState);
    // Remove thiese
    // lastDebounceTime += millis();
    flagDebouncing = 0;

    if (buttonState != lastButtonState) {
      // Only return button pressed for Unpressed -> Pressed
        if (buttonState == 0) {// low is pressed 
          lastButtonState = buttonState; // update for next iteration
          return BUTTON_PRESSED;
        }
    } else {
      lastButtonState = buttonState; // update for next iteration
      return DEBOUNCE_FALSE;
    }
  }
  // Fall through to here if timer is not finished or we are not debouncing
  if (!flagDebouncing) {
    // Update last button state outside of the debounce check when not debouncing
    lastButtonState = buttonState; 
  } else {
    // do nothing (wait), since we are debouncing
  }

  return DEBOUNCE_FALSE;
}
