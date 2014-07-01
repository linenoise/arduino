/*
|| @file 	playa_bike.ino
|| @version	1.0
|| @description
|| | This runs the LED strip lighting effects on Dann's 2014 Burning Man playa bike.
|| #
||
|| @changelog
|| | 1.0 2014-06-24 - Dann Stayskal : Initial Release
|| #
||
|| @author	Dann Stayskal
|| @contact	dann@stayskal.com
|| @support     http://dann.stayskal.com/
|| @license     MIT License
*/

/* 
==========================================
=  LIBRARIES
*/

// Profiling
#include <MemoryFree.h>

// LCD Monitor
#include <LiquidCrystal.h>

// LED Strip
#include "LPD8806.h"
#include "SPI.h"
#define NUM_LEDS 188
#define NUM_CHANNELS 3
#define DATA_PIN 11
#define CLOCK_PIN 13
#define MAX_BRIGHTNESS 255

// Heartbeat LED
#define HEARTBEAT_PIN 8

// Brightness knob
#define BRIGHTNESS_KNOB_PIN 2

// Contouring
// ----------
// NUM_PHASES counts how many phase steps are in the channel phase map
//
// MODULATION_STEPS must be an even integer divisor of NUM_LEDS
// e.g. with NUM_LEDs at 188 and MODULATION_STEPS of 47 (which is 188 / 4),
// There will be four sinusoidal cycles of deviation per length of the LED strip.
#define NUM_PHASES 24
#define MODULATION_STEPS 47


/* 
==========================================
=  INITIALIZERS
*/

// LCD Monitor
LiquidCrystal lcd(12, 10, 5, 4, 3, 2);

// LED Strip
LPD8806 strip = LPD8806(NUM_LEDS, DATA_PIN, CLOCK_PIN);

// One-dimensional cellular automata rules (0..255, inclusive)
uint8_t rules[NUM_CHANNELS] = {124, 110, 110};

// Provision neighborhood and generation maps
uint8_t neighborhood[NUM_LEDS][NUM_CHANNELS];
uint8_t generation[NUM_LEDS][NUM_CHANNELS];

// Heartbeat LED
uint8_t pulse;

// Brightness knob
int brightness;

// Channel phase map
// -----------------
// The channel phase map is the coefficient that cycles red, green, and blue automata through dominance.
// Basically, it's what makes the LEDs cycle through a rainbow of textures rather than sitting and
// spinning on a single color.
//
// * phase_map is the array of "muting" coefficients that each automata channels' values are divided by
// before being displayed on the strip. Example: 
//   * at phase_map[channel_phase[c] of 1, channel c (R=0, G=1, B=2) is full-bright.
//   * at phase_map[channel_phase[c] of 9, channel c it's 1/9th as bright as whichever channel(s) 
//     (if any) is/are being displayed at 1.
//
// * channel_phase and channel_phase_velocity are described in the documentation for setup().
//
// * prev_channel_phase is a staging vector that permits the pixel fader in push_frame() to correctly
//   cross-fade between color values at each heartbeat

uint8_t phase_map[NUM_PHASES] = {
  1, 1, 1, 1, 1, 1, 1, 1, 
  2, 3, 4, 5, 6, 7, 8, 9, 
  9, 8, 7, 6, 5, 4, 3, 2 
};

uint8_t channel_phase[NUM_CHANNELS];
uint8_t prev_channel_phase[NUM_CHANNELS];
int8_t channel_phase_velocity[NUM_CHANNELS];


/* 
==========================================
=  FUNCTIONS
*/

void automata();
void heartbeat();
void contour_frame();
void push_frame(uint16_t wait, uint16_t steps);
void revert_frame();
void read_brightness();
void step_phase_vectors();
void state(char* message);


/* 
==========================================
= setup()
=
= Initializes variables and drivers, called once by Arduino during startup.
*/
void setup() {

  // Start the LCD Monitor
  lcd.begin(16, 3);
  
  // Start the LED strip
  strip.begin();
  strip.show();

  // Start the heartbeat LED
  pulse = HIGH;
  pinMode(HEARTBEAT_PIN, OUTPUT);
  digitalWrite(HEARTBEAT_PIN, pulse);

  // Channel phase vectors
  // -------------------------
  // * channel_phase holds the offset within phase_map (0..NUM_PHASES) each channel (RGB) is at. 
  //
  // The effect that this has on each channel is a muting one. If channel_phase[0] is set to "10", and
  // the 11th value of phase_map is 5, every pixel on that channel will be divided by 5 before being 
  // displayed. Staggering these makes it cycle the automata's color-textures through various rainbow-
  // like effects, depending on the velocities you choose next.
  //
  // * channel_phase_velocity holds the rate at which each channel phase changes per heartbeat. 
  //
  // This may, if you want one or more channel phases to rotate backwards, be a negative number. 
  // At a value of 1, a channel will progress down phase_map one element at a time. 
  // At a value of 4, it'll skip three per heartbeat, which would cause that channel to run through
  // the phase map faster than others with lower values. This overlap would creating more cyans,
  // magentas, and yellows between the reds, greens, and blues than if all velocities were set equally.

  channel_phase[0] = 0;
  channel_phase[1] = 8;
  channel_phase[2] = 16;  

  channel_phase_velocity[0] = 1;
  channel_phase_velocity[1] = 1;
  channel_phase_velocity[2] = 1;

  // Initialize neighborhood, generation
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
=
= The main heartbeat, called by Arduino when previous run of loop() completes.
*/
void loop() {
  heartbeat();
  read_brightness();

  // Run the cellular automata rules
  automata();
  
  // Contour, push, and revert the frame
  contour_frame();
  push_frame(10,20);
  revert_frame();

  // Step phase vectors according to their respective velocities
  step_phase_vectors();

  state("Automata");
}


/* 
==========================================
= read_brightness()
=
= Reads brightness level from potentiometer attached to an analog input
*/
void read_brightness() {
  brightness = analogRead(BRIGHTNESS_KNOB_PIN);
  brightness = int(brightness / 4);
  if (brightness < 5) {
    brightness = MAX_BRIGHTNESS;
  }
}


/* 
==========================================
= contour_frame()
=
= Applies visual effects to neighborhood
*/
void contour_frame() {
  uint8_t i, i_offset, modulation_offset, c, c_gain, delta;

  // Apply the phase map to this neighborhood and the previous generation (so the 
  // pixel fader in push_frame() knows where to fade in from)
  for (i = 0; i < NUM_LEDS; i++) {
    for (c = 0; c < NUM_CHANNELS; c++) {
      neighborhood[i][c] = neighborhood[i][c] / phase_map[channel_phase[c]];
      generation[i][c] = generation[i][c] / phase_map[prev_channel_phase[c]];
    }
  }
}


/* 
==========================================
= revert_frame()
=
= Reverts visual effects applied to neighborhood by contour_frame()
*/
void revert_frame() {
  uint8_t i, c;

  for (i = 0; i < NUM_LEDS; i++) {
    for (c = 0; c < NUM_CHANNELS; c++) {
      // Revert the phase map on each channel's pixels
      neighborhood[i][c] = neighborhood[i][c] * phase_map[channel_phase[c]];

      // Don't worry about reverting the previous generation. Save the clock cycles.
      // automata() will overwrite it anyhow.
    }
  }
}


/* 
==========================================
= step_phase_vectors()
=
= Moves the color phases down its map according to channel velocities
*/
void step_phase_vectors() {
  uint8_t c;

  for (c = 0; c < NUM_CHANNELS; c++) {
    prev_channel_phase[c] = channel_phase[c];

    channel_phase[c] += channel_phase_velocity[c];

    if (channel_phase[c] >= NUM_PHASES){
      channel_phase[c] -= NUM_PHASES;
    }
    if (channel_phase[c] < 0) {
      channel_phase[c] += NUM_PHASES;
    }
  }
}


/* 
==========================================
= automata()
=
= Populates a generation based on automata rules governing this neighborhood
*/
void automata() {
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
    // Reuse the iterator to save a byte. We're done with it in the parent block
    for (c = 0; c < NUM_CHANNELS; c++) {
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
= heartbeat()
=
= Pulses the heartbeat LED
*/
void heartbeat() {
  digitalWrite(HEARTBEAT_PIN,pulse);
  if (pulse == LOW) {
    pulse = HIGH;
  } else {
    pulse = LOW; 
  }
}


/* 
==========================================
= push_frame(uint16_t wait, uint16_t steps)
=   * wait is the number of seconds to wait between steps
=   * steps is how many divisions of colorspace to cycle through
= 
= Pushes the frame buffer to the LED strip
*/
void push_frame(uint16_t wait, uint16_t steps) {

  uint8_t step_values[3];
  steps *= 10;

  for (uint8_t step = 1; step <= steps; step +=10) {
    for (uint8_t i = 0; i < strip.numPixels(); i++) {
      for (uint8_t c = 0; c < NUM_CHANNELS; c++) {

        // Linear cross-fade
        step_values[c] = ((step * neighborhood[i][c])/steps) + (((steps - step ) * generation[i][c])/steps);

        // Scale brightness according to the knob
        step_values[c] = (step_values[c] * brightness) / MAX_BRIGHTNESS;

        // Clip values greater than MAX_BRIGHTNESS
        if (step_values[c] > MAX_BRIGHTNESS) {
          step_values[c] = MAX_BRIGHTNESS;
        }

      }
      strip.setPixelColor(i, strip.Color(step_values[0],step_values[1],step_values[2]) );
    }
    strip.show();
    delay(wait);
  }
}


/* 
==========================================
= state(char* message)
= 
= Prints a status message to the LCD panel followed by memory metrics
*/
void state(char* message) {

  // Clear the display
  char* empty = "                ";
  lcd.setCursor(0, 0);
  lcd.print(empty);
  lcd.setCursor(0, 1);
  lcd.print(empty);

  // Display the message
  lcd.setCursor(0, 0);
  lcd.print(message);
  
  // Display memory metrics
  lcd.setCursor(0, 1);
  uint16_t free_memory = freeMemory();
  lcd.print(free_memory);
  lcd.print("B mem free");
}