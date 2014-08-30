/*
|| @file 	tent_lights.ino
|| @version	1.0
|| @description
|| | This runs the LED strip on Dann's guitar's LED driver
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
#include "FastLED.h"        // https://github.com/FastLED/FastLED
#define NUM_LEDS 41
#define NUM_CHANNELS 3
#define DATA_PIN 12
#define MAX_BRIGHTNESS 200


/* 
==========================================
=  INITIALIZERS
*/

// LED Strip
CRGB leds[NUM_LEDS];
uint8_t decalog_scale[10] = {0, 4, 9, 15, 22, 30, 39, 52, 69, 100};

// Serial Interface
char character;
char newline = '\n';
int row, col, i, j;

// Buffers
uint8_t input_buffer[67];
uint8_t input_pos = 0;
CRGB output_buffer[41];
CRGB previous_buffer[41];

/* 
==========================================
=  PROTOTYPES
*/

void setup();
void loop();
CRGB pixel_for_range(uint8_t range_start, uint8_t range_end, uint8_t max_red, uint8_t max_green, uint8_t max_blue, double gain);
void push_frame(uint16_t wait, uint16_t steps);


/* 
==========================================
= setup()
=
= Initializes variables and drivers, called once by Arduino during startup.
*/
void setup() {

  // Start the LED strip
  FastLED.addLeds<WS2811, DATA_PIN, RGB>(leds, NUM_LEDS);

  // Start the serial port
  Serial.begin(115200);
  
  // Initialize the LEDs
  for (uint8_t i = 0; i < NUM_LEDS; i++) {
    for (uint8_t c = 0; c < NUM_CHANNELS; c++) {
      leds[i] = CRGB(0,0,0);
    }
  }
  FastLED.show();
}


/* 
==========================================
= loop()
=
= The main heartbeat, called by Arduino when previous run of loop() completes.
*/
void loop() {
  if (Serial.available()) {
    character = Serial.read();
    if (character == newline) {
      input_pos = 0;
      //                                   --INPUT--    ---COLOR---   GAMMA
      // Calculate the pixel value         START END    R    G    B
      output_buffer[0]  =  pixel_for_range(1,    1,     20,  10,  0,      1); 
      output_buffer[1]  =  pixel_for_range(1,    1,     20,  10,  0,      1);
      output_buffer[2]  =  pixel_for_range(1,    1,     30,  20,  0,      1); // Brown
      output_buffer[3]  =  pixel_for_range(1,    2,     40,  30,  0,      1);
      output_buffer[4]  =  pixel_for_range(1,    3,     50,  40,  0,      1); 
      output_buffer[5]  =  pixel_for_range(2,    3,     50,  50,  0,      1);
      output_buffer[6]  =  pixel_for_range(2,    3,     50,  50,  0,      1); // Yellow
      output_buffer[7]  =  pixel_for_range(2,    3,     50,  50,  0,      1);
      output_buffer[8]  =  pixel_for_range(2,    3,     60,  50,  0,      1);
      output_buffer[9]  =  pixel_for_range(2,    3,     70,  45,  0,      1); 
      output_buffer[10] =  pixel_for_range(2,    4,     85,  35,  0,      1);
      output_buffer[11] =  pixel_for_range(2,    4,     100, 25,  0,      1); // Orange
      output_buffer[12] =  pixel_for_range(2,    5,     100, 20,  0,      1);
      output_buffer[13] =  pixel_for_range(2,    5,     100, 10,  0,      1);
      output_buffer[14] =  pixel_for_range(3,    5,     100, 0,   0,      1);
      output_buffer[15] =  pixel_for_range(3,    5,     100, 0,   0,      1); // Red
      output_buffer[16] =  pixel_for_range(3,    6,     90,  0,   0,      1);
      output_buffer[17] =  pixel_for_range(4,    7,     70,  0,   10,     1);
      output_buffer[18] =  pixel_for_range(4,    8,     50,  0,   30,     1);
      output_buffer[19] =  pixel_for_range(4,    9,     40,  0,   60,     1); // Purple
      output_buffer[20] =  pixel_for_range(6,    10,    30,  0,   80,     1);
      output_buffer[21] =  pixel_for_range(7,    11,    20,  0,   100,    1);
      output_buffer[22] =  pixel_for_range(8,    12,    10,  0,   100,    1);
      output_buffer[23] =  pixel_for_range(9,    13,    0,   0,   100,    1);
      output_buffer[24] =  pixel_for_range(10,   14,    0,   0,   100,    1); // Blue
      output_buffer[25] =  pixel_for_range(12,   16,    0,   0,   100,    1);
      output_buffer[26] =  pixel_for_range(13,   19,    0,   0,   100,    1);
      output_buffer[27] =  pixel_for_range(14,   20,    0,   25,  50,     1);
      output_buffer[28] =  pixel_for_range(18,   25,    0,   80,  25,     1); // Aquamarine
      output_buffer[29] =  pixel_for_range(21,   27,    0,   90,  10,     1);
      output_buffer[30] =  pixel_for_range(26,   30,    0,   100, 0,      1);
      output_buffer[31] =  pixel_for_range(29,   34,    0,   100, 0,      1); // Green
      output_buffer[32] =  pixel_for_range(31,   36,    0,   100, 0,      1);
      output_buffer[33] =  pixel_for_range(35,   40,    10,  86,  0,      1);
      output_buffer[34] =  pixel_for_range(39,   45,    20,  72,  0,      1); 
      output_buffer[35] =  pixel_for_range(48,   52,    25,  50,  0,      1); // Pea soup
      output_buffer[36] =  pixel_for_range(54,   58,    50,  50,  0,      1);
      output_buffer[37] =  pixel_for_range(59,   63,    50,  50,  0,      1);
      output_buffer[38] =  pixel_for_range(60,   63,    50,  50,  0,      1); // Yellow
      output_buffer[39] =  pixel_for_range(61,   63,    50,  50,  0,      1);
      output_buffer[40] =  pixel_for_range(62,   63,    50,  50,  0,      1);
      
      // Show this section of pixels
      push_frame(20,10);
        
    } else {
      input_buffer[input_pos] = (int)character;
      input_pos++;
    }
  }
}


/* 
==========================================
= pixel_for_range(uint8_t range_start, uint8_t range_end, uint8_t red, uint8_t green, uint8_t blue, double gain)
=
= Determines the correct value for a pixel based on:
=   * range_start: which input buffer index to start aggregating at
=   * range_end: which input buffer index to end at
=   * max_red, max_green, and max_blue: (0..MAX_BRIGHTNESS) values for this pixel at max strength
=   * gain, to compensate for buffer index positions that never reach full strength
*/
CRGB pixel_for_range(uint8_t range_start, uint8_t range_end, uint8_t max_red, uint8_t max_green, uint8_t max_blue, double gain) {

  // Aggregate it
  uint8_t range_scalar = (range_end - range_start) + 1;
  uint8_t range_value = 0;
  for(i = range_start; i <= range_end; i++){
    range_value += input_buffer[i] - '0';
  }

  // Scale it
  range_value = range_value / range_scalar;
  uint8_t red = ((max_red * 2 * decalog_scale[range_value]) / 100) * gain;
  uint8_t green = ((max_green * 2 * decalog_scale[range_value]) / 100) * gain;
  uint8_t blue = ((max_blue * 2 * decalog_scale[range_value]) / 100) * gain;
  
  // Clip it
  if (red > MAX_BRIGHTNESS) {
    red = MAX_BRIGHTNESS;
  }
  if (green > MAX_BRIGHTNESS) {
    green = MAX_BRIGHTNESS;
  }
  if (blue > MAX_BRIGHTNESS) {
    blue = MAX_BRIGHTNESS;
  }

  // And return it
  return CRGB(green,red,blue);
}


/* 
==========================================
= push_frame(uint16_t wait, uint16_t steps)
=   * wait is the number of microseconds to wait between steps
=   * steps is how many divisions of colorspace to cycle through
= 
= Pushes the frame buffer to the LED strip
*/
void push_frame(uint16_t wait, uint16_t steps) {

  uint8_t step_values[3];
  steps *= 10;

  for (uint8_t step = 1; step <= steps; step +=10) {
    for (uint8_t i = 0; i < NUM_LEDS; i++) {
      for (uint8_t c = 0; c < NUM_CHANNELS; c++) {

        // Linear cross-fade
        step_values[c] = ((step * output_buffer[i][c])/steps) + (((steps - step ) * previous_buffer[i][c])/steps);

        // Clip values greater than MAX_BRIGHTNESS
        if (step_values[c] > MAX_BRIGHTNESS) {
          step_values[c] = MAX_BRIGHTNESS;
        }

      }

      leds[i] = CRGB(step_values[0],step_values[1],step_values[2]);
    }
    FastLED.show();
    delay(wait);
  }

  for (uint8_t i = 0; i < NUM_LEDS; i++) {
    previous_buffer[i] = output_buffer[i];
  }
}
