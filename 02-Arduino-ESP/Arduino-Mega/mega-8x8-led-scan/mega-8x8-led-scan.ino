// Test 4 max7219 driven 8x8 Led Matrices at once
// Simultaneously flash each row_i, column_j led of all four matrices 
// 8x8 led matrix Max7219 chip is connected via SPI

//We always have to include the library
#include "LedControl.h"

// SPI pins from Mega → Max LED matrix chip
int DIN = 51; 
int CS = 53;
int CLK = 52;
// also need +Vcc (5V) and Ground 

#define NUM_DEVICES 4

LedControl lc=LedControl(DIN, CLK, CS, NUM_DEVICES);

/* we always wait a bit between updates of the display */
unsigned long delaytime=200;

/* 
 This time we have more than one device. 
 But all of them have to be initialized 
 individually.
 */
void setup() {
  //we have already set the number of devices when we created the LedControl
  int devices=lc.getDeviceCount();
  //we have to init all devices in a loop
  for(int address=0;address<devices;address++) {
    /*The MAX72XX is in power-saving mode on startup*/
    lc.shutdown(address,false);
    /* Set the brightness to a medium values */
    lc.setIntensity(address,8);
    /* and clear the display */
    lc.clearDisplay(address);
  }
}

void loop() { 
  //read the number cascaded devices
  int devices=lc.getDeviceCount();
  
  //we have to init all devices in a loop
  for(int row=0;row<8;row++) {
    for(int col=0;col<8;col++) {
      // For each device
      for(int address=0;address<devices;address++) {
        lc.setLed(address,row,col,true);
        delay(1);
      }

      delay(delaytime);

      for(int address=0;address<devices;address++) {
        lc.setLed(address,row,col,false);
        delay(1);
      }
      delay(delaytime);
    }
  }
}
