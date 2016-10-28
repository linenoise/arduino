#include <Adafruit_NeoPixel.h>

// Parameter 1 = number of pixels in strip
// Parameter 2 = pin number (most are valid)
// Parameter 3 = pixel type flags, add together as needed:
//   NEO_RGB     Pixels are wired for RGB bitstream
//   NEO_GRB     Pixels are wired for GRB bitstream
//   NEO_KHZ400  400 KHz bitstream (e.g. FLORA pixels)
//   NEO_KHZ800  800 KHz bitstream (e.g. High Density LED strip)

Adafruit_NeoPixel strip = Adafruit_NeoPixel(500, 13, NEO_GRB + NEO_KHZ800);

void setup() {
  strip.begin();
  strip.show(); // Initialize all pixels to 'off'
}

void loop() {
  rainbowCycle(20);
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
   return strip.Color(50+((WheelPos * 3)/3), 0, 50);
//   return strip.Color(10+((WheelPos * 3)/3), 10+((255 - WheelPos * 3)/3), 10);
  } else if(WheelPos < 170) {
   WheelPos -= 85;
   return strip.Color(50+((255 - WheelPos * 3)/3), 0, 50+((WheelPos * 3)/3));
//   return strip.Color(10+((255 - WheelPos * 3)/3), 10, 10+((WheelPos * 3)/3));
  } else {
   WheelPos -= 170;
   return strip.Color(50, 0, 50+((255 - WheelPos * 3)/3));
//   return strip.Color(10, 10+((WheelPos * 3)/3), 10+((255 - WheelPos * 3)/3));
  }
}

