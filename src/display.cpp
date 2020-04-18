#include "display.h"

Adafruit_HT1632 display = Adafruit_HT1632(HT_DATA, HT_WR, HT_CS);


void setupDisplay() {
    Serial.print("Setting up display... ");

    display.begin(ADA_HT1632_COMMON_16NMOS);

    for (int y=0; y<TOTAL_HEIGHT; y++) {
        for (int x=0; x<TOTAL_WIDTH; x++) {
            setPixelAt(x, y);
            delay(5);
            display.writeScreen();
        }
    }
    delay(500);
    display.clearScreen();
    Serial.println();
}

Adafruit_HT1632 getDisplay() {
    return display;
}

void setPixelAt(int x, int y) {
    drawPixelAt(x, y, true);
}

void clrPixelAt(int x, int y) {
    drawPixelAt(x, y, false);
}

void drawPixelAt(int x, int y, bool set) {
    if (set) {
        display.setPixel(pixelAt(x,y));
    } else {
        display.clrPixel(pixelAt(x,y));
    }
}

int pixelAt(int x, int y) {
    if (y<=7) y=7-y;
    if (y>7) y=7-y+(TOTAL_HEIGHT);
    return (x*TOTAL_HEIGHT +y);
}

void drawChar3x5(int x, int y, int index) {
    int offset = index*FONT_3X5_BYTES_PER_CHAR;
    int font_w = 3;
    int font_h = 5;

    for (int w=0; w<font_w; w++) {
        for (int h=0; h<font_h; h++) {
            drawPixelAt(x+w, y+h,  (FONT_3X5[offset+h] & 1<<(font_w-1-w))  );
        }
    }
}

void drawChar5x10(int x, int y, int index) {
    int offset = index*FONT_5X10_BYTES_PER_CHAR;
    int font_w = 5;
    int font_h = 10;

    for (int w=0; w<font_w; w++) {
        for (int h=0; h<font_h; h++) {
            drawPixelAt(x+w, y+h,  (FONT_5X10[offset+h] & 1<<(font_w-1-w))  );
        }
    }
}

