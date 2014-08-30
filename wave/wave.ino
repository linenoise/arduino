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
#include "FastFader.h"  // https://github.com/linenoise/fastfader
#include <math.h>

// How many LEDs are in the strip
#define NUM_LEDS 18

// Using the Arduino Uno in dev mode
#define DATA_PIN 2
#define CLOCK_PIN 3

// Create the LED interface, pixel buffer, and fader
CRGB leds[NUM_LEDS];
int pixel_buffer[NUM_LEDS][3];
FastFader pixel_fader;

// Starting positions for the block
int block_start = 0;

// Speed of the block in pixels per heartbeat
int block_speed = 1;

// Length and shape of the block 
int block_length = 10;
int block_shape[10] = { 1, 4, 10, 27, 90, 90, 27, 10, 4, 1 };

/* 
==========================================
= setup()
= Initializes needed variables and drivers, called by Arduino once during initialization.
*/
void setup() {
  // Initialize the LED interface
  FastLED.addLeds<LPD8806, DATA_PIN, CLOCK_PIN, RGB>(leds, NUM_LEDS);

  // Bind the pixel fader to the LED interface
  pixel_fader.bind(pixel_buffer, leds, NUM_LEDS, FastLED);
}


/* 
==========================================
= loop()
= The main runtime heartbeat, called by Arduino once whenever the previous run of loop() completes.
*/
void loop() {

  // Clear the pixel buffer
  pixel_fader.clear();
  

  // Position the block
  int block_end;
  block_start = block_start + block_speed;
  if (block_start > NUM_LEDS - 1) {
    block_start = block_start - NUM_LEDS;
  }
  if (block_start < 0) {
    block_start = NUM_LEDS - 1;
  }
  block_end = block_start + block_length;

  // Place the block in the pixelbuffer
  for (int pos = block_start; pos < block_end; pos++) {
    int offset = block_end - pos;
    int pos_wrapped = pos;
    if (pos_wrapped > NUM_LEDS - 1) {
      pos_wrapped = pos_wrapped - NUM_LEDS;
    }
    int value = block_shape[offset];
    pixel_fader.set_pixel(pos_wrapped, value);
  }

  // Push this frame to the LEDs
  pixel_fader.push(150);

}
