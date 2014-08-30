/*
|| @file 	samarkand.ino
|| @version	1.0
|| @description
|| | This software runs the LED strip lighting effects of the Samarkand gate
|| | for the Silk Road Art Project, a 2014 Burning Man Honorarium recipient.
|| #
||
|| @changelog
|| | 1.0 2014-06-24 - Danne Stayskal : Initial Release
|| #
||
|| @author	Danne Stayskal
|| @contact	danne@stayskal.com
|| @support     http://danne.stayskal.com/
|| @license     MIT License
*/

/* 
==========================================
=  LIBRARIES
*/

// LED Strip
#include "LPD8806.h"
#include "SPI.h"
#define NUM_LEDS 192
#define LEDS_PER_STRIP 48
#define NUM_CHANNELS 3

#define DATA_PIN_A 6
#define CLOCK_PIN_A 7
#define DATA_PIN_B 8
#define CLOCK_PIN_B 9
#define DATA_PIN_C 10
#define CLOCK_PIN_C 11
#define DATA_PIN_D 12
#define CLOCK_PIN_D 13
#define MAX_BRIGHTNESS 255


/* 
==========================================
=  INITIALIZERS
*/

// LED Strip
LPD8806 strip_a = LPD8806(LEDS_PER_STRIP, DATA_PIN_A, CLOCK_PIN_A);
LPD8806 strip_b = LPD8806(LEDS_PER_STRIP, DATA_PIN_B, CLOCK_PIN_B);
LPD8806 strip_c = LPD8806(LEDS_PER_STRIP, DATA_PIN_C, CLOCK_PIN_C);
LPD8806 strip_d = LPD8806(LEDS_PER_STRIP, DATA_PIN_D, CLOCK_PIN_D);

// One-dimensional cellular automata rules (0..255, inclusive)
uint8_t rules[NUM_CHANNELS] = {110, 90, 90};

// Provision neighborhood and generation maps
uint8_t neighborhood[NUM_LEDS][NUM_CHANNELS];
uint8_t generation[NUM_LEDS][NUM_CHANNELS];


/* 
==========================================
=  FUNCTIONS
*/

void automata();
void contour_frame();
void revert_frame();
void push_frame(uint16_t wait, uint16_t steps);


/* 
==========================================
= setup()
= Initializes variables and drivers, called once by Arduino during startup.
*/
void setup() {

  // Start the LED strips
  strip_a.begin();
  strip_b.begin();
  strip_c.begin();
  strip_d.begin();
  strip_a.show();
  strip_b.show();
  strip_c.show();
  strip_d.show();

  // Initialize neighborhood, generation, and frame buffer  
  for(uint8_t i = 0; i < NUM_LEDS; i++){
    for(uint8_t c = 0; c < NUM_CHANNELS; c++){
      neighborhood[i][c] = int(random(64));
      generation[i][c] = 0;
    }
  }
}


/* 
==========================================
= loop()
= The main heartbeat, called by Arduino when previous run of loop() completes.
*/
void loop() {

  // Run the cellular automata rules
  automata();
  
  // Contour, push, and revert the frame
  contour_frame();
  push_frame(1,20);
  revert_frame();

}


/* 
==========================================
= contour_frame()
= Applies visual effects to neighborhood
*/
void contour_frame() {
  uint8_t i;

  for (i = 0; i < NUM_LEDS; i++) {
    neighborhood[i][0] = neighborhood[i][0] / 1;   // Red
    neighborhood[i][1] = neighborhood[i][1] / 16;  // Blue
    neighborhood[i][2] = neighborhood[i][2] / 32;  // Green

    generation[i][0]   = generation[i][0]   / 1;   // Red
    generation[i][1]   = generation[i][1]   / 16;  // Blue
    generation[i][2]   = generation[i][2]   / 32;  // Green
  }
}


/* 
==========================================
= revert_frame()
= Reverts visual effects applied to neighborhood by contour_frame()
*/
void revert_frame() {
  uint8_t i;

  for (i = 0; i < NUM_LEDS; i++) {
    neighborhood[i][0] = neighborhood[i][0] * 1;   // Red
    neighborhood[i][1] = neighborhood[i][1] * 16;  // Blue
    neighborhood[i][2] = neighborhood[i][2] * 32;  // Green

    generation[i][0]   = generation[i][0]   * 1;   // Red
    generation[i][1]   = generation[i][1]   * 16;  // Blue
    generation[i][2]   = generation[i][2]   * 32;  // Green
  }
}


/* 
==========================================
= automata()
= Populates a generation based on automata rules governing this neighborhood
*/
void automata() {

  // Incrementers
  uint8_t i, i_right, i_left, c;

  for (i = 0; i < NUM_LEDS; i++) {
    for (c = 0; c < NUM_CHANNELS; c++) {
      generation[i][c] = neighborhood[i][c] / 8;
    }
    
    // Orient ourselves
    i_left = i - 1;
    if (i_left < 0) {
      i_left = NUM_LEDS;
    }
    i_right = i + 1;
    if (i_right > NUM_LEDS){
      i_right = 0;
    }
    
    // Calculate the generation
    for (uint8_t c = 0; c < NUM_CHANNELS; c++) {
      if (neighborhood[i_left][c] <= 32 && neighborhood[i][c] <= 32 && neighborhood[i_right][c] <= 32) {
        if (rules[c] & (1 << 0)){
          generation[i][c] = 64;
        }
      }
      if (neighborhood[i_left][c] <= 32 && neighborhood[i][c] <= 32 && neighborhood[i_right][c] > 32) {
        if (rules[c] & (1 << 1)){
          generation[i][c] = 64;
        }
      }
      if (neighborhood[i_left][c] <= 32 && neighborhood[i][c] > 32 && neighborhood[i_right][c] <= 32) {
        if (rules[c] & (1 << 2)){
          generation[i][c] = 64;
        }
      }
      if (neighborhood[i_left][c] <= 32 && neighborhood[i][c] > 32 && neighborhood[i_right][c] > 32) {
        if (rules[c] & (1 << 3)){
          generation[i][c] = 64;
        }
      }
      if (neighborhood[i_left][c] > 32 && neighborhood[i][c] <= 32 && neighborhood[i_right][c] <= 32) {
        if (rules[c] & (1 << 4)){
          generation[i][c] = 64;
        }
      }
      if (neighborhood[i_left][c] > 32 && neighborhood[i][c] <= 32 && neighborhood[i_right][c] > 32) {
        if (rules[c] & (1 << 5)){
          generation[i][c] = 64;
        }
      }
      if (neighborhood[i_left][c] > 32 && neighborhood[i][c] > 32 && neighborhood[i_right][c] <= 32) {
        if (rules[c] & (1 << 6)){
          generation[i][c] = 64;
        }
      }
      if (neighborhood[i_left][c] > 32 && neighborhood[i][c] > 32 && neighborhood[i_right][c] > 32) {
        if (rules[c] & (1 << 7)){
          generation[i][c] = 64;
        }
      }
    }
  }

  // Swap the neightborhood with the new generation
  for (uint8_t i = 0; i < NUM_LEDS; i++) {
    for (uint8_t c = 0; c < NUM_CHANNELS; c++) {
      uint8_t temp = neighborhood[i][c];
      neighborhood[i][c] = generation[i][c]; 
      generation[i][c] = temp;
    }
  }
}


/* 
==========================================
= push_frame(uint16_t wait, uint16_t steps)
= Pushes the frame buffer to the LED strip
*/
void push_frame(uint16_t wait, uint16_t steps) {

  uint8_t step_values[3];
  steps *= 10;

  for (uint8_t step = 1; step <= steps; step +=10) {
    for (uint8_t i = 0; i < NUM_LEDS; i++) {
      for (uint8_t c = 0; c < NUM_CHANNELS; c++) {

        // Linear cross-fade
        step_values[c] = ((step * neighborhood[i][c])/steps) + (((steps - step ) * generation[i][c])/steps);

        // Clip values greater than MAX_BRIGHTNESS
        if (step_values[c] > MAX_BRIGHTNESS) {
          step_values[c] = MAX_BRIGHTNESS;
        }
      }
      if (i < 48) {
        strip_a.setPixelColor(i, strip_a.Color(step_values[0],step_values[1],step_values[2]) );
      }
      if (i >= 48 && i < 96) {
        strip_b.setPixelColor(i - 48, strip_b.Color(step_values[0],step_values[1],step_values[2]) );
      }
      if (i >= 96 && i < 144) {
        strip_c.setPixelColor(i - 96, strip_c.Color(step_values[0],step_values[1],step_values[2]) );
      }
      if (i >= 144) {
        strip_d.setPixelColor(i - 144, strip_d.Color(step_values[0],step_values[1],step_values[2]) );
      }

    }
    strip_a.show();
    strip_b.show();
    strip_c.show();
    strip_d.show();
    delay(wait / steps);
  }
}