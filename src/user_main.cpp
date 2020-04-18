#include <Arduino.h>

#include <Wire.h>

// originaly found here but slightly modified https://github.com/gauravmm/HT1632-for-Arduino
#include "./inc/HT1632.h"
#include "./font/font_3x5_numbers_a.h"
#include "./font/font_5x10_numbers_a.h"
#include "./inc/wifi.h"

#define PN_DATA 13
#define PN_WR 14
#define PN_CS1 16
#define PN_LDR A0

#define TOTAL_WIDTH 24
#define TOTAL_HEIGHT 16
#define BUFFER_SIZE 48

#define ALARM 60*8+0


byte brightness_raw;
byte brightness;

void setup () {
  Serial.begin(921600);
  Serial.println("\nBooting...");

  Serial.println("Preparing display...");
  // CS1; CS2; CS3; CS4; WR; DATA
  HT1632.begin(PN_CS1, PN_WR, PN_DATA);
  
  Serial.println("Starting Wire...");
  Wire.begin();
  
  Serial.println("Setting up LDR...");
  // internal pullup on phototransistors
  pinMode(PN_LDR, INPUT);
  digitalWrite(PN_LDR, HIGH);

  setupWifi();
  setupNTP();
}


void drawSmallTime(byte hours, byte minutes, byte seconds, int x, int y) {
  // T-23:45
  int16_t amins = ALARM;
  int16_t tmins = hours*60 + minutes;
  
  // show countdown to next alarm after 6 hours past from current
  if (amins+6*60 < tmins) {
    amins += 24*60;
  }

  byte sign;  
  if (amins < tmins) {
    hours = (tmins-amins)/60;
    minutes = (tmins-amins)%60;
    sign = FONT_3X5_PLUS;
  } else {
    hours = (amins-tmins)/60;
    minutes = (amins-tmins)%60;
    seconds = 59-seconds;
    sign = FONT_3X5_MINUS;
  }

  byte h1 = hours/10;
  byte h2 = hours%10;
  byte m1 = minutes/10;
  byte m2 = minutes%10;
  
  //HT1632.drawImage(FONT_3X5, 3, 5, x+3*0+0, y, FONT_3X5_T*FONT_3X5_BYTES_PER_CHAR);
//  HT1632.drawImage(FONT_3X5, 3, 5, x+3*0+0, y, brightness*FONT_3X5_BYTES_PER_CHAR);
  
//  HT1632.drawImage(FONT_3X5, 3, 5, x+3*1+1, y, sign*FONT_3X5_BYTES_PER_CHAR);

//  HT1632.drawImage(FONT_3X5, 3, 5, x+3*2+1, y, h1*FONT_3X5_BYTES_PER_CHAR);
//  HT1632.drawImage(FONT_3X5, 3, 5, x+3*3+2, y, h2*FONT_3X5_BYTES_PER_CHAR);

//  HT1632.drawImage(FONT_3X5, 3, 5, x+3*5+2, y, m1*FONT_3X5_BYTES_PER_CHAR);
//  HT1632.drawImage(FONT_3X5, 3, 5, x+3*6+3, y, m2*FONT_3X5_BYTES_PER_CHAR);
  HT1632.drawImage(FONT_3X5, 3, 5, 4, 3, m2*FONT_3X5_BYTES_PER_CHAR);

  /*
  // bin seconds  
  seconds &= 0b111111; // 6 bytes (max 63); we have 6 leds to operate on;
  byte s1 = (seconds>>2)&0b1111;
  byte s2 = seconds&0b11;
  s1 = (s1>>3&1) ^ ((s1>>2&1)<<1) ^ ((s1>>1&1)<<2) ^ ((s1&1)<<3);
  s2 = (s2>>1&1) ^ ((s2&1)<<1);
  char i[] = { s1, s2 };
  HT1632.drawImage(i, 1, 8, x+3*4+3, y-1, 0);
  */
}

void drawBigTime(byte hours, byte minutes, int x, int y) {
  // 23 45
  byte h1 = hours/10;
  byte h2 = hours%10;
  byte m1 = minutes/10;
  byte m2 = minutes%10;
  
  HT1632.drawImage(FONT_5X10, 5, 10, x+5*0+0, y, h1*FONT_5X10_BYTES_PER_CHAR);
  HT1632.drawImage(FONT_5X10, 5, 10, x+5*1+1, y, h2*FONT_5X10_BYTES_PER_CHAR);
  HT1632.drawImage(FONT_5X10, 5, 10, x+5*2+3, y, m1*FONT_5X10_BYTES_PER_CHAR);
  HT1632.drawImage(FONT_5X10, 5, 10, x+5*3+4, y, m2*FONT_5X10_BYTES_PER_CHAR);
}

void showTimeOnDisplay(NTPClient t) {
  HT1632.drawTarget(0);
  HT1632.clear();
//  drawBigTime(t.getHours(), t.getMinutes(), 0,0);
  drawSmallTime(t.getHours(), t.getMinutes(), t.getSeconds(), 0,11);
  HT1632.render();
  HT1632.setBrightness( brightness+1 , 0b1111);
}
void updateBrightness() {
  int light = (1024-analogRead(PN_LDR))>>6;
  //Serial.println(light);
  brightness_raw -= brightness_raw>>4;
  brightness_raw += (15-light);
  brightness = (brightness_raw>>3)-1;
  if (brightness > 15) 
    brightness = 15;
  //Serial.println(brightness);
}
void loop () {
  // Time stuff
  updateNTP();

  showTimeOnDisplay(getNTPClient());
  
  // Light stuff; averaging of 16 samples
  updateBrightness();
  
  delay(3000);

}

