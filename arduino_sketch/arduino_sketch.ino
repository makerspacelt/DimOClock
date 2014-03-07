#include <Wire.h>
// git clone https://github.com/adafruit/RTClib.git
#include <RTClib.h>

// originaly found here but slightly modified https://github.com/gauravmm/HT1632-for-Arduino
#include "./HT1632.h"
#include "./font_3x5_numbers_a.h"
#include "./font_5x10_numbers_a.h"

#define TOTAL_WIDTH 24
#define TOTAL_HEIGHT 16
#define BUFFER_SIZE 48

#define ALARM 60*7+0

RTC_DS1307 rtc;
DateTime time, t;
byte brightness_raw;
byte brightness;

void setup () {
  Serial.begin(9600);
  
  // CS1; CS2; CS3; CS4; WR; DATA
  HT1632.begin(10, 9, 8, 7, 11, 12);
  
  Wire.begin();
  rtc.begin();

  // internal pullup on phototransistors
  pinMode(A3, INPUT);
  digitalWrite(A3, HIGH);

  // lets blink every second
  pinMode(13, OUTPUT);
/*
  Serial.println("Setting clock...");
  rtc.adjust(DateTime(2014,3,5, 18,35,0));
*/
}


void drawSmallTime(byte hours, byte minutes, byte seconds, int x, int y) {
  // T-23:45
  int16_t amins = ALARM;
  int16_t tmins = hours*60 + minutes;
  
  // show countdown to next alarm after 4 hours past from current
  if (amins+4*60 < tmins) {
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
  HT1632.drawImage(FONT_3X5, 3, 5, x+3*0+0, y, brightness*FONT_3X5_BYTES_PER_CHAR);
  
  HT1632.drawImage(FONT_3X5, 3, 5, x+3*1+1, y, sign*FONT_3X5_BYTES_PER_CHAR);
  HT1632.drawImage(FONT_3X5, 3, 5, x+3*2+1, y, h1*FONT_3X5_BYTES_PER_CHAR);
  HT1632.drawImage(FONT_3X5, 3, 5, x+3*3+2, y, h2*FONT_3X5_BYTES_PER_CHAR);
  //HT1632.drawImage(FONT_3X5, 3, 5, x+3*4+2, y, spacer*FONT_3X5_BYTES_PER_CHAR);
  HT1632.drawImage(FONT_3X5, 3, 5, x+3*5+2, y, m1*FONT_3X5_BYTES_PER_CHAR);
  HT1632.drawImage(FONT_3X5, 3, 5, x+3*6+3, y, m2*FONT_3X5_BYTES_PER_CHAR);

  // bin seconds  
  seconds &= 0b111111; // 6 bytes (max 63); we have 6 leds to operate on;
  byte s1 = (seconds>>2)&0b1111;
  byte s2 = seconds&0b11;
  s1 = (s1>>3&1) ^ ((s1>>2&1)<<1) ^ ((s1>>1&1)<<2) ^ ((s1&1)<<3);
  s2 = (s2>>1&1) ^ ((s2&1)<<1);
  char i[] = { s1, s2 };
  HT1632.drawImage(i, 1, 8, x+3*4+3, y-1, 0);
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

void showTimeOnDisplay(DateTime &t) {
  HT1632.drawTarget(0);
  HT1632.clear();
  drawBigTime(t.hour(), t.minute(), 0,0);
  drawSmallTime(t.hour(), t.minute(), t.second(), 0,11);
  HT1632.render();
  HT1632.setBrightness( brightness+1 , 0b1111);
}
void updateBrightness() {
  int light = (1024-analogRead(A3))>>6;
  brightness_raw -= brightness_raw>>4;
  brightness_raw += light;
  brightness = (brightness_raw>>3)-1;
  if (brightness > 15) 
    brightness = 15;

}
void loop () {
  
  // Time stuff
  t = rtc.now();
  if ( t.second() != time.second() ) {
    digitalWrite(13,HIGH);
    delay(50);
    digitalWrite(13,LOW);

    showTimeOnDisplay(t);
    time = t;
  }
  
  // Light stuff; averaging of 16 samples
  updateBrightness();
  
  delay(100);
}

