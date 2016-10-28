/*
||
|| @file 	inflection_relay_test.pde
|| @version	0.1
|| @author	Danne Stayskal
|| @contact	danne@stayskal.com
||
|| @description
|| | This is the test code that makes sure we have the relay hooked up properly.
|| | It just cycles through channels 1 through 8, turning them on and off in a row.
|| #
||
|| @changelog
|| | 0.1 2015-06-25 - Danne Stayskal : Initial Prototype
|| | 0.2 2015-06-25 - Danne Stayskal : Coordinating relays with Critical build
|| #
||
*/

#define RELAY_CH1_PIN 2
#define RELAY_CH2_PIN 3
#define RELAY_CH3_PIN 4
#define RELAY_CH4_PIN 5
#define RELAY_CH5_PIN 6
#define RELAY_CH6_PIN 7
#define RELAY_CH7_PIN 8
#define RELAY_CH8_PIN 9

int i = 0;

void setup() {
  pinMode(RELAY_CH1_PIN, OUTPUT);
  pinMode(RELAY_CH2_PIN, OUTPUT);
  pinMode(RELAY_CH3_PIN, OUTPUT);
  pinMode(RELAY_CH4_PIN, OUTPUT);
  pinMode(RELAY_CH5_PIN, OUTPUT);
  pinMode(RELAY_CH6_PIN, OUTPUT);
  pinMode(RELAY_CH7_PIN, OUTPUT);
  pinMode(RELAY_CH8_PIN, OUTPUT);
}

void loop() {

  if(i == 0) {
    digitalWrite(RELAY_CH8_PIN, LOW);
  }
  
  if(i == 1) {
    digitalWrite(RELAY_CH1_PIN, HIGH);
  }
  
  if(i == 2) {
    digitalWrite(RELAY_CH1_PIN, LOW);
    digitalWrite(RELAY_CH2_PIN, HIGH);
  }
  
  if(i == 3) {
    digitalWrite(RELAY_CH2_PIN, LOW);
    digitalWrite(RELAY_CH3_PIN, HIGH);
  }
  
  if(i == 4) {
    digitalWrite(RELAY_CH3_PIN, LOW);
    digitalWrite(RELAY_CH4_PIN, HIGH);
  }

  i = i + 1;
  if (i == 5) {
    i = 0;
  }
  
  delay(1000);

}
