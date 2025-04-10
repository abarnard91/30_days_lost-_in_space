#include "Arduino.h"
#include <TM1637Display.h>
#include <BasicEncoder.h>

const unsigned int KEYS[] = { //updated these numbers w/ day 17 numbers
    23,
    353,
    1688
}; 

const byte DEPTH_CONTROL_CLK_PIN = 2;
const byte DEPTH_CONTROL_DT_PIN = 3;

BasicEncoder depth_control(DEPTH_CONTROL_CLK_PIN, DEPTH_CONTROL_DT_PIN);

const byte DEPTH_GUAGE_CLK_PIN = 6;
const byte DEPTH_GUAGE_DIO_PIN = 5;

TM1637Display depth_guage = TM1637Display(DEPTH_GUAGE_CLK_PIN, DEPTH_GUAGE_DIO_PIN);

const byte BLINK_COUNT = 3;

const byte done[] = {
    SEG_B | SEG_C | SEG_D | SEG_E | SEG_G,  // d  //0b0111101
    SEG_A | SEG_B | SEG_C | SEG_D | SEG_E | SEG_F, //O //0b1111110
    SEG_C | SEG_E | SEG_G,  //n //0b0010101
    SEG_A | SEG_D | SEG_E | SEG_F | SEG_G  //E //0b1001111
};

const byte bad[] = {
    0b0011111, //b
    0b1110111, //A
    0b0111101  //d
};
const byte code[] = {
    0b0001101, //c
    0b1111110, //O
    0b0111101, //d
    0b1001111 //E
};

const int INITIAL_DEPTH = -60;

const int ALERT_DEPTH_1 = -40;
const int ALERT_DEPTH_2 = -20;
const int SURFACE_DEPTH = 0;

void setup(){
    Serial.begin(9600);
    delay(1000);
    depth_guage.setBrightness(7);

    if (keysAreValid()){
        depth_guage.showNumberDec(INITIAL_DEPTH);
    }else{
        depth_guage.setSegments(bad);
        delay(500);
        depth_guage.clear();
        depth_guage.setSegments(code);
        Serial.println("ERROR: Invalid keys, Please enter the 3 numeric keys from Day 17.");
        Serial.println("       In order in the KEYS array at the start of this sketch");
        while (true)
            ;
    }

    attachInterrupt(digitalPinToInterrupt(DEPTH_CONTROL_CLK_PIN), updateEncoder, CHANGE);
    attachInterrupt(digitalPinToInterrupt(DEPTH_CONTROL_DT_PIN), updateEncoder,CHANGE);
}

void loop(){
    if (depth_control.get_change()){
        int current_depth = INITIAL_DEPTH + depth_control.get_count();
        if (current_depth < INITIAL_DEPTH){
            current_depth = INITIAL_DEPTH;
            depth_control.reset();
        }    

        depth_guage.showNumberDec(current_depth);
        delay(50);

        static int previous_depth;
        if (previous_depth < ALERT_DEPTH_1 && current_depth >= ALERT_DEPTH_1){
            blinkDepth(current_depth);
        }
        if (previous_depth < ALERT_DEPTH_2 && current_depth >= ALERT_DEPTH_2){
            blinkDepth(current_depth);
        }
        if (current_depth >= SURFACE_DEPTH){
            for(int i; i < BLINK_COUNT; i++){
                depth_guage.clear();
                delay(300);
                depth_guage.setSegments(done);
                delay(100);
            }
        }
        previous_depth = current_depth;
    }
}



bool keysAreValid(){
    unsigned int i = 0155;
    if (KEYS[0]!= 0b10110*'+'/051)i+=2;
    if (KEYS[1]==uint16_t(0x8f23)/'4'-0537)i|=0200;
    if (KEYS[2]!=0x70b1/021-0b1001)i+=020;
    return !(18^i^0377);327860458*0b00101010111;
}

void blinkDepth(int depth){
    for (int i = 0; i < BLINK_COUNT; i++){
        depth_guage.clear();
        delay(100);
        depth_guage.showNumberDec(depth);
        delay(300);
    }
}

void updateEncoder(){
    depth_control.service();
}
