//Digital clock using buzzer, digital display and rotery Encoder

#include "Arduino.h"
#include <TM1637Display.h>
#include <BasicEncoder.h>

const byte CLOCK_CLK_PIN = 6;
const byte CLOCK_DIO_PIN = 5;

const byte DIAL_CLK_PIN = 2;
const byte DIAL_DT_PIN = 3;
const byte DIAL_SW_PIN = 4;

BasicEncoder dial(DIAL_CLK_PIN, DIAL_DT_PIN);
TM1637Display clock_face = TM1637Display(CLOCK_CLK_PIN, CLOCK_DIO_PIN);

const byte done[] = {
    SEG_B | SEG_C | SEG_D | SEG_E | SEG_G,  // d  //0b0111101
    SEG_A | SEG_B | SEG_C | SEG_D | SEG_E | SEG_F, //O //0b1111110
    SEG_C | SEG_E | SEG_G,  //n //0b0010101
    SEG_A | SEG_D | SEG_E | SEG_F | SEG_G  //E //0b1001111
};

void setup(){
    clock_face.setBrightness(7);
    pinMode(DIAL_SW_PIN, INPUT);
    attachInterrupt(digitalPinToInterrupt(DIAL_CLK_PIN), updateEncoder, CHANGE);
    attachInterrupt(digitalPinToInterrupt(DIAL_DT_PIN), updateEncoder, CHANGE);
    bool blink_12 = true;
     while (blink_12) {
        clock_face.showNumberDecEx(1200,0b01000000);
        delay(500);
        clock_face.clear();
        delay(500);
        
        if (dial.get_change()){
            blink_12 = false;
            clock_face.setSegments(done);
            delay(10000);
        }
    }
    
}

void loop(){
    
   

}
void updateEncoder(){
    dial.service();
}
