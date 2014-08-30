/*
||
|| @file 	Mandelbrot.pde
|| @version	1.0
|| @author	Danne Stayskal
|| @contact	danne@stayskal.com
|| @support     http://danne.stayskal.com/software/fastfader
||
|| @description
|| | This is a graphical demonstration that runs on an LED strip.
|| | In this demonstration, the LED strip displays a portion of the Mandelbrot fractal one line at a time.
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

int pixel_weights[24][3] = {
  {30, 10, 90},
  {40, 0, 80},
  {50, 0, 70},
  {60, 0, 60},
  {70, 0, 50},
  {80, 0, 40},
  {90, 10, 30},
  {100, 20, 20},
  {90, 30, 10},
  {80, 40, 0},
  {70, 50, 0},
  {60, 60, 0},
  {50, 70, 0},
  {40, 80, 0},
  {30, 90, 10},
  {20, 100, 20},
  {10, 90, 30},
  {0, 80, 40},
  {0, 70, 50},
  {0, 60, 60},
  {0, 50, 70},
  {0, 40, 80},
  {10, 30, 90},
  {20, 20, 100},
};


int mandelbrots[1][20][18] = {
  {
    {24, 24, 24, 24, 24, 24, 24, 13, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24},
    {24, 24, 24, 24, 24, 24, 13, 11, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24},
    {24, 24, 24, 24, 24, 16, 11, 10, 18, 24, 24, 24, 24, 24, 24, 24, 24, 24},
    {24, 13, 12, 24, 13, 10,  8,  9, 14, 24, 24, 24, 24, 24, 24, 24, 24, 24},
    {14, 10, 11, 18, 10,  9,  8,  8, 11, 24, 24, 24, 24, 24, 24, 24, 24, 24},
    { 8,  7, 10,  9,  7,  6,  7,  7, 12, 24, 24, 24, 24, 24, 24, 24, 24, 24},
    { 6,  6,  7,  8,  6,  6,  6,  7,  9, 17, 24, 24, 24, 24, 24, 24, 24, 24},
    { 5,  5,  5,  6,  6,  6,  6,  6,  8, 12, 24, 24, 24, 24, 24, 24, 24, 24},
    { 5,  5,  5,  5,  5,  5,  6,  6,  7, 24, 24, 24, 24, 24, 24, 24, 24, 24},
    { 4,  4,  4,  4,  4,  5,  5,  5,  7, 24, 24, 24, 24, 24, 24, 24, 24, 24},
    { 4,  4,  4,  4,  4,  4,  5,  5,  6, 10, 12, 24, 24, 24, 24, 24, 24, 24},
    { 4,  4,  4,  4,  4,  4,  5,  5,  5,  7,  9, 13, 24, 24, 24, 24, 24, 24},
    { 3,  4,  4,  4,  4,  4,  4,  5,  5,  6,  8, 18, 24, 24, 24, 24, 24, 24},
    { 3,  3,  3,  4,  4,  4,  4,  4,  5,  6,  8, 19, 24, 24, 24, 24, 24, 24},
    { 3,  3,  3,  3,  4,  4,  4,  4,  5,  6,  8, 15, 24, 24, 24, 24, 24, 24},
    { 2,  3,  3,  3,  3,  4,  4,  4,  4,  5,  8, 24, 14, 10, 13, 24, 24, 24},
    { 2,  2,  3,  3,  3,  3,  4,  4,  4,  5,  8, 18, 12,  8, 11, 24, 24, 19},
    { 2,  2,  3,  3,  3,  3,  3,  3,  4,  4,  7,  7, 11,  7,  7,  8,  9, 13},
    { 2,  2,  2,  3,  3,  3,  3,  3,  4,  4,  5,  6,  7,  6,  6,  8,  9, 24},
    { 2,  2,  2,  2,  3,  3,  3,  3,  3,  3,  4,  5,  5,  5,  6,  6,  8, 24},
  },
};

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
  int val = 0;
  int frame[18];
  for (int fractal = 0; fractal < 1; fractal++) {
    for (int line = 0; line < 20; line++) {
      for (int pos = 0; pos < 18; pos++) {
        int value = mandelbrots[fractal][line][pos];
        val = mandelbrots[0][0][0];
        for (int channel = 0; channel < 3; channel++) {
          pixel_fader.set_pixel(pos, pixel_weights[value][channel], channel);
        }
      }        
      pixel_fader.push(50,10);
    }
    for (int line = 19; line > 0; line--) {
      for (int pos = 0; pos < 18; pos++) {
        int value = mandelbrots[fractal][line][pos];
        val = mandelbrots[0][0][0];
        for (int channel = 0; channel < 3; channel++) {
          pixel_fader.set_pixel(pos, pixel_weights[value][channel], channel);
        }
      }        
      pixel_fader.push(50,10);
    }
  }
}  

 
//========================================
// ACTUAL MANDELBROT CODE 
// (that's too slow to run on the Arduino Uno)
//========================================
//double center_real = -0.6209;
//double center_imaginary = 0.6555;
//double mandel_width = 0.3727;
//
//double min_real = center_real - mandel_width; 
//double max_real = center_real + mandel_width;
//double min_imaginary = center_imaginary - mandel_width; 
//double max_imaginary = center_imaginary + mandel_width;
//
//int max_iterations = 8;
//int mandel_height = 50;
/* 
==========================================
= loop()
= The main runtime heartbeat, called by Arduino once whenever the previous run of loop() completes.
*/
//void loop() {
//  for (double imaginary_pos = min_imaginary; imaginary_pos <= max_imaginary; imaginary_pos += (max_imaginary - min_imaginary) / mandel_height) {
//    int pixel_pos = 0;
//    for (double real_pos = min_real; real_pos <= max_real; real_pos = real_pos + ((max_real - min_real) / NUM_LEDS)) {
//      double real_depth = real_pos; 
//      double imaginary_depth = imaginary_pos;
//      int n = 0;
//      for (n = 0; n < max_iterations; n++) {
//        double a = real_depth * real_depth; 
//        double b = imaginary_depth * imaginary_depth;
//        if (a + b > 4.0){
//          break;
//        }
//        imaginary_depth = 2 * real_depth * imaginary_depth + imaginary_pos; 
//        real_depth = a - b + real_pos;
//      }
////      frame[pixel_pos][0] = pixel_weights[n][0];
////      frame[pixel_pos][1] = pixel_weights[n][1];
//      frame[pixel_pos][2] = pixel_weights[n][2];
//      Serial.print("pixel ");
//      Serial.print(pixel_pos);
//      Serial.print(" converged at ");
//      Serial.println(n);
//      pixel_pos++;
//    }
//    pixel_fader.set_frame(frame);
//    pixel_fader.push(1,1,NO_DECAY);
//  }
//}
