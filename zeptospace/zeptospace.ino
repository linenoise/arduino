#include <Adafruit_NeoPixel.h>

//   NEO_RGB     Pixels are wired for RGB bitstream
//   NEO_GRB     Pixels are wired for GRB bitstream
//   NEO_KHZ400  400 KHz bitstream (e.g. FLORA pixels)
//   NEO_KHZ800  800 KHz bitstream (e.g. High Density LED strip)
SIGNAL_TYPE=NEO_GRB + NEO_KHZ800

// Lateral data busses
DATA_A_PIN    = 2 // front lateral: 28 pixels  (0..27)
DATA_B_PIN    = 3 // left lateral: 28 pixels   (28..55)
DATA_C_PIN    = 4 // back lateral: 28 pixels   (56..83)
DATA_D_PIN    = 5 // right lateral: 28 pixels  (84..111)
DATA_A_PIXELS = 28
DATA_B_PIXELS = 28
DATA_C_PIXELS = 28
DATA_D_PIXELS = 28

// Ascender data busses
DATA_E_PIN    = 6 // front-left ascender: 30 pixels (112..141)
DATA_F_PIN    = 7 // left-back ascender: 30 pixels (142..171)
DATA_G_PIN    = 8 // back-right ascendar: 30 pixels (172..201)
DATA_H_PIN    = 9 // right-front ascender: 30 pixels (202..231)
DATA_E_PIXELS = 30
DATA_F_PIXELS = 30
DATA_G_PIXELS = 30
DATA_H_PIXELS = 30

// Allocate memory for the data busses
// Parameter 1 = number of pixels in strip
// Parameter 2 = pin number (most are valid)
// Parameter 3 = pixel type flags, add together as needed:
Adafruit_NeoPixel strip_a = Adafruit_NeoPixel(DATA_A_PIXELS, DATA_A_PIN, SIGNAL_TYPE);
Adafruit_NeoPixel strip_b = Adafruit_NeoPixel(DATA_B_PIXELS, DATA_B_PIN, SIGNAL_TYPE);
Adafruit_NeoPixel strip_c = Adafruit_NeoPixel(DATA_C_PIXELS, DATA_C_PIN, SIGNAL_TYPE);
Adafruit_NeoPixel strip_d = Adafruit_NeoPixel(DATA_D_PIXELS, DATA_D_PIN, SIGNAL_TYPE);
Adafruit_NeoPixel strip_e = Adafruit_NeoPixel(DATA_E_PIXELS, DATA_E_PIN, SIGNAL_TYPE);
Adafruit_NeoPixel strip_f = Adafruit_NeoPixel(DATA_F_PIXELS, DATA_F_PIN, SIGNAL_TYPE);
Adafruit_NeoPixel strip_g = Adafruit_NeoPixel(DATA_G_PIXELS, DATA_G_PIN, SIGNAL_TYPE);
Adafruit_NeoPixel strip_h = Adafruit_NeoPixel(DATA_H_PIXELS, DATA_H_PIN, SIGNAL_TYPE);

// Length and shape of the block that runs up the ascenders
int block_length    = 10;
int block_shape[10] = { 1, 4, 10, 27, 90, 90, 27, 10, 4, 1 };
int block_start     = 0;


void setup() {
  // Provision all strands
  strip_a.begin();
  strip_b.begin();
  strip_c.begin();
  strip_d.begin();
  strip_e.begin();
  strip_f.begin();
  strip_g.begin();
  strip_h.begin();

  // Initialize all strands to "off"
  strip_a.show();
  strip_b.show();
  strip_c.show();
  strip_d.show();
  strip_e.show();
  strip_f.show();
  strip_g.show();
  strip_h.show();
}

void loop() {
  
  
  
  // Write the white wave to the ascenders
  leds[i].green = block _shape[(i+block_start) % block_length];
  
  // Iterate the block up by one pixel and wait
  block_start = block_start + 1;
  if (block_start > block_length) {
    block_start = 0;
  }
  delay(50);  
  
//  // Some example procedures showing how to display to the pixels:
//  colorWipe(strip.Color(255, 0, 0), 50); // Red
//  colorWipe(strip.Color(0, 255, 0), 50); // Green
//  colorWipe(strip.Color(0, 0, 255), 50); // Blue
//
//  rainbow(20);
//  rainbowCycle(20);
}

// Fill the dots one after the other with a color
void colorWipe(uint32_t c, uint8_t wait) {
  for(uint16_t i=0; i<strip.numPixels(); i++) {
      strip.setPixelColor(i, c);
      strip.show();
      delay(wait);
  }
}

void rainbow(uint8_t wait) {
  uint16_t i, j;

  for(j=0; j<256; j++) {
    for(i=0; i<strip.numPixels(); i++) {
      strip.setPixelColor(i, Wheel((i+j) & 255));
    }
    strip.show();
    delay(wait);
  }
}

// Slightly different, this makes the rainbow equally distributed throughout
void rainbowCycle(uint8_t wait) {
  uint16_t i, j;

  for(j=0; j<256*5; j++) { // 5 cycles of all colors on wheel
    for(i=0; i< strip.numPixels(); i++) {
      strip.setPixelColor(i, Wheel(((i * 256 / strip.numPixels()) + j) & 255));
    }
    strip.show();
    delay(wait);
  }
}

// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t Wheel(byte WheelPos) {
  if(WheelPos < 85) {
   return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
  } else if(WheelPos < 170) {
   WheelPos -= 85;
   return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  } else {
   WheelPos -= 170;
   return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
}

