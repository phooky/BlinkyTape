#include "ColorLoop.h"
#include "BlinkyTape.h"

#include <Arduino.h>

void serialLoop(CRGB* leds) {
  static int pixelIndex;
  int idx = 0;
  uint8_t buffer[3];
  while(true) {
    if (Serial.available()) {
      uint8_t c = Serial.read();
      if (c == 255) {
	LEDS.show();
	pixelIndex = 0;
	idx = 0;   
	// BUTTON_IN (D10):   07 - 0111
	// EXTRA_PIN_A(D7):          11 - 1011
	// EXTRA_PIN_B (D11):        13 - 1101
	// ANALOG_INPUT (A9): 14 - 1110
	
	char c = (digitalRead(BUTTON_IN)    << 3)
	  | (digitalRead(EXTRA_PIN_A)  << 2)
	  | (digitalRead(EXTRA_PIN_B)  << 1)
	  | (digitalRead(ANALOG_INPUT)     );
	Serial.write(c);
      } else {        
        buffer[idx++] = Serial.read();
        if (idx == 3) {
          if(pixelIndex == LED_COUNT) break; // Prevent overflow by ignoring the pixel data beyond LED_COUNT
          leds[pixelIndex] = CRGB(buffer[0], buffer[1], buffer[2]);
          pixelIndex++;
          idx = 0;
        }
      }
    }
  }
}
