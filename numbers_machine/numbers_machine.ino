 
/*
 numbers_machine
 
 A blinking lights demo for arduino
 Updated by Danne Stayskal
 
 Library originally added 18 Apr 2008
 by David A. Mellis
 library modified 5 Jul 2009
 by Limor Fried (http://www.ladyada.net)
 example added 9 Jul 2009
 by Tom Igoe
 modified 22 Nov 2010
 by Tom Igoe
 
 This is based off ofexample code in the public domain.
 http://www.arduino.cc/en/Tutorial/LiquidCrystal
*/

// include the library code:
#include <LiquidCrystal.h>

// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

void setup() {
  // set up the LCD's number of columns and rows:
  lcd.begin(16, 3);
  // Print a message to the LCD.
  lcd.print("numbers_machine");
  pinMode(7, OUTPUT);
  pinMode(8, OUTPUT);
  pinMode(9, OUTPUT);
  pinMode(10, OUTPUT);
}

void loop() {
  // set the cursor to column 0, line 1
  lcd.setCursor(0, 1);

  // print the number of seconds since reset:
  int number = random(0,32767); //millis()/100;
  lcd.print("     ");
  lcd.print(number);

  // Flash the LEDs
  if ( number % 2 == 0 ){
    digitalWrite(7,HIGH);
  } else {
    digitalWrite(7,LOW);
  }
  if ( number % 2 == 1 ){
    digitalWrite(8,HIGH);
  } else {
    digitalWrite(8,LOW);
  }
  if ( number % 4 > 1 ){
    digitalWrite(9,HIGH);
  } else {
    digitalWrite(9,LOW);
  }
  if ( number % 8 > 1 ){
    digitalWrite(10,HIGH);
  } else {
    digitalWrite(10,LOW);
  }
  
  delay(333);

}

