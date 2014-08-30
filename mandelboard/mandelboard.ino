
#include "FastLED.h"
#include "DueTimer.h"

#define NUM_LEDS 240
#define DATA_PIN 7

CRGB leds[NUM_LEDS];
int pos = 0;

void setup() {
//  delay(2000);
  FastLED.addLeds<WS2812B, DATA_PIN, RGB>(leds, NUM_LEDS);
  
  Timer3.attachInterrupt(callback);
  Timer3.start(50000); // Calls every 50ms
}

void loop() {
  // put your main code here, to run repeatedly:

}


void callback() {
  FastLED.show();
  leds[pos] = CRGB::Black;
  pos++;
  pos %= NUM_LEDS;
  leds[pos] = CRGB::White;
}
