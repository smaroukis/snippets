#include <Arduino.h>
#include <TM1637Display.h>

#define DEBUG_ENABLED 0

#if DEBUG_ENABLED
  #define debug_println(str) Serial.println(str)
  #define debug_print(str) Serial.print(str)
#else
  #define debug_println(str) 
  #define debug_print(str) 
#endif

// Module connection pins (Digital Pins)
#define A_CLK 61 // A -Minutes & Seconds
#define A_DIO 60
#define B_CLK 55 // B - Hours
#define B_DIO 54
#define BUTTON_PIN 2
#define DEBOUNCE_DELAY 50
#define BUTTON_PRESSED 1
#define DEBOUNCE_FALSE 0

#define TEST_DELAY 500// tm1637 - The amount of time (in milliseconds) between tests

// Global variables
unsigned long SW_PREV_ms = 0; // stopwatch store current millis() time
const long SW_INTERVAL_SECOND = 1000; // interval in ms
// TODO load these from EEPROM
unsigned long seconds = 45;
unsigned long minutes = 59;
unsigned long hours = 0;

const uint8_t SEG_DONE[] = {
  SEG_B | SEG_C | SEG_D | SEG_E | SEG_G,          // d
  SEG_A | SEG_B | SEG_C | SEG_D | SEG_E | SEG_F,  // O
  SEG_C | SEG_E | SEG_G,                          // n
  SEG_A | SEG_D | SEG_E | SEG_F | SEG_G           // E
};

TM1637Display tm1637_minsec(A_CLK, A_DIO);
TM1637Display tm1637_hour(B_CLK, B_DIO);
// Function Decl
int update_button();
int second_has_passed();
int count_dig_return_pos(int x, int y);

//////////////////////////////////////////////////////////////

void setup() {
  // Setup button as input pullup
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  Serial.begin(9600);

  // Read EEPROM stored time value on startup
  Serial.println("Starting timer...");

  // Initialize Brightness and Test Displays by flashing 5 times:
  init_test(&tm1637_minsec);
  init_test(&tm1637_hour);

}

//////////////////////////////////////////////////////////////

void loop() {
  // Check for button press
  int buttonPress = update_button();

  if (buttonPress) {
    Serial.println("Button Press");
    // Start/stop timer
  }

  if (second_has_passed()) {
    seconds += 1;
    if (seconds % 60 == 0) {
      minutes += 1;
      if (minutes % 60 == 0) {
        hours += 1;
        if (hours % 10000 == 0) {
          // Mastery
        }
      }
    }
    Serial.println(seconds);
    update_stopwatch();
  }

  // update clock every 1s

  // analog read voltage for power shutoff

  // if analog read < THRESH, store EEPROM

}

// Calculate if second has passed
int second_has_passed() {
  unsigned long now = millis();
  if ( now - SW_PREV_ms > SW_INTERVAL_SECOND) {
    // update 
    SW_PREV_ms = now;
    return true;
  }

  return false;
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


/*
**************** TM1637 and Clock Functions *****************************
*/

// Uses global variables to update clock display
void update_stopwatch() {
  // Update Minutes and Seconds Display
  tm1637_minsec.showNumberDecEx(seconds % 60, 0, true, 2, 2);
  tm1637_minsec.showNumberDecEx(minutes % 60, 1 << 6, true, 2, 0);

  // Update Hours Display
  // leave leading blanks, "1"hr starts at pos 3, "10"hr at pos 2, etc.
  if (hours) {
    int pos = count_dig_return_pos(hours, 3);
    int len = 3 - pos + 1;
    tm1637_hour.showNumberDec(hours, false, len, pos);
  }
}

// Returns starting position for right aligned integer (max position = 3)
int count_dig_return_pos(int num, int startPos)
{
    num /= 10;
    int i = startPos;
    while (num != 0) {
        num /= 10;
        i--;
    }
    return i;
}

void init_test(TM1637Display* pDisplay) {
  uint8_t data[] = { 0xff, 0xff, 0xff, 0xff };
  uint8_t blank[] = { 0x00, 0x00, 0x00, 0x00 };
  pDisplay->setBrightness(0x0f);

  // On/Off test
  for (int k = 0; k < 4; k++) {
    pDisplay->setSegments(data);
    delay(TEST_DELAY);
    pDisplay->setSegments(blank);
    delay(TEST_DELAY);
  }
}

void test(TM1637Display* pDisplay) {
  TM1637Display display = *pDisplay;
  display = *pDisplay;
  int k;
  uint8_t data[] = { 0xff, 0xff, 0xff, 0xff };
  uint8_t blank[] = { 0x00, 0x00, 0x00, 0x00 };
  display.setBrightness(0x0f);

  // All segments on
  display.setSegments(data);
  delay(TEST_DELAY);

  // Selectively set different digits
  data[0] = display.encodeDigit(0);
  data[1] = display.encodeDigit(1);
  data[2] = display.encodeDigit(2);
  data[3] = display.encodeDigit(3);
  display.setSegments(data);
  delay(TEST_DELAY);

  /*
  for(k = 3; k >= 0; k--) {
	display.setSegments(data, 1, k);
	delay(TEST_DELAY);
	}
  */

  display.clear();
  display.setSegments(data + 2, 2, 2);
  delay(TEST_DELAY);

  display.clear();
  display.setSegments(data + 2, 2, 1);
  delay(TEST_DELAY);

  display.clear();
  display.setSegments(data + 1, 3, 1);
  delay(TEST_DELAY);


  // Show decimal numbers with/without leading zeros
  display.showNumberDec(0, false);  // Expect: ___0
  delay(TEST_DELAY);
  display.showNumberDec(0, true);  // Expect: 0000
  delay(TEST_DELAY);
  display.showNumberDec(1, false);  // Expect: ___1
  delay(TEST_DELAY);
  display.showNumberDec(1, true);  // Expect: 0001
  delay(TEST_DELAY);
  display.showNumberDec(301, false);  // Expect: _301
  delay(TEST_DELAY);
  display.showNumberDec(301, true);  // Expect: 0301
  delay(TEST_DELAY);
  display.clear();
  display.showNumberDec(14, false, 2, 1);  // Expect: _14_
  delay(TEST_DELAY);
  display.clear();
  display.showNumberDec(4, true, 2, 2);  // Expect: __04
  delay(TEST_DELAY);
  display.showNumberDec(-1, false);  // Expect: __-1
  delay(TEST_DELAY);
  display.showNumberDec(-12);  // Expect: _-12
  delay(TEST_DELAY);
  display.showNumberDec(-999);  // Expect: -999
  delay(TEST_DELAY);
  display.clear();
  display.showNumberDec(-5, false, 3, 0);  // Expect: _-5_
  delay(TEST_DELAY);
  display.showNumberHexEx(0xf1af);  // Expect: f1Af
  delay(TEST_DELAY);
  display.showNumberHexEx(0x2c);  // Expect: __2C
  delay(TEST_DELAY);
  display.showNumberHexEx(0xd1, 0, true);  // Expect: 00d1
  delay(TEST_DELAY);
  display.clear();
  display.showNumberHexEx(0xd1, 0, true, 2);  // Expect: d1__
  delay(TEST_DELAY);

  // Run through all the dots
  for (k = 0; k <= 4; k++) {
    display.showNumberDecEx(0, (0x80 >> k), true);
    delay(TEST_DELAY);
  }

  // Brightness Test
  for (k = 0; k < 4; k++)
    data[k] = 0xff;
  for (k = 0; k < 7; k++) {
    display.setBrightness(k);
    display.setSegments(data);
    delay(TEST_DELAY);
  }

  // On/Off test
  for (k = 0; k < 4; k++) {
    display.setBrightness(7, false);  // Turn off
    display.setSegments(data);
    delay(TEST_DELAY);
    display.setBrightness(7, true);  // Turn on
    display.setSegments(data);
    delay(TEST_DELAY);
  }


  // Done!
  display.setSegments(SEG_DONE);
}
