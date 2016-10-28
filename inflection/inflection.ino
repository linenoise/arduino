/*
||
|| @file 	inflection.pde
|| @version	0.2
|| @author	Danne Stayskal
|| @contact	danne@stayskal.com
||
|| @description
|| | This is the production software that runs the visual effects for the
|| | Inflection honorarium art project for Burning Man 2015
|| #
||
|| @changelog
|| | 0.1   2015-06-18 - Danne Stayskal : Initial Prototype
|| | 0.2   2015-06-25 - Danne Stayskal : Adding accelerometer and relay support, improved animations
|| | 0.2.1 2015-06-25 - Danne Stayskal : Adding sync code for floods on relays
|| | 1.0   2015-08-29 - Danne Stayskal : Final Production code.
|| #
||
*/

// red   - Power (5 Volts)
// white - Data - Pin 13
// green - Clock - Pin 12
// blue  - Ground

#include "FastLED.h"
#include "chipsets.h"
#include "Metro.h"

// Provision the LED interface
#define NUM_LEDS 1300
#define DATA_PIN 13
#define CLOCK_PIN 12
//CRGB leds[NUM_LEDS];


// Starting position and speed for the wave
int block_start = 0;
int block_speed = 1;

// Length and shape of the block
int block_length = 10;
int block_shape[10] = { 1, 4, 10, 27, 90, 90, 27, 10, 4, 1 };

CRGB RED( 64, 0, 0);
CRGB BLUE( 0, 0, 64 );

int n = 0;

WS2801Controller<DATA_PIN, CLOCK_PIN> leds;

/*
|| setup()
|| Initializes needed variables and drivers, called by Arduino once during initialization.
*/
void setup() {
  leds.init();
  leds.clearLeds(NUM_LEDS);
}

/*
|| loop()
|| The main runtime heartbeat, called by Arduino once whenever the previous run of loop() completes.
*/


void loop() {

  for (int n = 0; n < block_length; n++) {
    leds.mWaitDelay.wait();
    leds.mSPI.select();

    for (int x = 0; x < NUM_LEDS; x++) {
      int v = block_shape[(x + n) % block_length];
      CRGB data( CHSV( 2 * ((x + n) / (block_length)), 255, v ) );
      uint8_t a = scale8(data.r, 255);
      uint8_t b = scale8(data.g, 255);
      uint8_t c = scale8(data.b, 255);
      leds.mSPI.writeByte(a);
      leds.mSPI.writeByte(b);
      leds.mSPI.writeByte(c);
    }
    leds.mSPI.waitFully();
    leds.mSPI.release();
    leds.mWaitDelay.mark();
    //    leds.showColor(CRGB(0, n, 0), NUM_LEDS);
    //    delay(100);
    //    leds.showColor(CRGB(0, 0, n), NUM_LEDS);
    //    delay(100);
  }
  //  for (int i = 0; i < NUM_LEDS; i++){
  //    leds[i] = CRGB(random(64), 0, random(64));


  // Push this frame to the LEDs
  //  FastLED.show();
  //  delay(10);

}


/*
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
