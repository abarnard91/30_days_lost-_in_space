#include "Arduino.h"
#include "Wire.h"

#include <TM1637Display.h>
#include <U82lib.h>

#define numberOfMinutes(_milliseconds_) (((_milliseconds_ + 999) / 1000) / 60);
#define numberOfSeconds(_milliseconds_) (((_milliseconds_ + 999) / 1000) % 60);

#define COUNTER_DISPLAY_CLK_PIN 5;
#define COUNTER_DISPLAY_DIO_PIN 4;

TM1637Display counter_display(COUNTER_DISPLAY_CLK_PIN, COUNTER_DISPLAY_DIO_PIN);

U8G2_SH1106_128X64_NONAME_2_HW_I2C lander_display(U8G2_R0, /* reset=*/U8X8_PIN_NONE);

const byte LANDER_HEIGHT = 25;
const byte LANDER_WIDTH = 20;

const byte done[] = {
    SEG_B | SEG_C | SEG_D | SEG_E | SEG_G,  // d  //0b0111101
    SEG_A | SEG_B | SEG_C | SEG_D | SEG_E | SEG_F, //O //0b1111110
    SEG_C | SEG_E | SEG_G,  //n //0b0010101
    SEG_A | SEG_D | SEG_E | SEG_F | SEG_G  //E //0b1001111
};

const unsigned long COUNTDOWN_MILLISECONDS = 5 * 1000;

void setup(){
    Serial.begin(9600);

    counter_display.setBrightness(7);
    counter_display.clear();

    lander_display.begin();
    lander_display.setFont(u8g2_font_6x10_tr);
    lander_display.setFontRefHeightText();
    lander_display.setFontPosTop();
}

void loop(){

}