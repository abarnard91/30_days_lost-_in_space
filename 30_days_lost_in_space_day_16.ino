#include "Arduino.h"
#include <TM1637Display.h>

const byte CLK_PIN = 6;
const byte DIO_PIN = 5;

TM1637Display hero_display = TM1637Display(CLK_PIN,DIO_PIN);

const byte all_on[] = { 0b1111111,
                        0b1111111,
                        0b1111111,
                        0b1111111};

const byte done[] = {
    SEG_B | SEG_D | SEG_E | SEG_G,  // d  //0b0101101
    SEG_A | SEG_B | SEG_C | SEG_D | SEG_E | SEG_F, //O //0b1111110
    SEG_C | SEG_E | SEG_G,  //n //0b0010101
    SEG_A | SEG_D | SEG_E | SEG_F | SEG_G  //E //0b1001111
};

void void setup(){
    hero_display.setBrightness(7); //configure brightness from 0-7
}

void void loop(){
    hero_display.clear(); //clears hero_display
    delay(1000);

    hero_display.setSegments(all_on);
    delay(1000);

    hero_display.clear();
    delay(1000);

    for (int i = 0; i < 4; i++){
        hero_display.shownumberDecEx(1200,0b01000000);
        delay(500);
        hero_display.clear();
        delay(500);
    }
    for (int i = -100; i <= 100; i++){
        hero_display.shownumberDec(i);
        delay(50);
    } 
    delay(1000);

    hero_display.clear();

    delay(1000);

    hero_display.setSegments(done);

    delay(10000);

}
