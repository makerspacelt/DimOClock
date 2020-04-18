#ifndef DISPLAY_H
#define DISPLAY_H

#include <Arduino.h>

#include <Wire.h>
#include <SPI.h>
#include "Adafruit_HT1632.h"

#include "font_3x5.h"
#include "font_5x10.h"

#define HT_DATA 13
#define HT_WR 14
#define HT_CS 16
#define TOTAL_WIDTH 24
#define TOTAL_HEIGHT 16
#define BUFFER_SIZE 48

void setupDisplay();

int pixelAt(int x, int y);
void setPixelAt(int x, int y);
void crlPixelAt(int x, int y);
void drawPixelAt(int x, int y, bool set);
void drawChar3x5(int x, int y ,int index);
void drawChar5x10(int x, int y ,int index);

Adafruit_HT1632 getDisplay();

#endif // DISPLAY_H