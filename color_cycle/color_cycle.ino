/*
||
|| @file 	waves.pde
|| @version	1.0
|| @author	Danne Stayskal
|| @contact	danne@stayskal.com
|| @support     http://danne.stayskal.com/software/fastfader
||
|| @description
|| | This is a graphical demonstration that runs on an LED strip.
|| | In this demonstration, red, green, and blue colored waves move across the LED strip at varying speeds.
|| #
||
|| @changelog
|| | 1.0 2014-05-29 - Danne Stayskal : Initial Release
|| #
||
|| @license
|| | This library is free software; you can redistribute it and/or
|| | modify it under the terms of the GNU Lesser General Public
|| | License as published by the Free Software Foundation; version
|| | 2.1 of the License.
|| |
|| | This library is distributed in the hope that it will be useful,
|| | but WITHOUT ANY WARRANTY; without even the implied warranty of
|| | MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
|| | Lesser General Public License for more details.
|| |
|| | You should have received a copy of the GNU Lesser General Public
|| | License along with this library; if not, write to the Free Software
|| | Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
|| #
==========================================
*/

#include "FastLED.h"    // https://github.com/FastLED/FastLED

// How many LEDs are in the strip
#define NUM_LEDS 18

// Using the Arduino Uno in dev mode
#define DATA_PIN 2
#define CLOCK_PIN 3

// Create the LED interface, pixel buffer, and fader
CRGB leds[NUM_LEDS];

/* 
==========================================
= setup()
= Initializes needed variables and drivers, called by Arduino once during initialization.
*/
void setup() {
  // Initialize the LED interface
  FastLED.addLeds<LPD8806, DATA_PIN, CLOCK_PIN, RGB>(leds, NUM_LEDS);
}


/* 
==========================================
= loop()
= The main runtime heartbeat, called by Arduino once whenever the previous run of loop() completes.
*/
void loop() {
  static uint8_t hue = 0;
  CHSV(hue++, 150, 100)); 
  delay(10);
}
