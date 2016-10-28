#include "FastLED.h"

// Dimensions of lightboard
#define NUM_LEDS 256
#define NUM_ROWS 16
#define NUM_COLUMNS 16
#define WAVE_SIZE 8

// Pinout of Grove connector
#define DATA_PIN 13
#define CLOCK_PIN 14

// Define the world and pixel buffer
CRGB leds[NUM_LEDS];
bool world[NUM_ROWS][NUM_COLUMNS];
bool next_world[NUM_ROWS][NUM_COLUMNS];

// Wave positions
byte red_wave_pos, blue_wave_pos;
byte wave_table[WAVE_SIZE] = {13, 16, 13, 8, 4, 2, 4, 8};

// Function prototypes
byte count_neighbors(byte, byte);

// Iterators and accomplices
byte i, j, i_up, i_down, j_left, j_right;

void setup() { 
  FastLED.addLeds<WS2811, DATA_PIN, RGB>(leds, NUM_LEDS);
  
  // Seed the world with random data
  randomSeed(analogRead(0));
  for(i = 0; i < NUM_ROWS; i++){
    for(j = 0; j < NUM_COLUMNS; j++){
      world[i][j] = bool(random(2));
    }
  }

  // Set the wave positions
  red_wave_pos = WAVE_SIZE;
  blue_wave_pos = WAVE_SIZE;
}

void loop() {
  
  red_wave_pos += 1;
  if (red_wave_pos == WAVE_SIZE) {
    red_wave_pos = 0;
  }
  
  blue_wave_pos += 1;
  if (blue_wave_pos == WAVE_SIZE) {
    blue_wave_pos = 0;
  }
  
  // Clear the next world
  for(i = 0; i < NUM_ROWS; i++){
    for(j = 0; j < NUM_COLUMNS; j++){
      next_world[i][j] = false;
    }
  }  
  // Calculate the board
  for(i = 0; i < NUM_ROWS; i++){
    for(j = 0; j < NUM_COLUMNS; j++){

      // Plant some random new points in the world
      if(int(random(32)+1) == 32) {
        next_world[i][j] = true;
      }

      // Count our neighbors
      byte neighbors = count_neighbors(i,j);
      
      // Figure out the next world
      if (world[i][j] == true && neighbors < 2) {
        // Any live cell with fewer than two live neighbours dies, as if caused by under-population.
      	next_world[i][j] = false;
      }
      if (world[i][j] == true && (neighbors == 2 || neighbors == 3)) {
        // Any live cell with two or three live neighbours lives on to the next generation.
      	next_world[i][j] = true;
      }
      if (world[i][j] == true && neighbors > 3) {
        // Any live cell with more than three live neighbours dies, as if by overcrowding.
      	next_world[i][j] = false;
      }	
      if (world[i][j] == false && neighbors == 3) {
        // Any dead cell with exactly three live neighbours becomes a live cell, as if by reproduction.
      	next_world[i][j] = true;
      }
    }
  }

  for(byte frame = 0; frame < 32; frame++){
    // Send this fader frame of the world to the LEDs  
    for(i = 0; i < NUM_ROWS; i++){
      for(j = 0; j < NUM_COLUMNS; j++){
  
        // Count our neighbors
        byte neighbors = count_neighbors(i,j);
        
        // What are the non-automata subpixels doing at this point?
        byte blue_value_a = wave_table[(i+blue_wave_pos) % 8];
        byte blue_value_b = wave_table[(i+blue_wave_pos+1) % 8];
        int blue_value_delta = blue_value_b - blue_value_a;
        byte blue_value = blue_value_a + (blue_value_delta * (frame + 1) / 32);

        byte red_value_a = wave_table[(j+red_wave_pos) % 8] / 2;
        byte red_value_b = wave_table[(j+red_wave_pos+1) % 8] / 2;
        int red_value_delta = red_value_b - red_value_a;
        byte red_value = red_value_a + (red_value_delta * (frame + 1) / 32);

        byte green_value = 0;
        
        // Figure out the green pixel color
        if (world[i][j] == true && next_world[i][j] == true) {
          // This one was on and stays on this generation. Just leave it on.
          green_value = 32;
          
        } else if (world[i][j] == false && next_world[i][j] == false) {
          // This one was off and stays off for this generation. Just leave it off.
          green_value = 0;

        } else if (world[i][j] == false && next_world[i][j] == true) {
          // This one was off but comes on for this generation. Fade it in.
          green_value = (frame+1);
          
        } else if (world[i][j] == true && next_world[i][j] == false) {
          // This one was on and turned off for this generation. Fade it out.
          green_value = 32-((frame+1));
        }
        
        // Push this pixel to the panel
        CRGB pixel = CRGB(red_value, green_value, blue_value);
        if(j % 2 == 0) {  // Column is even. Populate in reverse order
          leds[(NUM_COLUMNS - i - 1) + (NUM_ROWS * j)] = pixel;
        } else {  // Column is odd. Populate in direct order
          leds[i + (NUM_COLUMNS * j)] = pixel;
        }
      }
    }
    FastLED.show();
  }
  
  // Move the new world values to next generation's world
  for(i = 0; i < NUM_ROWS; i++){
    for(j = 0; j < NUM_COLUMNS; j++){
      world[i][j] = next_world[i][j];
    }
  }
}

byte count_neighbors(byte i, byte j) {
  // Orient ourselves such that the edges wrap around
  i_up = i + 1;
  i_down = i - 1;
  j_right = j + 1;
  j_left = j - 1;
  if (i == NUM_ROWS - 1) { i_up = 0; }
  if (i == 0) { i_down = NUM_ROWS - 1; }
  if (j == NUM_COLUMNS - 1) { j_right = 0; }
  if (j == 0) { j_left = NUM_COLUMNS - 1; }
  
  // Count our neighbors
  byte neighbors = 0;
  if (world[i_up][j_right] == true) { neighbors += 1; }
  if (world[i_up][j] == true) { neighbors += 1; }
  if (world[i_up][j_left] == true) { neighbors += 1; }
  if (world[i][j_left] == true) { neighbors += 1; }
  if (world[i_down][j_left] == true) { neighbors += 1; }
  if (world[i_down][j] == true) { neighbors += 1; }
  if (world[i_down][j_right] == true) { neighbors += 1; }
  if (world[i][j_right] == true) { neighbors += 1; }
 
  return neighbors;
}
