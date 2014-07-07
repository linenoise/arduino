#include <Wire.h>
#include <LCD.h>
#include <LiquidCrystal_I2C.h>

#define I2C_ADDR 0x27
#define BACKLIGHT_PIN 3
#define En_pin 2
#define Rw_pin 1
#define Rs_pin 0
#define D4_pin 4
#define D5_pin 5
#define D6_pin 6
#define D7_pin 7

#define COLS 20
#define ROWS 4

LiquidCrystal_I2C lcd(I2C_ADDR,En_pin,Rw_pin,Rs_pin,D4_pin,D5_pin,D6_pin,D7_pin);
char character;
char newline = '\n';
int row, col, i, j;
char framebuffer[4][20];

void setup() {
  lcd.begin (20,4,LCD_5x8DOTS);
  lcd.setBacklightPin(BACKLIGHT_PIN,POSITIVE);
  lcd.setBacklight(HIGH);

  Serial.begin(115200);
      
  for (row = 0; row < ROWS; row++){
    for(col = 0; col < COLS; col++){
     framebuffer[row][col] = ' ';
    }
  }
  print_framebuffer();
 
  row = 0;
  col = 0;
}

void loop() {
  if (Serial.available()) {
    character = Serial.read();
    if (character == newline) { 
      print_framebuffer();

      col = 0;
      if (row == 3) {
        
        // Scroll the framebuffer
        for (i = 0; i < ROWS - 1; i++){
          for(j = 0; j < COLS; j++){
            framebuffer[i][j] = framebuffer[i+1][j];   
          }
        }
        for(j = 0; j < COLS; j++){
          framebuffer[ROWS - 1][j] = ' ';   
        }

      } else {
        row++;
      }
    } else {
      framebuffer[row][col] = character;
      col++;
    }
  }
}

void print_framebuffer() {
  // Print the framebuffer
  for (i = 0; i < ROWS; i++){
    lcd.setCursor(0, i);
    for(j = 0; j < COLS; j++){
     lcd.print(framebuffer[i][j]);   
    }
  }
}

