#include "display.h"

Adafruit_HT1632LEDMatrix matrix = Adafruit_HT1632LEDMatrix(HT_DATA, HT_WR, HT_CS1, HT_CS2);


void setupDisplay() {
    Serial.print("Setting up display... ");

    matrix.begin(ADA_HT1632_COMMON_16NMOS);

    for (int y=0; y<TOTAL_HEIGHT; y++) {
        for (int x=0; x<TOTAL_WIDTH; x++) {
            setPixelAt(x, y);
            delay(1);
            matrix.writeScreen();
        }
    }
    delay(500);
    matrix.clearScreen();
    Serial.println();
}

Adafruit_HT1632LEDMatrix getMatrix() {
    return matrix;
}

void setPixelAt(int x, int y) {
    drawPixelAt(x, y, true);
}

void clrPixelAt(int x, int y) {
    drawPixelAt(x, y, false);
}

void drawPixelAt(int x, int y, bool set) {
    if (set) {
        matrix.setPixel(pixelAtX(x,y),pixelAtY(x,y));
    } else {
        matrix.clrPixel(pixelAtX(x,y),pixelAtY(x,y));
    }
}

// New Magic (using matrix)
// for convertig normal x,y to magical x,y
int pixelAtX(int x, int y) {
    return (y&7)+(x>>3<<3);
}
int pixelAtY(int x, int y) {
    return (x&7)+(y>>3<<3);
}
// Old Magic (using display)
// for converting x,y to pixel address
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

void drawChar5x7(int x, int y, int index) {
    int offset = index*FONT_5X7_BYTES_PER_CHAR;
    int font_w = 5;
    int font_h = 7;

    for (int w=0; w<font_w; w++) {
        for (int h=0; h<font_h; h++) {
            drawPixelAt(x+w, y+h,  (FONT_5X7[offset+h] & 1<<(font_w-1-w))  );
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

void drawChar8x16(int x, int y, int index) {
    int offset = index*FONT_8X16_BYTES_PER_CHAR;
    int font_w = 8;
    int font_h = 16;

    for (int w=0; w<font_w; w++) {
        for (int h=0; h<font_h; h++) {
            drawPixelAt(x+w, y+h,  (FONT_8X16[offset+h] & 1<<(font_w-1-w))  );
        }
    }
}

