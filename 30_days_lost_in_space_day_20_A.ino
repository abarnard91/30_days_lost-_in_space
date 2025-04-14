//RGB color selector with Rotery Encoder, key pad and RGB LED
#include "Arduino.h"
#include <Keypad.h>
#include <BasicEncoder>

const byte ROW_PIN = 4;

const byte COL_PINS[4] = {5, 6, 7, 8};

const char BUTTON_MAP[4] = {'R', 'G', 'B', 'O'};

Keypad rgb_keypad = Keypad(makeKeymap(BUTTON_MAP),ROW_PIN,COL_PINS, 1, 4);

const byte RED_PIN = 11;
const byte GREEN_PIN = 10;
const byte BLUE_PIN = 9;

const byte COLOR_CTRL_CLK_PIN = 2;
const byte COLOR_CTRL_DT_PIN = 3;

BasicEncoder color_dial(COLOR_CTRL_CLK_PIN, COLOR_CTRL_DT_PIN);

void setup(){
    pinMode(RED_PIN, OUTPUT);
    pinMode(BLUE_PIN, OUTPUT);
    pinMode(GREEN_PIN, OUTPUT);

    displayColor(255,255,255); //Start with bright white to know everything is working

    attachInterrupt(digitalPinToInterrupt(COLOR_CTRL_CLK_PIN_CLK_PIN), updateEncoder, CHANGE);
    attachInterrupt(digitalPinToInterrupt(COLOR_CTRL_DT_PIN), updateEncoder,CHANGE);
}

void loop(){
    current_button = rgb_keypad.getKey();
    if (current_button == 'R'){
        button_press(RED_PIN);
    }
    else if (current_button == 'G'){
        button_press(GREEN_PIN);
    }
    else if (current_button == 'B'){
        button_press(BLUE_PIN);
    }
    else if (current_button == 'O'){
        displayColor(0, 0, 0);
    }


}

void displayColor(byte red_intensity, byte green_intensity, byte blue_intensity){
  analogWrite(RED_PIN, red_intensity);
  analogWrite(GREEN_PIN, green_intensity);
  analogWrite(BLUE_PIN, blue_intensity);
}

void button_press(color_pin){
    int color_level = 0;
    
    if color_dial.get_change(){
        color_level += color_dial.get_count();
        if (starting_color < 0){
            starting_color = 0;
            color_dial.reset();
        }
        analogWrite(color_pin, color_level);   
    }
}

