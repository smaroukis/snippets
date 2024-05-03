// 10,000 hour desktop counter using arduino mega and 2x tm1637 modules
// Usage: plug in power to start the timer automatically, 
// press push button to pause and save data into EEPROM
// Loads stored time from EEPROM after reboot 
// **user must save time manually by pausing the timer with the pushbutton before shutting down**

// TODO - brownout detection and automatically save to EEPROM 
// currently takes 3.3ms to perform EEPROM write, not sure if that's enough time to perform an analog read
// and write before the MCU goes brown, 
// probably look into a coin cell backup with timer for this

#include <Arduino.h>
#include <TM1637Display.h>
#include <EEPROM.h>

#define DEBUG_ENABLED 0

#if DEBUG_ENABLED
  #define debug_println(str) Serial.println(str)
  #define debug_print(str) Serial.print(str)
#else
  #define debug_println(str) 
  #define debug_print(str) 
#endif
#define TEST_PIN 62

// Module connection pins (Digital Pins)
#define A_CLK 61 // A -Minutes & Seconds
#define A_DIO 60
#define B_CLK 55 // B - Hours
#define B_DIO 54
#define BUTTON_PIN 2
#define DEBOUNCE_DELAY 50
#define BUTTON_PRESSED 1
#define DEBOUNCE_FALSE 0

#define TEST_DELAY 200// tm1637 - The amount of time (in milliseconds) between blinking tests

// Global variables
uint8_t stopwatch_on = 1;
unsigned long SW_PREV_ms = 0; // stopwatch store current millis() time
const long SW_INTERVAL_SECOND = 1000; // interval in ms
// TODO load these from EEPROM
#define SIGNATURE_EEADDR 0
#define EXPECTED_SIGNATURE 0x2A
#define SW_EEADDR_BASE 4
#define MINUTES_EEADDR 8
#define HOURS_EEADDR 12

typedef struct { // initialized to reset values, read from EEPROM at runtime
  uint32_t seconds;
  uint32_t minutes;
  uint32_t hours;
} SW_COUNTER_t;
SW_COUNTER_t stopwatch = { 0, 0, 0}; // Initial values if EEPROM is not set

uint8_t fill[] = { 0xff, 0xff, 0xff, 0xff };
uint8_t blank[] = { 0x00, 0x00, 0x00, 0x00 };
TM1637Display tm1637_minsec(A_CLK, A_DIO);
TM1637Display tm1637_hour(B_CLK, B_DIO);

// Function Decl
int update_button();
int second_has_passed();
int count_dig_return_pos(int x, int y);
void clear_eeprom();
void init_eeprom_struct();
void save_to_eeprom_struct();
void read_eeprom_struct();
void toggle_pin(int x);
void display_blink2();
void display_init_test2();

//////////////////////////////////////////////////////////////

void setup() {
  #if DEBUG_ENABLED
    Serial.begin(9600); Serial.println("Started Serial");
    while (!Serial); // wait until Serial is ready
    clear_eeprom();
  #endif 

  // Check and load from EEPROM
  int signature = EEPROM.read(SIGNATURE_EEADDR);
  if (signature != EXPECTED_SIGNATURE) {
    init_eeprom_struct(); // will also write values to eeprom
    debug_println("EEPROM uninitialized, initialized");
  } else {
    read_eeprom_struct();
    debug_print("Loaded HH:MM:SS from EEPROM: "); debug_print(stopwatch.hours); debug_print(":"); debug_print(stopwatch.minutes); debug_print(":"); debug_println(stopwatch.seconds);
  }

  // Setup button as input pullup
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  pinMode(TEST_PIN, OUTPUT); 

  debug_println("Starting timer...");

  // Initialize Brightness and Test Displays 
  display_init_test2(&tm1637_minsec, &tm1637_hour);
}

//////////////////////////////////////////////////////////////

void loop() {
  // Check for button press
  int buttonPress = update_button();

  if (buttonPress) { // Start/stop timer
    debug_println("Button Press");
    if (stopwatch_on) {
      save_to_eeprom_struct(); 
    } else {
      second_has_passed(); // set timer currentMillis to zero to start again
    }
    stopwatch_on = !stopwatch_on;
    display_blink2(&tm1637_minsec, &tm1637_hour, 1); update_stopwatch(); // hack to re-display digits
  }

  if (stopwatch_on && second_has_passed()) {
    
    stopwatch.seconds += 1;
    if (stopwatch.seconds % 60 == 0) {
      stopwatch.minutes += 1;
      if (stopwatch.minutes % 60 == 0) {
        stopwatch.hours += 1;
       if (stopwatch.hours % 10000 == 0) {
          tm1637_minsec.clear();
          while(1) {
            tm1637_minsec.setSegments(blank);
            tm1637_hour.setSegments(blank);
            delay(TEST_DELAY);
            tm1637_minsec.setSegments(fill);
            tm1637_hour.setSegments(fill);
            delay(TEST_DELAY);
          }
        }
      }
    }
    debug_println(stopwatch.seconds);
    update_stopwatch();
  }
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
    // start debounce timer
    lastDebounceTime = millis();
    flagDebouncing = 1;
  }

  // Debounce timer finished, accept current button state
  if ( flagDebouncing && (millis() - lastDebounceTime > debounceDelay)) {
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
**************** EEPROM Functions *****************************
*/
void clear_eeprom() {
    SW_COUNTER_t emptyStruct = { 0, 0, 0};
    EEPROM.put(SIGNATURE_EEADDR, 0); // to use custom initial values
    EEPROM.put(SW_EEADDR_BASE, emptyStruct);
}

void init_eeprom_struct() {
  save_to_eeprom_struct();
  EEPROM.put(SIGNATURE_EEADDR, EXPECTED_SIGNATURE);
}

void read_eeprom_struct() {
  EEPROM.get(SW_EEADDR_BASE, stopwatch); // read struct from start address
}

void save_to_eeprom_struct() {
  // benchmarking: 3.49 ms for (3) x .put commands
  EEPROM.put(SW_EEADDR_BASE, stopwatch);
}
/*
**************** TM1637 and Clock Functions *****************************
*/

// Uses global variables to update clock display
void update_stopwatch() {
  // Update Minutes and Seconds Display
  tm1637_minsec.showNumberDecEx(stopwatch.seconds % 60, 0, true, 2, 2);
  tm1637_minsec.showNumberDecEx(stopwatch.minutes % 60, 1 << 6, true, 2, 0);

  // Update Hours Display
  // leave leading blanks, "1"hr starts at pos 3, "10"hr at pos 2, etc.
  if (stopwatch.hours) {
    int pos = count_dig_return_pos(stopwatch.hours, 3);
    int len = 3 - pos + 1;
    tm1637_hour.showNumberDec(stopwatch.hours, false, len, pos);
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

/*
**************** TM1637 Display Functions *****************************
*/
void display_init_test2(TM1637Display* pDisplay1, TM1637Display* pDisplay2) {
  pDisplay1->setBrightness(0x0f);
  pDisplay2->setBrightness(0x0f);
  display_blink2(pDisplay1,pDisplay2, 4);
  // On/Off test
}
  
void display_blink2(TM1637Display* pDisplay1, TM1637Display* pDisplay2, int blinks){
  for (int k = 0; k < blinks; k++) {
    pDisplay1->setSegments(blank);
    pDisplay2->setSegments(blank);
    delay(TEST_DELAY);
    pDisplay2->setSegments(fill);
    pDisplay1->setSegments(fill);
    delay(TEST_DELAY);
  }
  // Leave blank for next write
  pDisplay1->setSegments(blank);
  pDisplay2->setSegments(blank);
}

/*
**************** MISC / DEBUG *****************************
*/
// For scope use
void toggle_pin(int PIN_NO){
  digitalWrite(PIN_NO, !digitalRead(PIN_NO));
}