// Juke box using keypad and passive buzzer
#include "Arduino.h";
#include <Keypad.h>;

const byte ROW_PINS[ROWS] = { 5, 4, 3, 2};
const byte COL_PINS[COLS] = { 6, 7, 8, 13};

const char BUTTONS[ROWS][COLS] = {
  { '1', '2', '3', 'A'},
  { '4', '5', '6', 'B'},
  { '7', '8', '9', 'C'},
  { '*', '0', '#', 'D'}
};

Keypad heroKeypad = Keypad(makeKeymap(BUTTONS), ROW_PINS, COL_PINS, ROWS, COLS);

const byte BUZZER_PIN = 12;

void setup(){

}

void loop(){

}