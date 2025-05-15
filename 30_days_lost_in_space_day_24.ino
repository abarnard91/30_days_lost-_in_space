#include "Arduino.h"
#include <TM1637Display.h>
#include <U8g2lib.>
#include "Wire.h"

#define numberOfMinutes(_milliseconds_) (((_milliseconds_ + 999) / 1000) / 60)
#define numberOfSeconds(_milliseconds_) (((_milliseconds_ + 999) / 1000) % 60)

const byte COUNTER_DISPLAY_DIO_PIN = 4;
const byte COUNTER_DISPLAY_CLK_PIN = 5;

TM1637Display counter_display(COUNTER_DISPLAY_CLK_PIN, COUNTER_DISPLAY_DIO_PIN);

const byte THRUST_LEVER_PIN = 9;
const byte SYSTEMS_LEVER_PIN = 8;
const byte CONFIRM_LEVER_PIN = 7;

const byte BUZZER_PIN = 6;

U8G2_SH106_128X64_NONAME_2_HW_I2C lander_display(U8G2_R0, /*reset=*/U8x8_PIN_NONE);

const byte LANDER_HEIGHT = 25;
const byte LANDER_WIDTH = 20;

const byte done[] = {
    SEG_B | SEG_C | SEG_D | SEG_E | SEG_G,  // d  //0b0111101
    SEG_A | SEG_B | SEG_C | SEG_D | SEG_E | SEG_F, //O //0b1111110
    SEG_C | SEG_E | SEG_G,  //n //0b0010101
    SEG_A | SEG_D | SEG_E | SEG_F | SEG_G  //E //0b1001111
};

const unsigned long COUNTDOWN_MILLISECONDS = 70UL * 1000UL;

enum LIFTOFF_STATE {
    INIT,
    PENDING,
    COUNTDOWN,
    LIFTOFF,
    ABORT,
};

void setup(){
    Serial.begin(9600);
    counter_display.setBrightness(7);
    counter_display.clear();
    
    lander_display.begin();
    lander_display.setFont(u8g2_font_6x10_tr);
    lander_display.setFontRefHeightText();
    lander_display.setFontPosTop();

    pinMode(THRUST_LEVER_PIN, INPUT);
    pinMode(SYSTEMS_LEVER_PIN, INPUT);
    pinMode(CONFIRM_LEVER_PIN, INPUT);
    
    lander_display.clearDisplay();
}

const unsigned long MIN_LOOP_TIME = 200;

void loop(){
    static unsigned long timeRemaining = COUNTDOWN_MILLISECONDS;
    static unsigned long countdown_start_time;
    static enum LIFTOFF_STATE liftoff_state = INIT;

    static bool loop_toggle = true;

    unsigned long loop_start_time = millis();

    bool thrust_lever = digitalRead(THRUST_LEVER_PIN);
    bool systems_lever = digitalRead(SYSTEMS_LEVER_PIN);
    bool confirm_lever = digitalRead(CONFIRM_LEVER_PIN);

    updateLanderDisplay(liftoff_state, thrust_lever, systems_lever, confirm_lever);

    if (liftoff_state == INIT){
        if (!thrust_lever && !systems_lever && !confirm_lever){
            noTone(BUZZER_PIN);
            liftoff_state = PENDING;
        }else {
            if (loop_toggle){
                tone(BUZZER_PIN, 100);
            }else{
                noTone(BUZZER_PIN);
            }
        }
    }else if (liftoff_state == PENDING){
        if (thrust_lever && systems_lever && confirm_lever){
            for (int i =0; i < 3, i++){
                counter_display.clear();
                delay(MIN_LOOP_TIME);
                displayCounter(COUNTDOWN_MILLISECONDS);
                delay(MIN_LOOP_TIME);
            }
            countdown_start_time = millis();
            liftoff_state = COUNTDOWN;
        }
    }else if (liftoff_state == COUNTDOWN){
        unsigned long elapsed_time = millis();
        if (elapsed_time > COUNTDOWN_MILLISECONDS){
            timeRemaining = 0;
            liftoff_state = LIFTOFF;
        }else {
            timeRemaining = COUNTDOWN_MILLISECONDS - elapsed_time;
        }
        if (!thrust_lever || !systems_lever || !confirm_lever){
            liftoff_state = ABORT;
        }
        displayCounter(timeRemaining);
    }else if (liftoff_state == LIFTOFF) {
        counter_display.setSegments(done);
        tone(BUZZER_PIN, 300);
        delay(200);
        tone(BUZZER_PIN, 500);
        delay(400);
        tone(BUZZER_PIN, 38, 5000);

        while(true){
            updateLanderDisplay(liftoff_state, true,true,true);
        }
    }else if (liftoff_state == ABORT){
        tone(BUZZER_PIN, 100, 1000);
        delay(5000);
        liftoff_state = INIT;
    }

    unsigned loon loop_time = millis() - loop_start_time;
    if (loop_time < MIN_LOOP_TIME){
        delay(MIN_LOOP_TIME - loop_time);
    }

    loop_toggle = !loop_toggle;
}

const byte MAX_LANDER_SPEED = 5;

void updateLanderDisplay(enum LIFTOFF_STATE liftoff_state, bool thrust_lever, bool systems_lever,bool confirm_lever){
    static int lander_height = lander_display.getDisplayHeight() - LANDER_HEIGHT;
    static byte current_lander_speed = 1;

    do { 
        lander_display.setFontPosTop();
        byte y_offset = drawString(0, 0, "Exploration Lander");
        y_offset = drawString(0, y_offset, "Liftoff Sequence");

        if (liftoff_state == LIFTOFF){
            const char LIFTOFF_TEXT[] = "Liftoff!";
            byte y_center = y_offset + ((lander_display.getDisplayHeight()-y_offset) / 2);
            lander_display.setFontPosCenter();
            static byte text_width = lander_display.getStrWidth(LIFTOFF_TEXT);
            static byte x_left = ((lander_display.getDisplayWidth()-LANDER_WIDTH) / 2) - (text_width / 2);
            lander_display.drawStr(x_left, y_center, LIFTOFF_TEXT);
        }else if(liftoff_state == ABORT){
            
        }
    }
}