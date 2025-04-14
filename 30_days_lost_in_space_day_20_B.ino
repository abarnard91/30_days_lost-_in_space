//Digital clock using buzzer, digital display and rotery Encoder

#include "Arduino.h"
#include <TM1637Display.h>
#include <BasicEncoder.h>

const byte CLOCK_CLK_PIN = 6;
const byte CLOCK_DIO_PIN = 5;

const byte DIAL_CLK_PIN = 3;
const byte DIAL_DT_PIN = 2;
const byte DIAL_SW_PIN = 4;

BasicEncoder dial(DIAL_CLK_PIN, DIAL_DT_PIN);
TM1637Display clock_face = TM1637Display(CLOCK_CLK_PIN, CLOCK_DIO_PIN);

void setup(){
    clock_face.setBrightness(7);
    pinMode(DIAL_SW_PIN, INPUT);
    attachInterrupt(digitalPinToInterrupt(DIAL_CLK_PIN), updateEncoder, CHANGE);
    attachInterrupt(digitalPinToInterrupt(DIAL_DT_PIN), updateEncoder, CHANGE);
}

void loop(){
    bool blink_12 = true
    while blink_12 {
        clock_face.showNumberDecEx(1200,0b01000000);
        delay(250);
        clock_face.clear();
        delay(250);
    }
    if (dial.get_change()){
        blink_12 = false;
    }

}

