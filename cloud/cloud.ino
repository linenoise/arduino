/*
||
|| @file 	waves.pde
|| @version	1.0
|| @author	Dann Stayskal
|| @contact	dann@stayskal.com
|| @support     http://dann.stayskal.com/software/fastfader
||
|| @description
|| | This is a graphical demonstration that runs on an LED strip.
|| | In this demonstration, red, green, and blue colored waves move across the LED strip at varying speeds.
|| #
||
|| @changelog
|| | 1.0 2014-05-29 - Dann Stayskal : Initial Release
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
#include "FastFader.h"  // https://github.com/danndalf/fastfader
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

int cloud_start = 0;
int cloud_shape[18] = { 0, 0, 0, 0, 0, 0, 0, 0, 1, 3, 2, 9, 7, 10, 8, 5, 4, 1 };

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
  int frame[NUM_LEDS][3];
  
  // Clear the pixel buffer
  pixel_fader.clear();
    
  // Seed the frame with some random data
  for (int i = 0; i < NUM_LEDS; i++) {
    int value = int(random(0,50));
    frame[i][0] = frame[i][1] = frame[i][2] = (value * (cloud_shape[cloud_start + i] / 10)) / 250;
  }
  
  // Move the cloud
  cloud_start = cloud_start + int(random(0,2));
  if (cloud_start > NUM_LEDS){
    cloud_start = 0;
  }
  
  // Send the frame to the pixelbuffer
  pixel_fader.set_frame(frame);
  
  // Push this frame to the LEDs
  pixel_fader.push(500,50);

}
