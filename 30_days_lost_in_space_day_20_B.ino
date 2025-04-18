//Digital clock using buzzer, digital display and rotery Encoder

#include "Arduino.h"
#include <TM1637Display.h>
#include <BasicEncoder.h>

const byte CLOCK_CLK_PIN = 6;
const byte CLOCK_DIO_PIN = 5;

const byte DIAL_CLK_PIN = 2;
const byte DIAL_DT_PIN = 3;
const byte DIAL_SW_PIN = 4;
 
const byte BUZZER_PIN = 10;

BasicEncoder dial(DIAL_CLK_PIN, DIAL_DT_PIN);
TM1637Display clock_face = TM1637Display(CLOCK_CLK_PIN, CLOCK_DIO_PIN);

const byte done[] = {
    SEG_B | SEG_C | SEG_D | SEG_E | SEG_G,  // d  //0b0111101
    SEG_A | SEG_B | SEG_C | SEG_D | SEG_E | SEG_F, //O //0b1111110
    SEG_C | SEG_E | SEG_G,  //n //0b0010101
    SEG_A | SEG_D | SEG_E | SEG_F | SEG_G  //E //0b1001111
};

const byte set[] = {
    0b1011101, //S
    0b1111001, //E
    0b1000110, //-|
    0b1000000  //  -
};

static int switch_num = 0;
short int hrs = 1200;
short int mins = 0;
short int a_mins = 5;
short int a_hrs = 1200;
//unsigned int alarm_time = a_hrs + a_mins;
//unsigned int set_time = hrs + mins;

void setup(){
    clock_face.setBrightness(7);
    pinMode(DIAL_SW_PIN, INPUT_PULLUP);
    pinMode(BUZZER_PIN, OUTPUT);
    attachInterrupt(digitalPinToInterrupt(DIAL_CLK_PIN), updateEncoder, CHANGE);
    attachInterrupt(digitalPinToInterrupt(DIAL_DT_PIN), updateEncoder, CHANGE);
    attachInterrupt(digitalPinToInterrupt(DIAL_SW_PIN), updateEncoder, INPUT_PULLUP);
    Serial.begin(9600);
    bool blink_12 = true;
     while (blink_12) {
        clock_face.showNumberDecEx(1200,0b01000000);
        delay(500);
        clock_face.clear();
        delay(500);
        
        if (dial.get_change()|| digitalRead(DIAL_SW_PIN) == LOW){
            blink_12 = false;
            clock_face.setSegments(done);
            delay(100);
        }
    }
    
}

void loop(){
    //set up switch to set time, set alarm, show time
    //static int hrs = 1200; //set as a global variables so it doesn't constantly get reset
    //static int mins = 00;
    //static int switch_num = 0; 
    Serial.print("dial.get_count is ");
    Serial.println(dial.get_count());
    short int alarm_time = a_hrs + a_mins;
    short int set_time = hrs + mins;
    delay(100);
    //dial.reset();
    
    if (switch_num == 0){ //set time
        Serial.println(switch_num);
        
        if (dial.get_change()){
            mins += dial.get_count();
            dial.reset();
            Serial.print("mins is ");
            Serial.println(mins);
            if (mins > 59){
                mins = 0;
                hrs += 100;
                if (hrs > 2300) {
                    hrs = 0;
                }
            }
            if (mins < 0) {
                mins = 59;
                hrs -= 100;
                if (hrs < 0) {
                    hrs = 2300;
                }
            }
            clock_face.showNumberDecEx(set_time, 0b01000000, true);
            delay(50);
        }
        //clock_face.showNumberDecEx(set_time, 0b01000000, true);
        delay(100);
        //clock_face.clear();
        //delay(500);
        if(digitalRead(DIAL_SW_PIN) == LOW){
            delay(100);
            switch_num += 1;
        }

   }
    else if( switch_num == 1) { //show time
        Serial.println(switch_num);
         
        if(digitalRead(DIAL_SW_PIN) == LOW){
            delay(100);
            switch_num += 1;
        }
        clock_face.showNumberDecEx(set_time,0b01000000, true);
        //delay(60000); //1 minute == 60,000 miliseconds
        mins += 1;
        if (mins > 59){
                mins = 0;
                hrs += 100;
                if (hrs > 2300) {
                    hrs = 0;
                }
            }
        
        if (set_time == alarm_time){ //alarm goes off when set time matches the set alarm time and clock flashes 4 times
            for (int i = 0; i < 4; i++){
                tone(BUZZER_PIN, 110,100);
                clock_face.showNumberDecEx(set_time,0b01000000, true);
                delay(100);
                clock_face.clear();
                delay(100);
            }
        }

       
   }
    else { //set alarm
        Serial.println(switch_num);
        if(digitalRead(DIAL_SW_PIN) == LOW){
            delay(100);
            switch_num = 0;
        }
       
        clock_face.setSegments(set);
        delay(1000);
        clock_face.clear();
        display_alarm();
        clock_face.clear();
        delay(1000);
        
        if (dial.get_change()){
            a_mins += dial.get_count();
            if (a_mins > 59){
                a_mins = 0;
                a_hrs += 100;
                if (a_hrs > 2300) {
                    a_hrs = 0;
                }
            }
            if (a_mins < 0) {
                a_mins = 59;
                a_hrs -= 100;
                if (a_hrs < 0) {
                    a_hrs = 2300;
                }
            }
        }
        clock_face.showNumberDecEx(alarm_time, 0b01000000, true); //flash the alarm time
        delay(500);
        clock_face.clear();
        delay(2000);
        
        
    }
   
}
void updateEncoder(){
    dial.service();
}
void display_alarm(){
    byte ___A[] = {
        0b0000000,
        0b0000000,
        0b0000000,
        0b1110111
    };
    byte __AL[] = {
        0b0000000,
        0b0000000,
        0b1110111,
        0b0111000
    };
    byte _ALA[] = {
        0b0000000,
        0b1110111,
        0b0111000,
        0b1110111
    };
    byte ALAr[] = {
        0b1110111,
        0b0111000,
        0b1110111,
        0b1010000
    };
    byte LArn[] = {
        0b0111000,
        0b1110111,
        0b1010000,
        0b1010100
    };
    byte Arnn[] = {
        0b1110111,
        0b1010000,
        0b1010100,
        0b1010100
    };
    byte rnn_[] = {
        0b1010000,
        0b1010100,
        0b1010100,
        0b0000000
    };
    byte nn__[] = {
        0b1010100,
        0b1010100,
        0b0000000,
        0b0000000
    };
    char16_t alarm_arr[8] {
        ___A,
        __AL,
        _ALA,
        ALAr,
        LArn,
        Arnn,
        rnn_,
        nn__
    };
    for (int i = 0; i < 8; i ++){
        clock_face.setSegments(alarm_arr[i]);
        delay(100);
        clock_face.clear();
        delay(50);
    };
}

