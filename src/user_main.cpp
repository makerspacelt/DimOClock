#include <Arduino.h>

#include "wifi.h"
#include "display.h"

#define PN_LDR A0

#define ALARM 60*8+0


byte brightness_raw;
byte brightness;

void setup () {
    Serial.begin(921600);
    Serial.println("\nBooting...");

    Serial.println("Setting up LDR...");
    // internal pullup on phototransistors
    pinMode(PN_LDR, INPUT);
    digitalWrite(PN_LDR, HIGH);

    setupDisplay();
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
  
    drawChar3x5(x+0  , y, brightness);
    drawChar3x5(x+4  , y, sign);
    drawChar3x5(x+8  , y, h1);
    drawChar3x5(x+12 , y, h2);
    drawChar3x5(x+17 , y, m1);
    drawChar3x5(x+21 , y, m2);

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
  
    drawChar5x10(x+0  , y, h1);
    drawChar5x10(x+6  , y, h2);
    drawChar5x10(x+13 , y, m1);
    drawChar5x10(x+19 , y, m2);
}

void showTimeOnDisplay(NTPClient t) {
    //getDisplay().clearScreen();
    drawBigTime(t.getHours(), t.getMinutes(), 0,0);
    drawSmallTime(t.getHours(), t.getMinutes(), t.getSeconds(), 0,11);
    getDisplay().writeScreen();
    getDisplay().setBrightness(brightness+1);
}
void updateBrightness() {
    int light = (1024-analogRead(PN_LDR))>>6;
    brightness_raw -= brightness_raw>>4;
    brightness_raw += (15-light);
    brightness = (brightness_raw>>3)-1;
    if (brightness > 15)
        brightness = 15;
}
void loop () {
    updateNTP();

    showTimeOnDisplay(getNTPClient());

    updateBrightness();

    delay(100);
}

